using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TileImagerySystem_CLR;
using System.Drawing;

namespace TileImageryLoader
{
    /// <summary>
    /// bing projection related transformation
    /// </summary>
    static class BingProjection
    {
        public static string GetQuadKey(TileXY tilexy)
        {
            StringBuilder quadKey = new StringBuilder();
            for (int i = tilexy.level; i > 0; i--)
            {
                char digit = '0';
                int mask = 1 << (i - 1);
                if ((tilexy.x & mask) != 0)
                {
                    digit++;
                }
                if ((tilexy.y & mask) != 0)
                {
                    digit++;
                    digit++;
                }
                quadKey.Append(digit);
            }
            return quadKey.ToString();
        }

        public static string TileXYToLatLong(TileXY tilexy)
        {
            double lat, lon;
            PixelXYToLatLong((int)tilexy.x * 256, (int)tilexy.y * 256, (int)tilexy.level, out lat, out lon);
            return lat.ToString("F6") + "," + lon.ToString("F6");
        }

        public static string TileXYToLatLong(MSTileXY tilexy)
        {
            double lat, lon;
            PixelXYToLatLong((int)tilexy.x * 256, (int)tilexy.y * 256, (int)tilexy.level, out lat, out lon);
            return lat.ToString("F6") + "," + lon.ToString("F6");
        }

        public static void PixelXYToLatLong(int pixelX, int pixelY, int levelOfDetail, out double latitude, out double longitude)
        {
            double mapSize = MapSize(levelOfDetail);
            double x = (Clip(pixelX, 0, mapSize - 1) / mapSize) - 0.5;
            double y = 0.5 - (Clip(pixelY, 0, mapSize - 1) / mapSize);

            latitude = 90 - 360 * Math.Atan(Math.Exp(-y * 2 * Math.PI)) / Math.PI;
            longitude = 360 * x;
        }

        public static Point LatLongToPixelXY(double latitude, double longitude, int levelOfDetail)
        {
            latitude = Clip(latitude, MinLatitude, MaxLatitude);
            longitude = Clip(longitude, MinLongitude, MaxLongitude);

            double x = (longitude + 180) / 360;
            double sinLatitude = Math.Sin(latitude * Math.PI / 180);
            double y = 0.5 - Math.Log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * Math.PI);

            uint mapSize = MapSize(levelOfDetail);
            return new Point((int)Clip(x * mapSize + 0.5, 0, mapSize - 1), (int)Clip(y * mapSize + 0.5, 0, mapSize - 1));
        }

        #region private members

        private const double EarthRadius = 6378137;
        private const double MinLatitude = -85.05112878;
        private const double MaxLatitude = 85.05112878;
        private const double MinLongitude = -180;
        private const double MaxLongitude = 180;

        private static double Clip(double n, double minValue, double maxValue)
        {
            return Math.Min(Math.Max(n, minValue), maxValue);
        }

        private static uint MapSize(int levelOfDetail)
        {
            return (uint)256 << levelOfDetail;
        }

#endregion
    }
}
