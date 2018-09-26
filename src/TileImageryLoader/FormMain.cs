using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    public partial class FormMain : Form
    {
        #region static and global

        public static FormMain formMain;

        public static void UICall(MethodInvoker mi)
        {
            try
            {
                if (formMain.InvokeRequired)
                    formMain.Invoke(mi);
                else
                    mi.Invoke();
            }
            catch (Exception ex)
            {
                ShowMessage("FormMain.UICall: ");
                ShowMessage(ex.InnerException.Message);
            }
        }

        public static void ShowMessage(Exception ex)
        {
            ShowMessage(ex.Message + "\r\n" + ex.StackTrace);
        }

        public static void ShowMessage(string message)
        {
            UICall(delegate
            {
                if (formMain.tbMsg.Text.Length > 50000) formMain.tbMsg.Clear();
                formMain.tbMsg.AppendText(message + "\r\n");
                formMain.tbMsg.SelectionStart = formMain.tbMsg.Text.Length;
                formMain.tbMsg.ScrollToCaret();
                try
                {
                    File.AppendAllText(Config.LOG_DOTNET_PATH, message + "\r\n");
                }
                catch (Exception) { }
            });
        }

        public static void ShowLoaderStatus(string message)
        {
            UICall(delegate { formMain.tbLoaderStatus.Text = message; });
        }

        private void SafeRun(MethodInvoker mi)
        {
            try
            {
                mi.Invoke();
            }
            catch (Exception ex)
            {
                ShowMessage(ex);
            }
        }

        #endregion
        
        #region intialize

        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                if (File.Exists(Config.LOADER_CONFIG_PATH))
                {
                    MUtilities.InitConfig(File.ReadAllText(Config.LOADER_CONFIG_PATH));
                    MUtilities.InitConfig("LOG_PATH = " + Config.LOG_CPP_PATH);
                }
                else
                    ShowMessage("Warning: Config file is not found. Default config is used.");
                cbSourceProjection.SelectedIndex = cbDestProjection.SelectedIndex = 0;
            });
        }

        #endregion

        #region tips

        private void ShowTips(string tips)
        {
            tbTips.Text = tips;
        }

        private void ClearTips()
        {
            tbTips.Clear();
        }

        private void tbSourceSetDate_Enter(object sender, EventArgs e)
        {
            ShowTips("Input \"Year/Month/Day\", like \"2012/12/31\". In one load, all images must have the same date.");
        }

        private void tbSourceSetDate_Leave(object sender, EventArgs e)
        {
            ClearTips();
        }

        private void tbOriginId_Enter(object sender, EventArgs e)
        {
            ShowTips("Indicate all images in this load belongs to which origin");
        }

        private void tbOriginId_Leave(object sender, EventArgs e)
        {
            ClearTips();
        }
        
        private void tbSrcProj_Enter(object sender, EventArgs e)
        {
            ShowTips("Leave blank if you don't want to convert projection.");
        }

        private void tbSrcProj_Leave(object sender, EventArgs e)
        {
            ClearTips();
        }
        
        #endregion

        #region tab LoadList

        private void btnScanDir_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                LoadDraft draft = new LoadDraft(tbScanDirPathToScan.Text.Trim(), tbScanDirPathEndsWith.Text.Trim(), cbScanDirOnly1SubDir.Checked);
                draft.Save(Config.LOADLIST_STAGE1_PATH);
                ShowMessage("Stage 1: result saved to " + Config.LOADLIST_STAGE1_PATH);
            });
        }

        private void btnGetUtmFromSourceFile_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                LoadDraft draft = new LoadDraft(Config.LOADLIST_STAGE1_PATH);
                List<string> listRecordStrings = new List<string>();
                List<string> guessedDates = new List<string>();
                LoadListRecord listRecord = new LoadListRecord();
                foreach (LoadDraftRecord draftRecord in draft.records)
                {
                    try
                    {
                        listRecord.InitByUtmSourceFile(draftRecord.filePath, draftRecord.fileType, tbSrcProj.Text.Trim(), tbDestProj.Text.Trim());
                        guessedDates.Add(listRecord.guessedDate);
                        listRecordStrings.Add(listRecord.ToString());
                    }
                    catch (Exception)
                    {
                        ShowMessage("failed to get meta info from " + draftRecord.filePath);
                        ShowMessage("rename to " + draftRecord.filePath + ".damaged");
                        File.Move(draftRecord.filePath, draftRecord.filePath + ".damaged");
                    }
                }
                File.WriteAllLines(Config.LOADLIST_STAGE2_PATH, listRecordStrings);
                guessedDates.Sort();
                File.WriteAllLines(Config.LOADLIST_STAGE2_DATE_PATH, guessedDates);
                ShowMessage("Stage 2: result saved to " + Config.LOADLIST_STAGE2_PATH);
                ShowMessage("Stage 2: guessed source dates saved to " + Config.LOADLIST_STAGE2_DATE_PATH);
            });
        }

        private void btnAddHeader_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                // generate loadlist's save path
                string loadlistSavePath = Helper.GetCurrentDir() + tbAddHeaderLoadlistId.Text + ".txt";

                // generate dataset output path
                string outputPath = tbAddHeaderOutputDir.Text.Trim();
                if (!outputPath.EndsWith("\\"))
                    outputPath += "\\";
                outputPath += tbAddHeaderLoadlistId.Text + "_" + Int32.Parse(tbAddHeaderOriginId.Text).ToString("D3") + "_" + tbAddHeaderSourceName.Text + "_" + tbAddHeaderSourceDate.Text.Replace("/", "_");
                ShowMessage("Stage 3: outputpath = " + outputPath);

                // generate header
                LoadListHeader header = new LoadListHeader(outputPath, tbAddHeaderSourceDate.Text, tbAddHeaderOriginId.Text);
                string result = header.ToString() + "\r\n" + File.ReadAllText(Config.LOADLIST_STAGE2_PATH);
                File.WriteAllText(loadlistSavePath, result);
                ShowMessage("Stage 3: loadlist saved to " + loadlistSavePath);
            });
        }

        private void btnFixDoqqHeader_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                int headerLength = Int32.Parse(tbFixDoqqHeaderLength.Text);
                int lineLength = Int32.Parse(tbFixDoqqLineLength.Text);
                if (headerLength == 0 || lineLength == 0)
                    throw new Exception("Error: neither header length nor line length can be 0");

                LoadDraft draft = new LoadDraft(Config.LOADLIST_STAGE1_PATH);
                foreach (LoadDraftRecord rec in draft.records)
                    rec.FixDoqqHeader(headerLength, lineLength);

                ShowMessage("Fix success!");
            });
        }

        private void tbScanDirPathToScan_Click(object sender, EventArgs e)
        {
            tbScanDirPathToScan.SelectAll();
        }

        private void btnUtmCorrectFromOldIndex_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                OpenFileDialog dialog = new OpenFileDialog();
                dialog.Multiselect = true;
                DialogResult result = dialog.ShowDialog();
                if (result == DialogResult.OK)
                    LoadListCreator.CorrectLoadListsFromOldIndex(dialog.FileNames, "C:\\dnsindex");
            });
        }

        #endregion

        #region tab Load

        private void SetLoadProjection()
        {
            SafeRun(delegate
            {
                if (cbSourceProjection.Text == "" || cbDestProjection.Text == "")
                    return;
                if (cbDestProjection.Text == "UTM" && cbSourceProjection.Text != "UTM")
                    ShowMessage("Error: when dest is UTM, source MUST also be UTM!");
                MUtilities.InitConfig("SOURCE_PROJECTION = " + cbSourceProjection.Text +
                    "\nDEST_PROJECTION = " + cbDestProjection.Text);
                ShowMessage("Load: " + cbSourceProjection.Text + " -> " + cbDestProjection.Text);
            });
        }

        private void cbSourceProjection_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetLoadProjection();
        }

        private void cbDestProjection_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetLoadProjection();
        }

        LoadListCollection loadingLoadListCollection;

        private void DoLoad(bool isUtmDest)
        {
            SafeRun(delegate
            {
                // show dialog
                OpenFileDialog dialog = new OpenFileDialog();
                dialog.Multiselect = true;
                dialog.Filter = "Text files|*.txt";

                // if not selected, return
                DialogResult result = dialog.ShowDialog();
                if (result != DialogResult.OK)
                    return;

                // build source set list and load
                loadingLoadListCollection = new LoadListCollection(dialog.FileNames);
                loadingLoadListCollection.Load(isUtmDest);
            });
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            DoLoad(cbDestProjection.Text == "UTM");
        }

        private void btnAbortLoad_Click(object sender, EventArgs e)
        {
            ShowMessage("Load: abort");
            if (loadingLoadListCollection != null)
                loadingLoadListCollection.Abort();
        }
        
        #endregion

        #region tab View

        private void GenerateOverviews(DirectoryInfo dir)
        {
            // get all tmeta file paths
            List<string> tmetaPaths = new List<string>();
            Helper.GetAllFilesEndsWith(dir.FullName, tmetaPaths, ".tmeta");
            if (tmetaPaths.Count == 0)
            {
                ShowMessage("No data found in " + dir.Name);
                return;
            }
            TDataOverviewMaker nav = new TDataOverviewMaker(tmetaPaths.ToArray());
            string prefix = Helper.GetCurrentDir() + dir.Name;
            nav.GenerateOverview(prefix + "_overview.png");
            nav.GenerateVoidSummary(prefix + "_voidsummary.jpg");
            nav.GenerateDetailedView(prefix + "_detailedview1.png", prefix + "_detailedview2.png", prefix + "_statistics.txt");
        }

        private void btnGenerateOverview_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                FolderBrowserDialog dialog = new FolderBrowserDialog();

                // if not selected, return
                DialogResult result = dialog.ShowDialog();
                if (result != DialogResult.OK)
                    return;

                GenerateOverviews(new DirectoryInfo(dialog.SelectedPath));
            });
        }

        private void btnGenerateMultipleOverviews_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                FolderBrowserDialog dialog = new FolderBrowserDialog();

                // if not selected, return
                DialogResult result = dialog.ShowDialog();
                if (result != DialogResult.OK)
                    return;

                // generate an overview for each sub dir
                foreach (DirectoryInfo subDir in (new DirectoryInfo(dialog.SelectedPath)).GetDirectories())
                    GenerateOverviews(subDir);
            });
        }

        private void btnOpenNavigator_Click(object sender, EventArgs e)
        {
            FormNav.formNav = new FormNav();
            FormNav.formNav.ShowDialog();
        }

        #endregion

        #region tab Build Index

        private void btnBuildIndexExpandDir_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                string[] pathList = Helper.GetLines(tbBuildIndexDirs.Text);
                if (pathList.Length == 0)
                    return;
                DirectoryInfo di = new DirectoryInfo(pathList[0].Trim());
                string result = "";
                List<string> resultList = new List<string>();
                foreach (DirectoryInfo subDir in di.GetDirectories())
                    resultList.Add(subDir.FullName);
                resultList.Sort();
                foreach (string path in resultList)
                    result += path + "\r\n";
                tbBuildIndexDirs.Text = result;
            });
        }

        private void btnBuildIndexStart_Click(object sender, EventArgs e)
        {
            Helper.AsyncRun(delegate
            {
                int recordTotal = 0;
                foreach (string path in Helper.GetLines(tbBuildIndexDirs.Text))
                {
                    FormMain.ShowMessage("BuildIndex: Start " + path);
                    recordTotal += MUtilities.BuildIndex(path, tbConnectionString.Text);
                    FormMain.ShowMessage("BuildIndex: End " + path);
                }
                FormMain.ShowMessage("Total records inserted: " + recordTotal + ", check the database to verify the number");
            });
        }

        #endregion

    }
}
