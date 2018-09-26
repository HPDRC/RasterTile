using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    class TDataNavigator
    {
        #region configurable parameters

        private const int columnCount = 3, rowCount = 3;
        private const int centerX = 1, centerY = 1;
        private const int imageSize = 256;
        private Brush brushMaskSkip = new SolidBrush(Color.FromArgb(90, Color.Red));
        private Brush brushMaskNewRange = new SolidBrush(Color.FromArgb(140, Color.Green));
        private Brush brushMaskRemove = new SolidBrush(Color.FromArgb(90, Color.Purple));
        private Brush brushCoverPNG = new SolidBrush(Color.FromArgb(90, Color.Yellow));
        private Pen penMouseDragLine = new Pen(Color.Red);
        private Pen penMouseDragLineUtmCorrected = new Pen(Color.Blue);
        private Color bgColor = Color.FromArgb(128, Color.Yellow);

        #endregion

        #region members

        /// <summary>
        /// used to look up index when tilexy is known
        /// </summary>
        private Dictionary<TileXY, TileMeta> tiles = new Dictionary<TileXY, TileMeta>();

        /// <summary>
        /// store tdata paths
        /// </summary>
        private string[] tdataPaths;

        /// <summary>
        /// the picture box to render image on
        /// </summary>
        private PictureBox pbox;

        private ComboBox cbGuessedPixelValidRanges;

        private TextBox tbUtmCorrectionZone;

        /// <summary>
        /// the textbox to show tile info on
        /// </summary>
        private TextBox tbTileInfo;

        /// <summary>
        /// current tile coordiante which should be rendered in the center of image
        /// </summary>
        private TileXY centerTilexy;

        private int levelMin = 0, levelMax = 0;

        /// <summary>
        /// list of masks
        /// </summary>
        public List<Mask> masks = new List<Mask>();

        /// <summary>
        /// cover png tiles with a half transparent color so that it looks different from non-png tiles
        /// </summary>
        public bool distinguishPng = false;

        public bool isMouseDown = false;

        public Point mousePositionNow, mousePositionStart;

        private bool _isShowValidGraph = false;

        /// <summary>
        /// switch to show valid graph mode, must set pixelValidRange BEFORE the switch.
        /// </summary>
        public bool isShowValidGraph
        {
            set 
            {
                _isShowValidGraph = value;
                if (pixelValidRange == null)
                    _isShowValidGraph = false;
                Paint();
            }
        }

        public PixelValidRange pixelValidRange = null;

        #endregion

        #region construction

        /// <summary>
        /// initialize map from a dir which contains *.tdata and *.tmeta files
        /// </summary>
        /// <param name="dataDir"></param>
        public TDataNavigator(string[] tmetaPaths, PictureBox pbox, TextBox tbTileInfo, ComboBox cbGuessedPixelValidRanges, TextBox tbUtmCorrectionZone)
        {
            this.pbox = pbox;
            this.tbTileInfo = tbTileInfo;
            this.cbGuessedPixelValidRanges = cbGuessedPixelValidRanges;
            this.tbUtmCorrectionZone = tbUtmCorrectionZone;

            // create dataPath array
            tdataPaths = new string[tmetaPaths.Length];
            for (int i = 0; i < tmetaPaths.Length; i++)
                tdataPaths[i] = tmetaPaths[i].ToLower().Replace(".tmeta", ".tdata");

            // load all meta info
            levelMin = Int32.MaxValue;
            levelMax = 0;
            for (int i = 0; i < tmetaPaths.Length; i++)
            {
                using (MMetaFile metaFile = new MMetaFile(tmetaPaths[i]))
                {
                    int recordCnt = metaFile.GetRecordCnt();
                    for (int j = 0; j < recordCnt; j++)
                    {
                        MSTileMetaInfo meta = metaFile.Read(j);
                        if ((int)meta.tilexy.level < levelMin)
                        {
                            levelMin = (int)meta.tilexy.level;
                            centerTilexy = new TileXY(meta.tilexy);
                        }
                        levelMax = Math.Max(levelMax, (int)meta.tilexy.level);
                        tiles.Add(new TileXY(meta.tilexy), new TileMeta(meta, i));
                    }
                }
            }

            // zoom in center tile to a reasonable level
            while (true)
            {
                int validSonCount = 0;
                TileXY validSonXY = centerTilexy;
                foreach (TileXY txy in centerTilexy.GetSon().GetSiblings())
                {
                    if (tiles.ContainsKey(txy))
                    {
                        validSonCount++;
                        validSonXY = txy;
                    }
                }
                if (validSonCount == 1)
                    centerTilexy = validSonXY;
                else
                    break;
            }

            // guess an initial utm correction zone
            double lat, lon;
            BingProjection.PixelXYToLatLong(centerTilexy.x * 256 + 128, centerTilexy.y * 256 + 128, (int)centerTilexy.level, out lat, out lon);
            tbUtmCorrectionZone.Text = Helper.LongitudeToUtmZone(lon).ToString();

            Paint();
        }

        #endregion

        #region paint

        public void Paint(bool originalOnly = false, bool centerOnly = false)
        {
            // clear image
            Graphics g = Graphics.FromImage(pbox.Image);
            if (!originalOnly)
                g.Clear(bgColor);
            else
                g.Clear(Color.FromArgb(0,0,0,0));

            // draw tiles
            for (int j = 0; j < rowCount; j++)
            {
                for (int i = 0; i < columnCount; i++)
                {
                    int offsetx = i - centerX;
                    int offsety = j - centerY;
                    if (centerOnly && (offsetx != 0 || offsety != 0))
                        continue;
                    TileXY xy = centerTilexy.GetNeighbor(offsetx, offsety);
                    if (tiles.ContainsKey(xy))
                    {
                        TileMeta tmeta = tiles[xy];
                        byte[] imgBuff = MDataFile.Read(tdataPaths[tmeta.tdataPathId], tmeta.imgOffset, tmeta.imgLength);
                        Rectangle rect = new Rectangle(i * imageSize, j * imageSize, imageSize, imageSize);
                        g.DrawImage(Image.FromStream(new MemoryStream(imgBuff)), rect);
                        if (!originalOnly && !_isShowValidGraph && distinguishPng && tmeta.hasAlpha)
                            g.FillRectangle(brushCoverPNG, rect);
                    }
                }
            }

            if (!originalOnly)
            {
                // get current utm correction zone
                int zone = Int32.Parse(tbUtmCorrectionZone.Text);

                // get top left tile xy
                TileXY topLeftTileXY = centerTilexy.GetNeighbor(-centerX, -centerY);

                // draw masks
                if (!_isShowValidGraph)
                {
                    foreach (Mask m in masks)
                    {
                        // draw utm corrected masks
                        g.FillPolygon(m.type == 0 ? brushMaskSkip : (m.type == 1 ? brushMaskNewRange : brushMaskRemove), m.GetUtmCorrectedRelativePolygon(topLeftTileXY, zone));
                    }
                }

                // show valid graph
                if (_isShowValidGraph && pixelValidRange != null)
                {
                    pixelValidRange.DrawValidGraph((Bitmap)pbox.Image);
                }

                // draw mouse line
                if (isMouseDown)
                {
                    g.DrawRectangle(penMouseDragLine, Math.Min(mousePositionNow.X, mousePositionStart.X),
                        Math.Min(mousePositionNow.Y, mousePositionStart.Y),
                        Math.Abs(mousePositionNow.X - mousePositionStart.X),
                        Math.Abs(mousePositionNow.Y - mousePositionStart.Y));

                    // draw utm corrected line
                    Point start = new Point(Math.Min(mousePositionStart.X, mousePositionNow.X), Math.Min(mousePositionStart.Y, mousePositionNow.Y));
                    Point end = new Point(Math.Max(mousePositionStart.X, mousePositionNow.X), Math.Max(mousePositionStart.Y, mousePositionNow.Y));
                    Mask mTmp = new Mask(start, end, topLeftTileXY);
                    g.DrawPolygon(penMouseDragLineUtmCorrected, mTmp.GetUtmCorrectedRelativePolygon(topLeftTileXY, zone));
                }
            }

            pbox.Refresh();
            ShowCenterTileInfo();
        }

        private void ShowCenterTileInfo()
        {
            if (!tiles.ContainsKey(centerTilexy))
            {
                tbTileInfo.Text = "center tile is blank!";
                return;
            }

            TileMeta centerTile = tiles[centerTilexy];
            string result = "X,Y,Z/" + centerTilexy + "  TileCount/" + tiles.Count + "    LevelRange/" + levelMin + "->" + levelMax;

            // show utm info
            int utmx, utmy;
            double res;
            centerTilexy.GetUtmXy(out utmx, out utmy, out res);
            result += "\r\nIf Utm: x,y,zone,res/" + utmx + "," + utmy + "," + centerTile.zone + "," + res
                + " x1,x2,y1,y2/" + utmx * 400 * res + "," + (utmx + 1) * 400 * res + "," + (utmy + 1) * 400 * res + "," + utmy * 400 * res;

            // show bing info
            result += "\r\nIf Bing: QKey/" + BingProjection.GetQuadKey(centerTilexy) + "  LatLon/" + BingProjection.TileXYToLatLong(centerTilexy);

            // show NASA info
            double lat, lon;
            centerTilexy.GetNeighbor(0, 1).GetNASALatLon(out lat, out lon);
            result += "\r\nIf NASA: LeftBot/" + lat.ToString("F6") + "," + lon.ToString("F6");
            centerTilexy.GetNeighbor(1, 0).GetNASALatLon(out lat, out lon);
            result += "  RightTop/" + lat.ToString("F6") + "," + lon.ToString("F6");

            tbTileInfo.Text = result + "\r\n\r\n";
        }

        #endregion

        #region verify

        public void VerifyCenterAsBing(PictureBox pbVerify)
        {
            string quadKey = BingProjection.GetQuadKey(centerTilexy);
            string url = "http://ecn.dynamic.t0.tiles.virtualearth.net/comp/ch/" + quadKey + "?mkt=en-us&it=A,G,L&shading=hill&og=4&n=z";
            pbVerify.Load(url);
            Paint(false, true);
        }

        public void VerifyCenterAsNASA(PictureBox pbVerify)
        {
            double lat, lon;
            string url = "http://worldwind27.arc.nasa.gov/wms/virtualearth?service=WMS&request=GetMap&version=1.1.1&srs=EPSG:4326&layers=ve&styles=&transparent=TRUE&format=image/png&width=512&height=512&bbox=";
            centerTilexy.GetNeighbor(0, 1).GetNASALatLon(out lat, out lon);
            url += lon.ToString("F9") + "," + lat.ToString("F9") + ",";
            centerTilexy.GetNeighbor(1, 0).GetNASALatLon(out lat, out lon);
            url += lon.ToString("F9") + "," + lat.ToString("F9");
            pbVerify.Load(url);
            Paint(false, true);
        }

        #endregion

        #region navigation

        public void MoveTo(int offsetx, int offsety, bool isZoomOut, bool isZoomIn)
        {
            // get target location
            TileXY targetXY = centerTilexy;
            if (isZoomOut)
                targetXY = centerTilexy.GetParent();
            else if (isZoomIn)
            {
                TileXY[] sons = centerTilexy.GetSon().GetSiblings();
                foreach (TileXY txy in sons)
                {
                    targetXY = txy;
                    if (tiles.ContainsKey(targetXY))
                        break;
                }
            }
            else
                targetXY = centerTilexy.GetNeighbor(offsetx, offsety);

            // move the center and paint
            centerTilexy = targetXY;
            Paint();
        }

        public bool FindTileInMask(Mask m, bool pngTileOnly, ref TileXY txy)
        {
            foreach (KeyValuePair<TileXY, TileMeta> pair in tiles)
            {
                if (!m.ContainsTile(pair.Key))
                    continue;
                if (!pngTileOnly || pair.Value.hasAlpha)
                {
                    txy = pair.Key;
                    while (true)
                    {
                        bool hasParent = false;
                        TileXY[] parents = txy.GetParent().GetSiblings();
                        for (int i = 0; i < 4; i++)
                        {
                            if (m.ContainsTile(parents[i]) && tiles.ContainsKey(parents[i]) && (!pngTileOnly || tiles[parents[i]].hasAlpha))
                            {
                                txy = parents[i];
                                hasParent = true;
                                break;
                            }
                        }
                        if (!hasParent || txy.level == 0)
                            break;
                    }
                    return true;
                }
            }
            return false;
        }

        #endregion

        #region drag event

        public void OnMouseDown(Point position)
        {
            mousePositionStart = position;
            isMouseDown = true;
        }

        public void OnMouseMove(Point currentLocation)
        {
            mousePositionNow = currentLocation;
            Paint();
        }

        public void OnMouseUp(Point mousePositionEnd, int actionType)
        {
            isMouseDown = false;
            Bitmap bm = (Bitmap)pbox.Image;

            if (mousePositionEnd != mousePositionStart) // it's a drag
            {
                // get real start and end
                Point start = new Point(Math.Min(mousePositionStart.X, mousePositionEnd.X), Math.Min(mousePositionStart.Y, mousePositionEnd.Y));
                Point end = new Point(Math.Max(mousePositionStart.X, mousePositionEnd.X), Math.Max(mousePositionStart.Y, mousePositionEnd.Y));

                Mask mask = new Mask(start, end, centerTilexy.GetNeighbor(-centerX, -centerY));
                switch (actionType)
                {
                    case 0:
                        // get statistics
                        Paint(); // Very important! Paint first to get rid of red line drawn by drag of mouse
                        byte minr = byte.MaxValue, ming = byte.MaxValue, minb = byte.MaxValue;
                        byte maxr = 0, maxg = 0, maxb = 0;
                        for (int w = start.X; w <= end.X; w++)
                        {
                            for (int h = start.Y; h <= end.Y; h++)
                            {
                                Color c = bm.GetPixel(w, h);
                                if (c.A == 0)
                                    continue;
                                maxr = Math.Max(maxr, c.R);
                                maxg = Math.Max(maxg, c.G);
                                maxb = Math.Max(maxb, c.B);
                                minr = Math.Min(minr, c.R);
                                ming = Math.Min(ming, c.G);
                                minb = Math.Min(minb, c.B);
                            }
                        }
                        FormNav.ShowMessage("Min RGB: " + minr + "/" + ming + "/" + minb);
                        FormNav.ShowMessage("Max RGB: " + maxr + "/" + maxg + "/" + maxb);
                        break;
                    case 1:
                        // zoom in
                        TileXY txy = centerTilexy;
                        if (FindTileInMask(mask, false, ref txy))
                            centerTilexy = txy;
                        break;
                    case 2:
                        // add to mask
                        masks.Add(mask);
                        FormNav.ShowMessage("new mask added");
                        break;
                    case 3:
                        // find PNG
                        TileXY txy2 = centerTilexy;
                        if (FindTileInMask(mask, true, ref txy2))
                            centerTilexy = txy2;
                        break;
                    case 4:
                        // guess valid pixel range
                        // important, paint to original images before doing the calculation
                        Paint(true);

                        // first, get pixel cnt array
                        int[,] pixelCnt = new int[3, 256];
                        for (int i = 0; i < 3; i++)
                            for (int j = 0; j < 256; j++)
                                pixelCnt[i, j] = 0;
                        for (int w = start.X; w <= end.X; w++)
                        {
                            for (int h = start.Y; h <= end.Y; h++)
                            {
                                Color c = bm.GetPixel(w, h);
                                if (c.A == 0)
                                    continue;
                                pixelCnt[0, c.R]++;
                                pixelCnt[1, c.G]++;
                                pixelCnt[2, c.B]++;
                            }
                        }
                        // then, convert to pixel sum
                        for (int i = 0; i < 3; i++)
                            for (int j = 1; j < 256; j++)
                                pixelCnt[i, j] += pixelCnt[i, j - 1];
                        double[] ratios = new double[] {0.6, 0.7, 0.8, 0.9, 0.95, 0.96, 0.965, 0.97, 0.975, 0.98, 0.985, 0.99, 0.995, 0.998};
                        int[,] guessedRanges = new int[3, ratios.Length];
                        for (int i = 0; i < 3; i++)
                        {
                            int j = 0;
                            for (int k = 0; k < ratios.Length; k++)
                            {
                                for (; j < 50; j++)
                                {
                                    if (((double)pixelCnt[i, j]) / pixelCnt[i, 50] > ratios[k])
                                        break;
                                }
                                guessedRanges[i, k] = j;
                            }
                        }
                        cbGuessedPixelValidRanges.Items.Clear();
                        for (int i = 0; i < ratios.Length; i++)
                        {
                            cbGuessedPixelValidRanges.Items.Add((guessedRanges[0, i]+1) + "," + (guessedRanges[1, i]+1) + "," + (guessedRanges[2, i]+1) + ",255,255,255");
                        }
                        FormNav.ShowMessage("Pixel Valid Ranges are guessed");
                        break;
                    default:
                        break;
                }
                // paint anyway
                Paint();
            }
            else    // it's a single click
            {
                FormNav.ShowMessage("Color: " + bm.GetPixel(mousePositionEnd.X, mousePositionEnd.Y));

                // get top left tile
                TileXY tileTopLeft = centerTilexy.GetNeighbor(-centerX, -centerY);
                for (int i = 0; i < masks.Count; i++)
                {
                    if (masks[i].ContainsTilePixel(tileTopLeft, mousePositionEnd))
                        FormNav.ShowMessage("Mask " + i + ": " + masks[i].ToString());
                }
            }
        }

        #endregion

        #region mask

        public string GetMasksDescription()
        {
            string result = "TRIM_MASKS = ";
            foreach (Mask mask in masks)
            {
                result += mask.ToString() + "&";
            }
            if (masks.Count > 0)
                result = result.Substring(0, result.Length - 1);
            return result;
        }

        public void LoadMasksFromString(string mstring)
        {
            foreach (string text in mstring.Split(new char[] { '&' }, StringSplitOptions.RemoveEmptyEntries))
            {
                Mask m = new Mask(text);
                masks.Add(m);
            }
            FormNav.ShowMessage(masks.Count.ToString() + " masks loaded");
        }

        #endregion
    }
}