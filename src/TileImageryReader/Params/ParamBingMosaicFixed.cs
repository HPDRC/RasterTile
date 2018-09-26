using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TileImageryReader
{
    public class ParamBingMosaicFixed : Param
    {
        public int z, width, height, date = 0;
        public double top, left;
        public string source = "BEST_AVAILABLE";

        public ParamBingMosaicFixed(HttpRequest request)
            : base(request)
        {
            z = (int)GetUIntParam("z");
            width = GetIntParam("width");
            height = GetIntParam("height");
            top = Helper.DegreeToRadian(GetDoubleParam("top"));
            left = Helper.DegreeToRadian(GetDoubleParam("left"));

            // optional
            if (request["source"] != null)
                source = request["source"].ToUpper();
            if (request["date"] != null)
                date = GetIntParam("date");
        }
    }
}