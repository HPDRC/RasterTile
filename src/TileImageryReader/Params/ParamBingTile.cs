using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public class ParamBingTile : Param
    {
        public MSTileXY tilexy;

        public int date = 0;
        public string source = "BEST_AVAILABLE";

        public ParamBingTile(HttpRequest request) : base(request)
        {
            tilexy.x = GetUIntParam("x");
            tilexy.y = GetUIntParam("y");
            tilexy.level = (byte)GetIntParam("z");

            // optional
            if (request["source"] != null)
                source = request["source"].ToUpper();
            if (request["date"] != null)
                date = GetIntParam("date");
        }

        public override string ToString()
        {
            return tilexy.ToString();
        }
    }
}
