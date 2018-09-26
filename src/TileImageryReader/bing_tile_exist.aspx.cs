using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TileImageryReader
{
    public partial class bing_tile_exist : PageBase
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                ParamBingTile param = new ParamBingTile(Request);
                try
                {
                    Reader reader = new Reader(param.debug);
                    reader.GetBingTileExist(param);
                    if (!param.debug)
                        OutputText(reader.resultAsString);
                    else
                        OutputText(reader.logString);
                }
                catch (Exception ex)
                {
                    if (!param.debug)
                        OutputText("0");
                    else
                        OutputText(ex.Message);
                    Log("bing_tile_exist", ex);
                }
            });
        }
    }
}