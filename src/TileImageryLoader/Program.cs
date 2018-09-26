using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace TileImageryLoader
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            FormMain.formMain = new FormMain();
            Application.Run(FormMain.formMain);
        }
    }
}
