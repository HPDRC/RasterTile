using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    struct PointD
    {
        public double lon, lat;

        public PointD(double lon, double lat)
        {
            this.lon = lon;
            this.lat = lat;
        }
    }

    struct RectangleD
    {
        public double top, right, bottom, left;

        public RectangleD(double top, double right, double bottom, double left)
        {
            this.top = top;
            this.right = right;
            this.bottom = bottom;
            this.left = left;
        }
    }

    class Mask
    {
        private RectangleD box;

        private int[] newRange = new int[6];

        /// <summary>
        /// type: 0/skip, 1/newrange, 2/remove
        /// </summary>
        public int type = 0;

        public Mask(string description)
        {
            string[] segments = description.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
            box = new RectangleD(double.Parse(segments[0]), double.Parse(segments[1]), double.Parse(segments[2]), double.Parse(segments[3]));
            if (segments.Length < 5)
                throw new Exception("mask not recognized: " + description);
            if (segments[4] == "SKIP")
                type = 0;
            else if (segments[4] == "REMOVE")
                type = 2;
            else if (segments[4] == "NEWRANGE")
            {
                type = 1;
                for (int i = 0; i < 6; i++)
                    newRange[i] = Int32.Parse(segments[5 + i]);
            }
            else
                throw new Exception("mask type not recognized: " + description);
        }

        public Mask(Point start, Point end, TileXY tilexy)
        {
            if (end.X < start.X || end.Y < start.Y)
                throw new Exception("Invalid rectangle!");
            PointD a = GetLonLat(start, tilexy);
            PointD b = GetLonLat(end, tilexy);
            box = new RectangleD(a.lat, b.lon, b.lat, a.lon);
        }

        private PointD GetLonLat(Point pixelOffset, TileXY tilexy)
        {
            double lat, lon;
            BingProjection.PixelXYToLatLong(tilexy.x * 256 + pixelOffset.X, tilexy.y * 256 + pixelOffset.Y, (int)tilexy.level, out lat, out lon);
            return new PointD(lon, lat);
        }

        public Point[] GetRelativePolygon(TileXY tilexy)
        {
            Point[] points = new Point[4];
            points[0] = BingProjection.LatLongToPixelXY(box.top, box.left, tilexy.level);
            points[1] = BingProjection.LatLongToPixelXY(box.top, box.right, tilexy.level);
            points[2] = BingProjection.LatLongToPixelXY(box.bottom, box.right, tilexy.level);
            points[3] = BingProjection.LatLongToPixelXY(box.bottom, box.left, tilexy.level);
            for (int i = 0; i < 4; i++)
            {
                points[i].X -= tilexy.x * 256;
                points[i].Y -= tilexy.y * 256;
            }
            return points;
        }

        public Point[] GetUtmCorrectedRelativePolygon(TileXY tilexy, int zone)
        {
            double[] input = new double[4];
            double[] result = new double[8];
            input[0] = box.top;
            input[1] = box.right;
            input[2] = box.bottom;
            input[3] = box.left;
            for (int i = 0; i < 4; i++)
                input[i] = input[i] * Math.PI / 180;
            MUtilities.LatLonRectangleExpandByUtm(zone, input, result);
            for (int i = 0; i < 8; i++)
                result[i] = result[i] * 180 / Math.PI;
            Point[] points = new Point[4];
            points[0] = BingProjection.LatLongToPixelXY(result[0], result[1], tilexy.level);
            points[1] = BingProjection.LatLongToPixelXY(result[2], result[3], tilexy.level);
            points[2] = BingProjection.LatLongToPixelXY(result[4], result[5], tilexy.level);
            points[3] = BingProjection.LatLongToPixelXY(result[6], result[7], tilexy.level);
            for (int i = 0; i < 4; i++)
            {
                points[i].X -= tilexy.x * 256;
                points[i].Y -= tilexy.y * 256;
            }
            return points;
        }

        public bool ContainsTilePixel(TileXY tilexy, Point pixelOffset)
        {
            double lat, lon;
            BingProjection.PixelXYToLatLong((int)tilexy.x * 256 + pixelOffset.X, (int)tilexy.y * 256 + pixelOffset.Y, (int)tilexy.level, out lat, out lon);
            return ContainsLatLon(lat, lon) ? true : false;
        }

        /// <summary>
        /// check if this mask contains a tile
        /// </summary>
        /// <param name="tilexy"></param>
        /// <returns></returns>
        public bool ContainsTile(TileXY tilexy)
        {
            double lat, lon;
            BingProjection.PixelXYToLatLong((int)tilexy.x * 256, (int)tilexy.y * 256, (int)tilexy.level, out lat, out lon);
            if (!ContainsLatLon(lat, lon))
                return false;

            BingProjection.PixelXYToLatLong((int)tilexy.x * 256 + 256, (int)tilexy.y * 256, (int)tilexy.level, out lat, out lon);
            if (!ContainsLatLon(lat, lon))
                return false;

            BingProjection.PixelXYToLatLong((int)tilexy.x * 256, (int)tilexy.y * 256 + 256, (int)tilexy.level, out lat, out lon);
            if (!ContainsLatLon(lat, lon))
                return false;

            BingProjection.PixelXYToLatLong((int)tilexy.x * 256 + 256, (int)tilexy.y * 256 + 256, (int)tilexy.level, out lat, out lon);
            if (!ContainsLatLon(lat, lon))
                return false;

            return true;
        }

        private bool ContainsLatLon(double lat, double lon)
        {
            return lat <= box.top && lat >= box.bottom && lon >= box.left && lon <= box.right;
        }

        public override string ToString()
        {
            return string.Format("{0:F7},{1:F7},{2:F7},{3:F7},", box.top, box.right, box.bottom, box.left) + (type == 0 ? "SKIP" : 
                (type == 2 ? "REMOVE" : string.Format("NEWRANGE,{0},{1},{2},{3},{4},{5}", newRange[0], newRange[1], newRange[2], newRange[3], newRange[4], newRange[5])));
        }
    }
}
