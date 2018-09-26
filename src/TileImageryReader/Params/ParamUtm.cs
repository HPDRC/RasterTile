using System;
using System.Collections.Generic;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public enum ERequestMode
    {
        SingleTile,
        MultiTiles,
        OriginList,
        TimeSeriesList,
    }

    public enum ELayerType
    {
        FMap,
        Mapnik,
    }

    public struct SUtmXY
    {
        public double x, y, res;
    }

    public class ParamUtm : Param
    {
        /// <summary>
        /// required
        /// </summary>
        public ERequestMode mode;

        /// <summary>
        /// required
        /// </summary>
        public int zone;

        /// <summary>
        /// required
        /// </summary>
        public double res;

        /// <summary>
        /// optional
        /// </summary>
        public ELayerType layerType = ELayerType.FMap;

        /// <summary>
        /// optional
        /// </summary>
        public string originName = "BEST_AVAILABLE";

        /// <summary>
        /// optional
        /// </summary>
        public int jpegQuality = 50;

        public uint xn, yn;
        public MSTileXY tilexy;
        public SUtmXY utmxy1, utmxy2, utmxy;
        public int date;

        /// <summary>
        /// optional, can be null
        /// </summary>
        public string overlay;

        public ParamUtm(HttpRequest request) : base(request)
        {
            // parse style
            if (request["style"] == null)
                throw new Exception("Param: style is missing");
            string modeStr = request["style"].ToLower();
            if (modeStr.StartsWith("bt"))
                mode = ERequestMode.SingleTile;
            else if (modeStr.StartsWith("dt"))
                mode = ERequestMode.MultiTiles;
            else if (modeStr.StartsWith("so"))
                mode = ERequestMode.OriginList;
            else if (modeStr.StartsWith("ts"))
                mode = ERequestMode.TimeSeriesList;
            else
                throw new Exception("Param: unknown style: " + modeStr + " request:" + request.Url.Query);

            // parse layerType
            if (request["LayerType"] != null)
            {
                string layerTypeString = request["LayerType"].ToLower();
                if (layerTypeString == "fmap")
                    layerType = ELayerType.FMap;
                else if (layerTypeString == "mapnik")
                    layerType = ELayerType.Mapnik;
                else
                    throw new Exception("Param: unknown LayerType: " + layerTypeString + " request:" + request.Url.Query);
            }

            ////////////// parse common params used by all modes //////////////
            // required
            zone = GetIntParam("utmz1");
            res = GetDoubleParam("res");

            // optional
            if (request["source"] != null)
                originName = request["source"].ToUpper();
            if (request["jcq"] != null)
                jpegQuality = GetIntParam("jcq");

            // optional
            overlay = request["overlay"];
            if (overlay == null)
                overlay = request["composite"];

            ////////////// parse different params for different modes //////////////
            // xn and yn are for SingleTile mode
            if (mode == ERequestMode.SingleTile)
            {
                tilexy.x = GetUIntParam("xn");
                xn = tilexy.x;
                tilexy.y = GetUIntParam("yn");
                yn = tilexy.y;
                tilexy.y = ProjectionUtm.Transform_YnOld_YnNew(tilexy.y, res);
                tilexy.level = ProjectionUtm.ResToLevel(res);
            }

            // x1 y1 x2 y2 are not for single tile request
            if (mode != ERequestMode.SingleTile)
            {
                utmxy1.x = GetDoubleParam("x1");
                utmxy1.y = GetDoubleParam("y1");
                utmxy1.res = res;
                utmxy2.x = GetDoubleParam("x2");
                utmxy2.y = GetDoubleParam("y2");
                utmxy2.res = res;
                utmxy = utmxy1;
            }

            date = 0;
            if (request["QFSOURCE_IMAGE_DATE"] != null)
                Int32.TryParse(request["QFSOURCE_IMAGE_DATE"], out date);
        }

        public override string ToString()
        {
            return this.originName + "," + this.zone + "," + this.tilexy + "," + base.ToString();
        }
    }
}