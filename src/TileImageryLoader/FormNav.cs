using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    public partial class FormNav : Form
    {
        public FormNav()
        {
            InitializeComponent();
            pbImg.Image = new Bitmap(pbImg.Width, pbImg.Height);
            tbMsg.Text = "Warning: mask feature is only working correctly with bing projection tiles!\r\n";
            cbDragActionType.SelectedIndex = 0;
        }

        public static FormNav formNav;

        public static void UICall(MethodInvoker mi)
        {
            try
            {
                if (formNav.InvokeRequired)
                    formNav.Invoke(mi);
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
                if (formNav.tbMsg.Text.Length > 50000) formNav.tbMsg.Clear();
                formNav.tbMsg.AppendText(message + "\r\n");
                formNav.tbMsg.SelectionStart = formNav.tbMsg.Text.Length;
                formNav.tbMsg.ScrollToCaret();
            });
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

        #region open

        private TDataNavigator navigator = null;

        private void btnOpenMetaFile_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                OpenFileDialog dialog = new OpenFileDialog();
                dialog.Filter = "Meta file|*.tmeta";
                dialog.Multiselect = true;

                // if not selected, return
                DialogResult result = dialog.ShowDialog();
                if (result != DialogResult.OK)
                    return;

                navigator = new TDataNavigator(dialog.FileNames, pbImg, tbTileInfo, cbGuessedPixelValidRanges, tbUtmCorrectionZone);
            });
        }

        #endregion

        #region nav by direction
        
        private void btnReadZoomOut_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(0, 0, true, false); });
        }

        private void btnReadZoomIn_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(0, 0, false, true); });
        }

        private void btnReadLeft_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(-1, 0, false, false); });
        }

        private void btnReadRight_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(1, 0, false, false); });
        }

        private void btnReadUp_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(0, -1, false, false); });
        }

        private void btnReadDown_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.MoveTo(0, 1, false, false); });
        }

        #endregion

        #region verify

        private void btnSaveToDisk_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                Helper.SaveImageMultiColor(pbImg.Image, Config.LOADER_IMAGEVIEWER_SAVE_PATH + ".png");
                ShowMessage("Image saved to " + Config.LOADER_IMAGEVIEWER_SAVE_PATH + ".png");
            });
        }

        private void btnVerifyAsBing_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.VerifyCenterAsBing(pbVerify); });
        }

        private void btnVerifyAsNASA_Click(object sender, EventArgs e)
        {
            SafeRun(delegate { navigator.VerifyCenterAsNASA(pbVerify); });
        }

        #endregion

        #region masks

        private void pbImg_MouseDown(object sender, MouseEventArgs e)
        {
            SafeRun(delegate
            {
                if (e.Button == MouseButtons.Left)
                    navigator.OnMouseDown(e.Location);
            });
        }

        private void pbImg_MouseMove(object sender, MouseEventArgs e)
        {
            SafeRun(delegate
            {
                if (e.Button == MouseButtons.Left)
                    navigator.OnMouseMove(e.Location);
            });
        }

        private void pbImg_MouseUp(object sender, MouseEventArgs e)
        {
            SafeRun(delegate
            {
                try
                {
                    if (e.Button == MouseButtons.Left)
                        navigator.OnMouseUp(e.Location, cbDragActionType.SelectedIndex);
                    else
                        navigator.MoveTo(0, 0, true, false);
                }
                catch (Exception ex)
                {
                    ShowMessage(ex.Message);
                }
            });
        }

        private void btnShowMasks_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                ShowMessage(navigator.GetMasksDescription());
            });
        }

        private void btnClearMasks_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                navigator.masks.Clear();
                navigator.Paint();
            });
        }

        private void btnDeleteLastMask_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                navigator.masks.RemoveAt(navigator.masks.Count - 1);
                navigator.Paint();
            });
        }
        
        private void btnLoadMasks_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                navigator.LoadMasksFromString(tbMasks.Text);
                navigator.Paint();
            });
        }

        #endregion

        private void cbDistinguishPng_CheckedChanged(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                navigator.distinguishPng = cbDistinguishPng.Checked;
                navigator.Paint();
            });
        }

        private void tbMasks_Click(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                tbMasks.SelectAll();
            });
        }

        private void cbSelectValidPixelRange_SelectedIndexChanged(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                tbValidRange.Text = (string)cbGuessedPixelValidRanges.SelectedItem;
            });
        }

        private void cbViewValidGraph_CheckedChanged(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                if (cbViewValidGraph.Checked == false)
                    navigator.isShowValidGraph = false;
                else
                {
                    try
                    {
                        navigator.pixelValidRange = new PixelValidRange(tbValidRange.Text);
                        navigator.isShowValidGraph = true;
                    }
                    catch (Exception ex)
                    {
                        ShowMessage(ex.Message);
                        cbViewValidGraph.Checked = false;
                        navigator.isShowValidGraph = false;
                    }
                }
            });
        }

        private void tbValidRange_TextChanged(object sender, EventArgs e)
        {
            SafeRun(delegate
            {
                try
                {
                    if (cbViewValidGraph.Checked)
                    {
                        navigator.pixelValidRange = new PixelValidRange(tbValidRange.Text);
                        navigator.Paint();
                        ShowMessage("Now showing valid graph with range = " + tbValidRange.Text);
                    }
                }
                catch (Exception) { }
            });
        }
    }
}
