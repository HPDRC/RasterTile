using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    /// <summary>
    /// manage multiple loadlists
    /// </summary>
    class LoadListCollection
    {
        private List<LoadList> loadLists = new List<LoadList>();

        public LoadListCollection(string[] loadListPathes)
        {
            loadLists.Clear();

            // first check all the config files
            Dictionary<string, int> outputPathList = new Dictionary<string, int>();
            foreach (string loadListPath in loadListPathes)
            {
                LoadList loadList = new LoadList(loadListPath);
                string outputDir = loadList.header.outputPath;

                // check if outputDir already used
                if (outputPathList.ContainsKey(outputDir))
                    throw new Exception("SourceSet " + loadListPath + " want to use outputDir " + outputDir + " which is already used by another SourceSet");
                outputPathList[outputDir] = 1;

                // create dir and ensure empty
                if (Directory.Exists(outputDir))
                {
                    if (DialogResult.Cancel == MessageBox.Show("OutputDir " + outputDir + " already exists, you need to either delete it or cancel the load.\r\nDelete " + outputDir + " ?", "Confirm Delete", MessageBoxButtons.OKCancel))
                        throw new Exception("Load cancelled because outputDir already exists");
                    else
                        Directory.Delete(outputDir, true);
                }
                Directory.CreateDirectory(outputDir);
                loadLists.Add(loadList);
                FormMain.ShowMessage("Prepared: " + loadListPath);
            }
        }

        /// <summary>
        /// load all loadlists
        /// </summary>
        public void Load(bool isUtmDest)
        {
            Helper.AsyncRun(delegate
            {
                foreach (LoadList loadList in loadLists)
                {
                    try
                    {
                        FormMain.ShowMessage("----- Load: Start to load " + loadList.header.outputPath + " -----");
                        FormMain.ShowMessage("Load: Start time " + DateTime.Now.ToString());

                        // restore config to default before each loading
                        MUtilities.ResetConfig();
                        MUtilities.InitConfig(File.ReadAllText(Config.LOADER_CONFIG_PATH));

                        loadList.LoadStart(isUtmDest);
                        FormMain.ShowMessage("Load: End time " + DateTime.Now.ToString());
                        FormMain.ShowMessage("----- Load: Finish " + loadList.header.outputPath + " -----");
                    }
                    catch (Exception ex)
                    {
                        FormMain.ShowMessage(ex);
                    }
                }
            });
        }

        public void Abort()
        {
            foreach (LoadList loadList in loadLists)
            {
                try
                {
                    loadList.LoadAbort();
                }
                catch (Exception) { }
            }
        }
    }
}
