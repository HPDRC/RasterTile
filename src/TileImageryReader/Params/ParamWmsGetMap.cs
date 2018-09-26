using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public class ParamWmsGetMap : Param
    {
        public double top, right, bottom, left;
        public int width, height;

        public ParamWmsGetMap(HttpRequest request)
            : base(request)
        {
            width = GetIntParam("width");
            height = GetIntParam("height");
            string[] bbox = GetStringParam("bbox").Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
            if (bbox.Length != 4)
                throw new Exception("Param bbox has a wrong format. Correct format: xmin,ymin,xmax,ymax");

            try
            {
                left = double.Parse(bbox[0].Trim());
                bottom = double.Parse(bbox[1].Trim());
                right = double.Parse(bbox[2].Trim());
                top = double.Parse(bbox[3].Trim());
            }
            catch (Exception)
            {
                throw new Exception("Param bbox has a wrong format. All parameters should be double type");
            }
            if (request["crs"] != null && request["crs"].ToLower() == "epsg:4326")
            {
                double tmp;
                tmp = top;
                top = left;
                left = tmp;
                tmp = bottom;
                bottom = right;
                right = tmp;
            }
            if (request["request"] == null || (request["crs"] != null && request["crs"].ToLower() == "epsg:4326"))
            {
                double tmp;
                if (left > right)
                {
                    tmp = left;
                    left = right;
                    right = tmp;
                }
                if (top < bottom)
                {
                    tmp = top;
                    top = bottom;
                    bottom = tmp;
                }

                Projection.LatLon_to_bing_ext(ref left, ref top);
                Projection.LatLon_to_bing_ext(ref right, ref bottom);
            }
        }
    }
}