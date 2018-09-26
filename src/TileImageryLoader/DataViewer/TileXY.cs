using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    struct TileMeta2
    {
        public int imgOffset;
        public int tdataPathId;
        private int _imgLength;

        public int imgLength
        {
            get { return _imgLength < 0 ? -_imgLength : _imgLength; }
        }

        public bool hasAlpha
        {
            get { return _imgLength < 0 ? true : false; }
        }

        public TileMeta2(MSTileMetaInfo meta, int dataPathId)
        {
            this.tdataPathId = dataPathId;
            this.imgOffset = (int)meta.imgOffset;
            this._imgLength = (int)meta.imgLength;
            if (meta.hasAlpha == 1)
                this._imgLength = -this._imgLength;
        }
    }

    struct TileMeta
    {
        public int imgOffset;
        public int imgLength;
        public int tdataPathId;
        public byte zone;
        public bool hasAlpha;

        public TileMeta(MSTileMetaInfo meta, int dataPathId)
        {
            this.imgOffset = (int)meta.imgOffset;
            this.imgLength = (int)meta.imgLength;
            this.hasAlpha = meta.hasAlpha == 0 ? false : true;
            this.tdataPathId = dataPathId;
            this.zone = (byte)meta.zone;
        }
    }

    struct TileXY
    {
        public int x, y;
        public byte level;

        public TileXY(MSTileXY mtxy)
        {
            x = (int)mtxy.x;
            y = (int)mtxy.y;
            level = mtxy.level;
        }

        public override String ToString()
        {
            return x + "," + y + "," + level;
        }

        public override int GetHashCode()
        {
            return (x << 19) + ((y << 19) >> 13) + (level & 0x3F);
        }

        public TileXY GetNeighbor(int offsetx, int offsety)
        {
            TileXY txy = this;
            txy.x += offsetx;
            txy.y += offsety;
            return txy;
        }

        public TileXY GetParent()
        {
            TileXY txy = this;
            if (txy.level == 0)
                return txy;
            txy.level--;
            txy.x /= 2;
            txy.y /= 2;
            return txy;
        }

        public TileXY GetSon()
        {
            TileXY txy = this;
            txy.level++;
            txy.x *= 2;
            txy.y *= 2;
            return txy;
        }

        /// <summary>
        /// get it's 0,0 | 1,0 | 0,1 | 1,1 neighbors
        /// </summary>
        /// <returns></returns>
        public TileXY[] GetSiblings()
        {
            return new TileXY[] { this, this.GetNeighbor(0,1), this.GetNeighbor(1,0), this.GetNeighbor(1,1) };
        }

        #region projection related

        public void GetUtmXy(out int utmx, out int utmy, out double res)
        {
            utmx = x;
            int utmLevel = level;
            int utmyMax = 32768;
            if (utmLevel <= 20)
            {
                utmyMax = utmyMax / (1 << (20 - utmLevel));
                res = 1 << (20 - utmLevel);
            }
            else
            {
                utmyMax = utmyMax * (1 << (utmLevel - 20));
                res = 1.0 / (1 << (utmLevel - 20));
            }
            utmy = utmyMax - y;
        }

        public void GetNASALatLon(out double lat, out double lon)
        {
            // get x, y on (1, 0.5) sized map
            double x1 = ((double)(x)) / (1 << level);
            double y1 = ((double)(y)) / (1 << level);

            // calc latlon
            lon = (x1 - 0.5) * 360;
            lat = (0.25 - y1) * 360;
        }

        #endregion
    }
}
