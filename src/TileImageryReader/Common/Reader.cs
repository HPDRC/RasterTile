using System;
using System.Collections.Generic;
using System.Web;
using System.IO;
using System.Configuration;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public class Reader
    {
        #region members

        public byte[] resultAsBytes;

        public string resultAsString;

        public string logString;

        public bool debugMode;

        public Reader(bool debugMode)
        {
            this.debugMode = debugMode;
        }

        #endregion

        #region bing

        public void GetBingTile(ParamBingTile param)
        {
            MReader reader = new MReader(Config.CONN_BING, param.source, debugMode, 0, param.date, 50);
            resultAsBytes = reader.GetTile(param.tilexy, false);
            logString = reader.logString;
        }

        public void GetBingTileExist(ParamBingTile param)
        {
            MReader reader = new MReader(Config.CONN_BING, param.source, debugMode, 0, param.date, 50);
            resultAsString = reader.GetTileExist(param.tilexy) ? "1" : "0";
            logString = reader.logString;
        }

        public void GetBingSourceList(ParamBingSourceList param)
        {
            MReader reader = new MReader(Config.CONN_BING, "BEST_AVAILABLE", debugMode, 0, 0, 50);
            resultAsString = reader.GetBingSourceListJson(param.z, param.top, param.right, param.bottom, param.left);
            logString = reader.logString;
        }

        public void GetBingMosaicFixed(ParamBingMosaicFixed param)
        {
            MReader reader = new MReader(Config.CONN_BING, param.source, debugMode, 0, param.date, 50);
            resultAsBytes = reader.GetBingMosaic(param.z, param.width, param.height, param.top, param.left);
            logString = reader.logString;
        }

        public void GetBingMosaicAny(ParamBingMosaicAny param)
        {
            MReader reader = new MReader(Config.CONN_BING, param.source, debugMode, 0, param.date, 50);
            resultAsBytes = reader.GetBingMosaic(param.width, param.height, param.top, param.right, param.bottom, param.left);
            logString = reader.logString;
        }

        #endregion

        #region utm

        public void GetUtmTile(ParamUtm param)
        {
            MReader reader = new MReader(Config.CONN_UTM, param.originName, debugMode, param.zone, param.date, param.jpegQuality);
            resultAsBytes = reader.GetTile(param.tilexy, false);
            logString = reader.logString;
        }

        public void GetUtmMosaic(ParamUtm param)
        {
            MReader reader = new MReader(Config.CONN_UTM, param.originName, debugMode, param.zone, param.date, param.jpegQuality);
            resultAsBytes = reader.GetUtmMosaic(param.utmxy1.x, param.utmxy1.y, param.utmxy2.x, param.utmxy2.y, param.res);
            logString = reader.logString;
        }

        public void GetUtmSourceList(double utmx, double utmy, int zone, double res)
        {
            MReader reader = new MReader(Config.CONN_UTM, "", debugMode, zone, 0, 0);
            resultAsString = reader.GetUtmSourceListXml(utmx, utmy, res);
            logString = reader.logString;
        }

        public byte[] GetUtmOverlay(ParamUtm param)
        {
            string url = (param.layerType == ELayerType.FMap ? Config.URL_FMAP_SERVICE : Config.URL_MAPNIK_SERVICE)
                + "Composite=" + param.overlay.Replace(" ", "+")
                + "&res=" + param.res + "&UTMZ1=" + param.zone + "&UTMZ2=" + param.zone;
            if (param.mode == ERequestMode.SingleTile)
                url += "&XN=" + param.xn + "&YN=" + param.yn;
            else if (param.mode == ERequestMode.MultiTiles)
                url += "&X1=" + param.utmxy1.x + "&Y1=" + param.utmxy1.y + "&X2=" + param.utmxy2.x + "&Y2=" + param.utmxy2.y;
            else
                throw new Exception("param.mode not correct");
            try
            {
                return Helper.HttpGetBytes(url);
            }
            catch (Exception ex)
            {
                throw new Exception("GetUtmOverlay error: " + ex.Message + " Requesting url: " + url);
            }
        }

        #endregion

        #region wms
        
        public void GetWmsMosaic(ParamWmsGetMap param)
        {
            MReader reader = new MReader(Config.CONN_BING, "BEST_AVAILABLE", debugMode, 0, 0, 50);
            resultAsBytes = reader.GetBingMosaic(param.width, param.height, param.top, param.right, param.bottom, param.left);
            logString = reader.logString;
        }

        #endregion
    }
}