using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public static class Projection
    {
        public static void LatLon_to_bing_ext(ref double lonDegree, ref double latDegree)
        {
            double lonClipped = Clip(ref lonDegree, 180);
            double latClipped = Clip(ref latDegree, 85.0511);
            lonDegree = Helper.DegreeToRadian(lonDegree);
            latDegree = Helper.DegreeToRadian(latDegree);
            int result = MUtilities.TransformProjection("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs ", "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +units=m +k=1.0 +nadgrids=@null +no_defs", ref lonDegree, ref latDegree);
            if (result != 0)
                throw new Exception("projection transformation failed with error code " + result + " for lat/lon = " + latDegree + "," + lonDegree);

            //// extend result
            //if (lonClipped != 0)
            //    lonDegree += lonClipped * 20037500;
            //if (latClipped != 0)
            //    latDegree += latClipped * 20037500;
        }

        /// <summary>
        /// clip x to [-range,range] and return cliped value/range
        /// </summary>
        /// <param name="x"></param>
        /// <param name="range"></param>
        /// <returns></returns>
        private static double Clip(ref double x, double range)
        {
            double clippedValue = 0;
            if (x > range)
            {
                clippedValue = x - range;
                x = range;
            }
            else if (x < -range)
            {
                clippedValue = x - range;
                x = -range;
            }
            return clippedValue / range;
        }
    }
}