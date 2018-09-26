using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TileImageryReader
{
    public partial class bing_mosaic_fixed : PageBase
    {
        private const int tileSize = 256;

        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                ParamBingMosaicFixed param = new ParamBingMosaicFixed(Request);
                try
                {
                    Reader reader = new Reader(param.debug);
                    reader.GetBingMosaicFixed(param);
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
                    Log("bing_mosaic_fixed", ex);
                }
            });
        }
    }
}