using System;
using System.Collections.Generic;
using System.Web;
using System.Configuration;

namespace TileImageryReader
{
    public static class Config
    {
        public static string BLACK_256_TILE_PATH = HttpRuntime.AppDomainAppPath + "\\Resources\\black_single_tile_256.jpg";
        public static string BLACK_400_TILE_PATH = HttpRuntime.AppDomainAppPath + "\\Resources\\black_single_tile_400.jpg";
        public static string BLACK_512_TILE_PATH = HttpRuntime.AppDomainAppPath + "\\Resources\\black_single_tile_512.jpg";
        public static string LOG_PATH_CSHARP = ConfigurationManager.AppSettings["log_path_csharp"];
        public static string LOG_PATH_CPP = ConfigurationManager.AppSettings["log_path_cpp"];
        public static string CONN_UTM = ConfigurationManager.ConnectionStrings["connUtm"].ConnectionString;
        public static string CONN_BING = ConfigurationManager.ConnectionStrings["connBing"].ConnectionString;

        public const int CLIENT_CACHE_MINUTES = 20;
        public const int CLIENT_CACHE_BLACK_TILE_MINUTES = 5;

        public static string URL_FMAP_SERVICE = "http://131.94.133.180/scripts/fmap-2006.exe?targetimagetype=png&";
        public static string URL_MAPNIK_SERVICE = "http://terranode-246.cs.fiu.edu/scripts/demo_rendering_engine/rendering_engine.py?TargetImageType=png&style=BT&shadow=1&type=HYBRID&projType=utm&";
        //public static string URL_FMAP_SERVICE = "http://terranode-177.cs.fiu.edu/cache_mapnik/Default.aspx?TargetImageType=png&style=BT&shadow=1&type=HYBRID&projType=utm&";
    }
}