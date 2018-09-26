using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TileImageryReader
{
    public partial class bing : PageBase
    {
        private const int tileSize = 256;

        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                ParamBingTile param = new ParamBingTile(Request);
                try
                {
                    Reader reader = new Reader(param.debug);
                    reader.GetBingTile(param);
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
                    Log("bing_tile", ex);
                }
            });
        }
    }
}