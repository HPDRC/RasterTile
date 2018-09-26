using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TileImageryLoader
{
    static class LoadListCreator
    {

        /*
         * // scan data files' paths
                List<string> dataFilePaths = new List<string>();

                // it's different whether to check only *.1 subdir
                if (!cbOnlyScan1SubDir.Checked)
                    Helper.GetAllFilesByExt(tbInputDirPath.Text, tbInputImgExt.Text, dataFilePaths, cbAnyExtName.Checked);
                else
                {
                    DirectoryInfo di = new DirectoryInfo(tbInputDirPath.Text);
                    foreach (DirectoryInfo subDir in di.GetDirectories())
                    {
                        if (subDir.Name.EndsWith(".1"))
                            Helper.GetAllFilesByExt(subDir.FullName, tbInputImgExt.Text, dataFilePaths, cbAnyExtName.Checked);
                    }
                }

                // check is corresponding meta file exists
                if (tbInputMetaExt.Enabled == true)
                    Helper.CheckMetaExists(dataFilePaths, tbInputImgExt.Text, tbInputMetaExt.Text);

                // show a message
                ShowMessage("ScanDir: " + dataFilePaths.Count + " files in total");

                // generate list
                currentSourceSet.GenerateFromFiles((ESourceFileType)cbImgType.SelectedIndex, dataFilePaths);

                // update loadlist into ui
                ShowSourceSetOnUI(currentSourceSet);*/

        

        #region utm

        public static void CorrectLoadListsFromOldIndex(string[] loadListPaths, string pathOfOldIndex)
        {
            Helper.AsyncRun(delegate
            {
                // first, load dnsindex
                FormMain.ShowMessage("--- Loading dnsindex ---");
                Dictionary<string, OldIndexRecord> oldRecords = new Dictionary<string, OldIndexRecord>();
                DirectoryInfo rootDir = new DirectoryInfo(pathOfOldIndex);
                foreach (DirectoryInfo subDir in rootDir.GetDirectories())
                {
                    foreach (DirectoryInfo lastDir in subDir.GetDirectories())
                    {
                        if (!lastDir.Name.EndsWith(".1")) continue;
                        foreach (FileInfo indexfile in lastDir.GetFiles())
                        {
                            foreach (string line in File.ReadAllLines(indexfile.FullName))
                            {
                                try
                                {
                                    OldIndexRecord rec = new OldIndexRecord(line);
                                    oldRecords[rec.path] = rec;
                                }
                                catch (Exception) { }
                            }
                        }
                    }
                }

                // then, correct each list
                foreach (string loadListPath in loadListPaths)
                {
                    FormMain.ShowMessage("--- Processing " + loadListPath + " ---");
                    LoadList ss = new LoadList(loadListPath);
                    ss.CorrectByOldIndex(oldRecords);
                    ss.Save(loadListPath);
                }
            });
        }

        #endregion
    }
}
