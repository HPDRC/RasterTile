using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;
using TileImagerySystem_CLR;
using System.IO;

namespace TileImageryReader
{
    public class Global : System.Web.HttpApplication
    {
        protected void Application_Start(object sender, EventArgs e)
        {
            MUtilities.InitConfig("LOG_PATH = " + Config.LOG_PATH_CPP);

            HttpRuntime.Cache["memorycache_blacktile_256"] = File.ReadAllBytes(Config.BLACK_256_TILE_PATH);
            HttpRuntime.Cache["memorycache_blacktile_400"] = File.ReadAllBytes(Config.BLACK_400_TILE_PATH);
            HttpRuntime.Cache["memorycache_blacktile_512"] = File.ReadAllBytes(Config.BLACK_512_TILE_PATH);
        }

        protected void Session_Start(object sender, EventArgs e)
        {

        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {

        }

        protected void Application_AuthenticateRequest(object sender, EventArgs e)
        {

        }

        protected void Application_Error(object sender, EventArgs e)
        {

        }

        protected void Session_End(object sender, EventArgs e)
        {

        }

        protected void Application_End(object sender, EventArgs e)
        {

        }
    }
}