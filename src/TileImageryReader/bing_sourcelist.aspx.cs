using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TileImageryReader
{
    public partial class bing_sourcelist : PageBase
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                try
                {
                    ParamBingSourceList param = new ParamBingSourceList(Request);
                    Reader reader = new Reader(param.debug);
                    reader.GetBingSourceList(param);
                    if (!param.debug)
                        OutputJson(reader.resultAsString);
                    else
                        OutputText(reader.logString);
                }
                catch (Exception ex)
                {
                    if (!(Request["debug"] != null && Request["debug"].ToLower() == "true"))
                        OutputJson("{\"success\":false, \"error_message\":\"internal error: " + ex.Message + "\", \"sources\":[]}");
                    else
                        OutputText(ex.Message);
                    Log("bing_sourcelist", ex);
                }
            });
        }
    }
}