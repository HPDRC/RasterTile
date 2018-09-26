using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TileImageryLoader
{
    static class Config
    {
        public static string LOADER_IMAGEVIEWER_SAVE_PATH = Helper.GetCurrentDir() + "image";

        public static string LOADER_CONFIG_PATH = Helper.GetCurrentDir() + "config.ini";

        public static string LOG_CPP_PATH = Helper.GetCurrentDir() + "log_cpp.log";

        public static string LOG_DOTNET_PATH = Helper.GetCurrentDir() + "log_dotnet.log";

        public static string LOADLIST_STAGE1_PATH = Helper.GetCurrentDir() + "loadlist_stage1.txt";

        public static string LOADLIST_STAGE2_PATH = Helper.GetCurrentDir() + "loadlist_stage2.txt";

        public static string LOADLIST_STAGE2_DATE_PATH = Helper.GetCurrentDir() + "loadlist_stage2_guessedDates.txt";

        public const int TDATA_OVERVIEW_MAX_SIZE = 6000 * 6000;

        public const int TDATA_OVERVIEW_EXTEND_LEVEL = 2;

        public const int TDATA_VOID_SUMMARY_MAX_SIZE = 1800 * 1800;

        public const int TDATA_VOID_SUMMARY_BASIC_SIZE = 4;

        public const int TDATA_DETAILEDVIEW_MAX_SIZE = 8000 * 8000;

        public const int TDATA_DETAILEDVIEW_MAX_WIDTH = 1800;
    }
}
