using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    class TDataOverviewMaker
    {
        private string[] tdataPaths;

        private List<TileMeta2> tiles = new List<TileMeta2>();
        private List<TileXY> tilexys = new List<TileXY>();

        private int zMin, zMax;

        public TDataOverviewMaker(string[] tmetaPaths)
        {
            // create dataPath array
            tdataPaths = new string[tmetaPaths.Length];
            for (int i = 0; i < tmetaPaths.Length; i++)
                tdataPaths[i] = tmetaPaths[i].ToLower().Replace(".tmeta", ".tdata");

            // load all meta info
            for (int i = 0; i < tmetaPaths.Length; i++)
            {
                using (MMetaFile metaFile = new MMetaFile(tmetaPaths[i]))
                {
                    int recordCnt = metaFile.GetRecordCnt();
                    for (int j = 0; j < recordCnt; j++)
                    {
                        MSTileMetaInfo metainfo = metaFile.Read(j);
                        tiles.Add(new TileMeta2(metainfo, i));
                        tilexys.Add(new TileXY(metainfo.tilexy));
                    }
                    metaFile.Close();
                }
            }

            zMin = zMax = tilexys[0].level;
            for (int i = 0; i < tiles.Count; i++)
            {
                zMin = Math.Min(zMin, tilexys[i].level);
                zMax = Math.Max(zMax, tilexys[i].level);
            }
        }

        private byte[] GetImage(int index)
        {
            return MDataFile.Read(tdataPaths[tiles[index].tdataPathId], tiles[index].imgOffset, tiles[index].imgLength);
        }

        public void GenerateOverview(string path)
        {
            // get image size
            Bitmap testSizeImage = new Bitmap(new MemoryStream(GetImage(0)));
            int tileWidth = testSizeImage.Width;
            int tileHeight = testSizeImage.Height;
            int shrink = (int)Math.Pow(2, Config.TDATA_OVERVIEW_EXTEND_LEVEL);
            int shrinkWidth = tileWidth / shrink;
            int shrinkHeight = tileHeight / shrink;

            // find a proper level to generate the overview
            int xMin = 0, yMin = 0, xMax = 0, yMax = 0, z = 0;
            for (z = zMax; z >= zMin; z--)
            {
                xMax = yMax = 0;
                xMin = yMin = Int32.MaxValue;
                foreach (TileXY xy in tilexys)
                {
                    if (xy.level != z)
                        continue;
                    xMin = Math.Min(xy.x, xMin);
                    xMax = Math.Max(xy.x, xMax);
                    yMin = Math.Min(xy.y, yMin);
                    yMax = Math.Max(xy.y, yMax);
                }
                if (((yMax - yMin + 1.0) * (xMax - xMin + 1.0)) * shrinkWidth * shrinkHeight <= Config.TDATA_OVERVIEW_MAX_SIZE)
                    break;
            }
            if (z < zMin)
                z = zMin;

            // draw the image
            Bitmap bm = new Bitmap(shrinkWidth * (xMax - xMin + 1), shrinkHeight * (yMax - yMin + 1));
            Graphics g = Graphics.FromImage(bm);
            Brush coverBrush = new SolidBrush(Color.FromArgb(128, Color.Yellow));
            g.Clear(Color.Yellow);
            for (int i = 0; i < tiles.Count; i++)
            {
                if (tilexys[i].level != z)
                    continue;
                int x = (tilexys[i].x - xMin) * shrinkWidth;
                int y = (tilexys[i].y - yMin) * shrinkHeight;
                g.DrawImage(new Bitmap(new MemoryStream(GetImage(i))), x, y, shrinkWidth, shrinkHeight);
                if (!tiles[i].hasAlpha)
                    g.FillRectangle(coverBrush, x, y, shrinkWidth, shrinkHeight);
            }
            bm.Save(path, ImageFormat.Png);
            FormMain.ShowMessage("overview saved to " + path);
        }

        /// <summary>
        /// use a summary picture to show where void pixels exist
        /// </summary>
        /// <param name="path"></param>
        public void GenerateVoidSummary(string path)
        {
            // will use only the last level
            // find boundary
            int xMin = Int32.MaxValue, yMin = Int32.MaxValue, xMax = 0, yMax = 0;
            foreach (TileXY xy in tilexys)
            {
                if (xy.level != zMax)
                    continue;
                xMin = Math.Min(xy.x, xMin);
                xMax = Math.Max(xy.x, xMax);
                yMin = Math.Min(xy.y, yMin);
                yMax = Math.Max(xy.y, yMax);
            }

            // see if it's necessary to shrink
            int shrink = 1;
            int tileSize = Config.TDATA_VOID_SUMMARY_BASIC_SIZE;
            while (true)
            {
                if ((yMax - yMin + 1.0) * (xMax - xMin + 1.0) / shrink / shrink <= Config.TDATA_VOID_SUMMARY_MAX_SIZE / tileSize / tileSize)
                    break;
                shrink++;
            }

            // draw the picture
            int width = (xMax - xMin + 1) / shrink + 1;
            int height = (yMax - yMin + 1) / shrink + 1;
            Bitmap bm = new Bitmap(width * tileSize, height * tileSize);
            Graphics g = Graphics.FromImage(bm);
            Brush brush = new SolidBrush(Color.Red);
            g.Clear(Color.White);
            for (int i = 0; i < tilexys.Count; i++)
            {
                if (tilexys[i].level != zMax)
                    continue;
                if (tiles[i].hasAlpha == false)
                    continue;
                int x = (int)(tilexys[i].x - xMin) / shrink * tileSize;
                int y = (int)(tilexys[i].y - yMin) / shrink * tileSize;
                g.FillRectangle(brush, x, y, tileSize, tileSize);
            }
            bm.Save(path, ImageFormat.Jpeg);
            FormMain.ShowMessage("void summary saved to " + path);
        }

        public void GenerateDetailedView(string pathImg1, string pathImg2, string pathTxt)
        {
            // get image size
            Bitmap testSizeImage = new Bitmap(new MemoryStream(GetImage(0)));
            int tileWidth = testSizeImage.Width;
            int tileHeight = testSizeImage.Height;
            int pngZMaxCnt = 0;
            for (int i = 0; i < tilexys.Count; i++)
            {
                if (tilexys[i].level != zMax)
                    continue;
                if (tiles[i].hasAlpha)
                    pngZMaxCnt++;
            }

            // find a proper level to generate view1
            int maxTileCnt = Config.TDATA_DETAILEDVIEW_MAX_SIZE / tileWidth / tileHeight;
            int z = 0, pngCnt = 0;
            for (z = zMin; z <= zMax; z++)
            {
                pngCnt = 0;
                for (int i = 0; i < tilexys.Count; i++)
                {
                    if (tilexys[i].level != z)
                        continue;
                    if (tiles[i].hasAlpha)
                        pngCnt++;
                }
                if (pngCnt > maxTileCnt / 3)
                    break;
            }
            if (z > zMax)
                z = zMax;

            // generate view1
            int xSize = Config.TDATA_DETAILEDVIEW_MAX_WIDTH / tileWidth;
            int ySize = pngCnt / xSize + 1;
            Bitmap bm = new Bitmap(tileWidth * xSize, tileHeight * ySize);
            Graphics g = Graphics.FromImage(bm);
            g.Clear(Color.Yellow);
            for (int i = 0, j = 0; i < tiles.Count; i++)
            {
                if (tiles[i].hasAlpha == false)
                    continue;
                if (tilexys[i].level != z)
                    continue;

                g.DrawImage(new Bitmap(new MemoryStream(GetImage(i))), (j / ySize) * tileWidth, (j % ySize) * tileHeight);
                j++;
                if (j >= xSize * ySize)
                    break;
            }
            bm.Save(pathImg1, ImageFormat.Png);
            FormMain.ShowMessage("detailed view1 saved to " + pathImg1);

            if (z != zMax)
            {
                // find the proper probability to generate view2
                double p = (double)maxTileCnt / (double)pngZMaxCnt;
                Random rand = new Random();

                // generate view2
                Bitmap bm2 = new Bitmap(tileWidth * xSize, tileHeight * ySize);
                Graphics g2 = Graphics.FromImage(bm2);
                g2.Clear(Color.Yellow);
                for (int i = 0, j = 0; i < tiles.Count; i++)
                {
                    if (tiles[i].hasAlpha == false)
                        continue;
                    if (tilexys[i].level != zMax)
                        continue;
                    if (rand.NextDouble() > p)
                        continue;

                    g2.DrawImage(new Bitmap(new MemoryStream(GetImage(i))), (j / ySize) * tileWidth, (j % ySize) * tileHeight);
                    j++;
                    if (j >= xSize * ySize)
                        break;
                }
                bm2.Save(pathImg2, ImageFormat.Png);
                FormMain.ShowMessage("detailed view2 saved to " + pathImg2);
            }
            else
            {
                FormMain.ShowMessage("no need to generate detailed view2 because it's the same with view1");
            }

            // find the proper probability to generate statistics
            double pStat = 0;
            int zStat = zMax;
            if (pngZMaxCnt < 2000)
                pStat = 1;
            else
            {
                zStat = zMax - 2;
                int pngZStatCnt = 0;
                for (int i = 0; i < tiles.Count; i++)
                {
                    if (tilexys[i].level != zStat)
                        continue;
                    if (tiles[i].hasAlpha)
                        pngZStatCnt++;
                }
                pStat = 2000.0 / (double)pngZStatCnt;
            }
            Random random = new Random();
            Dictionary<Color, int> colorCnt = new Dictionary<Color, int>();
            for (int i = 0; i < tiles.Count; i++)
            {
                if (tiles[i].hasAlpha == false)
                    continue;
                if (tilexys[i].level != zStat)
                    continue;
                if (random.NextDouble() > pStat)
                    continue;

                Bitmap bmTile = new Bitmap(new MemoryStream(GetImage(i)));

                // get color statistics
                for (int w = 0; w < tileWidth; w++)
                {
                    for (int h = 0; h < tileHeight; h++)
                    {
                        Color c = bmTile.GetPixel(w, h);
                        if ((c.R > 40 && c.R < 210) || (c.G > 40 && c.G < 210) || (c.B > 40 && c.B < 210))
                            continue;
                        if (!colorCnt.ContainsKey(c))
                            colorCnt.Add(c, 0);
                        else
                            colorCnt[c]++;
                    }
                }
            }

            // save color statistics info
            List<KeyValuePair<Color, int>> colorList = colorCnt.ToList();
            colorList.Sort(delegate(KeyValuePair<Color, int> firstPair, KeyValuePair<Color, int> nextPair)
            {
                return nextPair.Value - firstPair.Value;
            });
            string content = "";
            for (int i = 0; i < colorList.Count && i < 50; i++)
                content += colorList[i].Key + " " + colorList[i].Value + "\r\n";
            File.WriteAllText(pathTxt, content);
            FormMain.ShowMessage("color statistics saved to " + pathTxt);
        }
    }
}
