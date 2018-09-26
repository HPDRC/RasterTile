using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TileImageryReader
{
    public class ParamBingMosaicAny : Param
    {
        public int date = 0;
        public string source = "BEST_AVAILABLE";
        public double top, right, bottom, left;
        public int width, height;

        public ParamBingMosaicAny(HttpRequest request)
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

            // optional
            if (request["source"] != null)
                source = request["source"].ToUpper();
            if (request["date"] != null)
                date = GetIntParam("date");
        }
    }
}