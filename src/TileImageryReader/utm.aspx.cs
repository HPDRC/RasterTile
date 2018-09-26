using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TileImageryReader
{
    public partial class utm : PageBase
    {
        private const int tileSize = 400;

        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                ParamUtm param = new ParamUtm(Request);
                Reader reader = new Reader(param.debug);
                switch (param.mode)
                {
                    case ERequestMode.SingleTile:
                        try
                        {
                            reader.GetUtmTile(param);
                            if (!param.debug)
                                OutputImage(reader.resultAsBytes, tileSize);
                            else
                                OutputText(reader.logString);
                        }
                        catch (Exception ex)
                        {
                            if (!param.debug)
                                OutputBlackTile(tileSize);
                            else
                                OutputText(ex.Message);
                            Log("GetUtmSingleTile", ex);
                        }
                        break;
                    case ERequestMode.MultiTiles:
                        try
                        {
                            reader.GetUtmMosaic(param);
                            if (!param.debug)
                            {
                                if (reader.resultAsBytes.Length == 0)
                                    OutputText("Requested image is too large.");
                                else
                                {
                                    if (param.overlay != null)
                                    {
                                        try
                                        {
                                            reader.resultAsBytes = Helper.OverlayImages(reader.resultAsBytes, reader.GetUtmOverlay(param), param.jpegQuality);
                                        }
                                        catch (Exception ex)
                                        {
                                            Log("GetUtmOverlay/OverlayImages", ex);
                                        }
                                    }
                                    OutputImage(reader.resultAsBytes, tileSize);
                                }
                            }
                            else
                                OutputText(reader.logString);
                        }
                        catch (Exception ex)
                        {
                            if (!param.debug)
                                OutputBlackTile(tileSize);
                            else
                                OutputText(ex.Message);
                            Log("utm_mosaic", ex);
                        }
                        break;
                    case ERequestMode.OriginList:
                        reader.GetUtmSourceList(param.utmxy.x, param.utmxy.y, param.zone, 0);
                        OutputXml(reader.resultAsString);
                        break;
                    case ERequestMode.TimeSeriesList:
                        reader.GetUtmSourceList(param.utmxy.x, param.utmxy.y, param.zone, param.res);
                        OutputXml(reader.resultAsString);
                        break;
                }
            });
        }
    }
}