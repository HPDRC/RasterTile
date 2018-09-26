namespace TileImageryLoader
{
    partial class FormNav
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.cbViewValidGraph = new System.Windows.Forms.CheckBox();
            this.cbGuessedPixelValidRanges = new System.Windows.Forms.ComboBox();
            this.tbValidRange = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnVerifyAsBing = new System.Windows.Forms.Button();
            this.btnVerifyAsNASA = new System.Windows.Forms.Button();
            this.btnSaveToDisk = new System.Windows.Forms.Button();
            this.pbVerify = new System.Windows.Forms.PictureBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbUtmCorrectionZone = new System.Windows.Forms.TextBox();
            this.cbDistinguishPng = new System.Windows.Forms.CheckBox();
            this.cbDragActionType = new System.Windows.Forms.ComboBox();
            this.tbMasks = new System.Windows.Forms.TextBox();
            this.btnLoadMasks = new System.Windows.Forms.Button();
            this.btnDeleteLastMask = new System.Windows.Forms.Button();
            this.btnClearMasks = new System.Windows.Forms.Button();
            this.btnShowMasks = new System.Windows.Forms.Button();
            this.tbMsg = new System.Windows.Forms.TextBox();
            this.tbTileInfo = new System.Windows.Forms.TextBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.btnReadZoomOut = new System.Windows.Forms.Button();
            this.btnReadUp = new System.Windows.Forms.Button();
            this.btnReadDown = new System.Windows.Forms.Button();
            this.btnReadZoomIn = new System.Windows.Forms.Button();
            this.btnReadRight = new System.Windows.Forms.Button();
            this.btnReadLeft = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnOpenMetaFile = new System.Windows.Forms.Button();
            this.pbImg = new System.Windows.Forms.PictureBox();
            this.groupBox1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVerify)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbImg)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox4);
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.tbMsg);
            this.groupBox1.Controls.Add(this.tbTileInfo);
            this.groupBox1.Controls.Add(this.groupBox7);
            this.groupBox1.Controls.Add(this.groupBox6);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(532, 767);
            this.groupBox1.TabIndex = 40;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Navigate Panel";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.cbViewValidGraph);
            this.groupBox4.Controls.Add(this.cbGuessedPixelValidRanges);
            this.groupBox4.Controls.Add(this.tbValidRange);
            this.groupBox4.Location = new System.Drawing.Point(304, 325);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(222, 113);
            this.groupBox4.TabIndex = 49;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Find Valid Pixel Range";
            // 
            // cbViewValidGraph
            // 
            this.cbViewValidGraph.AutoSize = true;
            this.cbViewValidGraph.Location = new System.Drawing.Point(5, 72);
            this.cbViewValidGraph.Name = "cbViewValidGraph";
            this.cbViewValidGraph.Size = new System.Drawing.Size(107, 17);
            this.cbViewValidGraph.TabIndex = 40;
            this.cbViewValidGraph.Text = "View Valid Graph";
            this.cbViewValidGraph.UseVisualStyleBackColor = true;
            this.cbViewValidGraph.CheckedChanged += new System.EventHandler(this.cbViewValidGraph_CheckedChanged);
            // 
            // cbGuessedPixelValidRanges
            // 
            this.cbGuessedPixelValidRanges.AllowDrop = true;
            this.cbGuessedPixelValidRanges.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbGuessedPixelValidRanges.FormattingEnabled = true;
            this.cbGuessedPixelValidRanges.Location = new System.Drawing.Point(5, 19);
            this.cbGuessedPixelValidRanges.Name = "cbGuessedPixelValidRanges";
            this.cbGuessedPixelValidRanges.Size = new System.Drawing.Size(209, 21);
            this.cbGuessedPixelValidRanges.TabIndex = 39;
            this.cbGuessedPixelValidRanges.SelectedIndexChanged += new System.EventHandler(this.cbSelectValidPixelRange_SelectedIndexChanged);
            // 
            // tbValidRange
            // 
            this.tbValidRange.Location = new System.Drawing.Point(5, 46);
            this.tbValidRange.Name = "tbValidRange";
            this.tbValidRange.Size = new System.Drawing.Size(209, 20);
            this.tbValidRange.TabIndex = 37;
            this.tbValidRange.TextChanged += new System.EventHandler(this.tbValidRange_TextChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnVerifyAsBing);
            this.groupBox3.Controls.Add(this.btnVerifyAsNASA);
            this.groupBox3.Controls.Add(this.btnSaveToDisk);
            this.groupBox3.Controls.Add(this.pbVerify);
            this.groupBox3.Location = new System.Drawing.Point(6, 93);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(291, 353);
            this.groupBox3.TabIndex = 48;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Verify";
            // 
            // btnVerifyAsBing
            // 
            this.btnVerifyAsBing.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnVerifyAsBing.ForeColor = System.Drawing.Color.White;
            this.btnVerifyAsBing.Location = new System.Drawing.Point(6, 18);
            this.btnVerifyAsBing.Name = "btnVerifyAsBing";
            this.btnVerifyAsBing.Size = new System.Drawing.Size(88, 25);
            this.btnVerifyAsBing.TabIndex = 27;
            this.btnVerifyAsBing.Text = "Verify as Bing";
            this.btnVerifyAsBing.UseVisualStyleBackColor = false;
            this.btnVerifyAsBing.Click += new System.EventHandler(this.btnVerifyAsBing_Click);
            // 
            // btnVerifyAsNASA
            // 
            this.btnVerifyAsNASA.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnVerifyAsNASA.ForeColor = System.Drawing.Color.White;
            this.btnVerifyAsNASA.Location = new System.Drawing.Point(100, 18);
            this.btnVerifyAsNASA.Name = "btnVerifyAsNASA";
            this.btnVerifyAsNASA.Size = new System.Drawing.Size(88, 25);
            this.btnVerifyAsNASA.TabIndex = 31;
            this.btnVerifyAsNASA.Text = "Verify as NASA";
            this.btnVerifyAsNASA.UseVisualStyleBackColor = false;
            this.btnVerifyAsNASA.Click += new System.EventHandler(this.btnVerifyAsNASA_Click);
            // 
            // btnSaveToDisk
            // 
            this.btnSaveToDisk.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnSaveToDisk.ForeColor = System.Drawing.Color.White;
            this.btnSaveToDisk.Location = new System.Drawing.Point(194, 18);
            this.btnSaveToDisk.Name = "btnSaveToDisk";
            this.btnSaveToDisk.Size = new System.Drawing.Size(88, 25);
            this.btnSaveToDisk.TabIndex = 22;
            this.btnSaveToDisk.Text = "SaveToDisk";
            this.btnSaveToDisk.UseVisualStyleBackColor = false;
            this.btnSaveToDisk.Click += new System.EventHandler(this.btnSaveToDisk_Click);
            // 
            // pbVerify
            // 
            this.pbVerify.Location = new System.Drawing.Point(16, 63);
            this.pbVerify.Name = "pbVerify";
            this.pbVerify.Size = new System.Drawing.Size(256, 256);
            this.pbVerify.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbVerify.TabIndex = 42;
            this.pbVerify.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.tbUtmCorrectionZone);
            this.groupBox2.Controls.Add(this.cbDistinguishPng);
            this.groupBox2.Controls.Add(this.cbDragActionType);
            this.groupBox2.Controls.Add(this.tbMasks);
            this.groupBox2.Controls.Add(this.btnLoadMasks);
            this.groupBox2.Controls.Add(this.btnDeleteLastMask);
            this.groupBox2.Controls.Add(this.btnClearMasks);
            this.groupBox2.Controls.Add(this.btnShowMasks);
            this.groupBox2.Location = new System.Drawing.Point(303, 114);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(221, 205);
            this.groupBox2.TabIndex = 47;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Mask";
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.Navy;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(6, 157);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(128, 20);
            this.label2.TabIndex = 41;
            this.label2.Text = "UTM Correction Zone";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbUtmCorrectionZone
            // 
            this.tbUtmCorrectionZone.Location = new System.Drawing.Point(140, 157);
            this.tbUtmCorrectionZone.Name = "tbUtmCorrectionZone";
            this.tbUtmCorrectionZone.Size = new System.Drawing.Size(75, 20);
            this.tbUtmCorrectionZone.TabIndex = 40;
            // 
            // cbDistinguishPng
            // 
            this.cbDistinguishPng.AutoSize = true;
            this.cbDistinguishPng.Location = new System.Drawing.Point(9, 19);
            this.cbDistinguishPng.Name = "cbDistinguishPng";
            this.cbDistinguishPng.Size = new System.Drawing.Size(103, 17);
            this.cbDistinguishPng.TabIndex = 39;
            this.cbDistinguishPng.Text = "Distinguish PNG";
            this.cbDistinguishPng.UseVisualStyleBackColor = true;
            this.cbDistinguishPng.CheckedChanged += new System.EventHandler(this.cbDistinguishPng_CheckedChanged);
            // 
            // cbDragActionType
            // 
            this.cbDragActionType.AllowDrop = true;
            this.cbDragActionType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbDragActionType.FormattingEnabled = true;
            this.cbDragActionType.Items.AddRange(new object[] {
            "drag to see statistics",
            "drag to zoom in",
            "drag to add to mask",
            "drag to find PNG",
            "drag to guess pixel valid range"});
            this.cbDragActionType.Location = new System.Drawing.Point(6, 42);
            this.cbDragActionType.Name = "cbDragActionType";
            this.cbDragActionType.Size = new System.Drawing.Size(209, 21);
            this.cbDragActionType.TabIndex = 38;
            // 
            // tbMasks
            // 
            this.tbMasks.Location = new System.Drawing.Point(6, 131);
            this.tbMasks.Name = "tbMasks";
            this.tbMasks.Size = new System.Drawing.Size(209, 20);
            this.tbMasks.TabIndex = 36;
            this.tbMasks.Click += new System.EventHandler(this.tbMasks_Click);
            // 
            // btnLoadMasks
            // 
            this.btnLoadMasks.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnLoadMasks.ForeColor = System.Drawing.Color.White;
            this.btnLoadMasks.Location = new System.Drawing.Point(115, 100);
            this.btnLoadMasks.Name = "btnLoadMasks";
            this.btnLoadMasks.Size = new System.Drawing.Size(100, 25);
            this.btnLoadMasks.TabIndex = 35;
            this.btnLoadMasks.Text = "Load Masks";
            this.btnLoadMasks.UseVisualStyleBackColor = false;
            this.btnLoadMasks.Click += new System.EventHandler(this.btnLoadMasks_Click);
            // 
            // btnDeleteLastMask
            // 
            this.btnDeleteLastMask.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnDeleteLastMask.ForeColor = System.Drawing.Color.White;
            this.btnDeleteLastMask.Location = new System.Drawing.Point(9, 100);
            this.btnDeleteLastMask.Name = "btnDeleteLastMask";
            this.btnDeleteLastMask.Size = new System.Drawing.Size(100, 25);
            this.btnDeleteLastMask.TabIndex = 34;
            this.btnDeleteLastMask.Text = "Delete Last Mask";
            this.btnDeleteLastMask.UseVisualStyleBackColor = false;
            this.btnDeleteLastMask.Click += new System.EventHandler(this.btnDeleteLastMask_Click);
            // 
            // btnClearMasks
            // 
            this.btnClearMasks.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnClearMasks.ForeColor = System.Drawing.Color.White;
            this.btnClearMasks.Location = new System.Drawing.Point(115, 69);
            this.btnClearMasks.Name = "btnClearMasks";
            this.btnClearMasks.Size = new System.Drawing.Size(100, 25);
            this.btnClearMasks.TabIndex = 33;
            this.btnClearMasks.Text = "Clear Masks";
            this.btnClearMasks.UseVisualStyleBackColor = false;
            this.btnClearMasks.Click += new System.EventHandler(this.btnClearMasks_Click);
            // 
            // btnShowMasks
            // 
            this.btnShowMasks.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnShowMasks.ForeColor = System.Drawing.Color.White;
            this.btnShowMasks.Location = new System.Drawing.Point(9, 69);
            this.btnShowMasks.Name = "btnShowMasks";
            this.btnShowMasks.Size = new System.Drawing.Size(100, 25);
            this.btnShowMasks.TabIndex = 32;
            this.btnShowMasks.Text = "Show Masks";
            this.btnShowMasks.UseVisualStyleBackColor = false;
            this.btnShowMasks.Click += new System.EventHandler(this.btnShowMasks_Click);
            // 
            // tbMsg
            // 
            this.tbMsg.BackColor = System.Drawing.Color.White;
            this.tbMsg.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbMsg.Location = new System.Drawing.Point(6, 575);
            this.tbMsg.Multiline = true;
            this.tbMsg.Name = "tbMsg";
            this.tbMsg.ReadOnly = true;
            this.tbMsg.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbMsg.Size = new System.Drawing.Size(520, 186);
            this.tbMsg.TabIndex = 46;
            // 
            // tbTileInfo
            // 
            this.tbTileInfo.BackColor = System.Drawing.Color.White;
            this.tbTileInfo.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbTileInfo.Location = new System.Drawing.Point(6, 452);
            this.tbTileInfo.Multiline = true;
            this.tbTileInfo.Name = "tbTileInfo";
            this.tbTileInfo.ReadOnly = true;
            this.tbTileInfo.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbTileInfo.Size = new System.Drawing.Size(526, 117);
            this.tbTileInfo.TabIndex = 45;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.btnReadZoomOut);
            this.groupBox7.Controls.Add(this.btnReadUp);
            this.groupBox7.Controls.Add(this.btnReadDown);
            this.groupBox7.Controls.Add(this.btnReadZoomIn);
            this.groupBox7.Controls.Add(this.btnReadRight);
            this.groupBox7.Controls.Add(this.btnReadLeft);
            this.groupBox7.Location = new System.Drawing.Point(303, 18);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(221, 90);
            this.groupBox7.TabIndex = 43;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Navigation";
            // 
            // btnReadZoomOut
            // 
            this.btnReadZoomOut.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadZoomOut.ForeColor = System.Drawing.Color.White;
            this.btnReadZoomOut.Location = new System.Drawing.Point(28, 19);
            this.btnReadZoomOut.Name = "btnReadZoomOut";
            this.btnReadZoomOut.Size = new System.Drawing.Size(50, 25);
            this.btnReadZoomOut.TabIndex = 23;
            this.btnReadZoomOut.Text = "Out";
            this.btnReadZoomOut.UseVisualStyleBackColor = false;
            this.btnReadZoomOut.Click += new System.EventHandler(this.btnReadZoomOut_Click);
            // 
            // btnReadUp
            // 
            this.btnReadUp.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadUp.ForeColor = System.Drawing.Color.White;
            this.btnReadUp.Location = new System.Drawing.Point(84, 19);
            this.btnReadUp.Name = "btnReadUp";
            this.btnReadUp.Size = new System.Drawing.Size(50, 25);
            this.btnReadUp.TabIndex = 22;
            this.btnReadUp.Text = "↑";
            this.btnReadUp.UseVisualStyleBackColor = false;
            this.btnReadUp.Click += new System.EventHandler(this.btnReadUp_Click);
            // 
            // btnReadDown
            // 
            this.btnReadDown.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadDown.ForeColor = System.Drawing.Color.White;
            this.btnReadDown.Location = new System.Drawing.Point(84, 54);
            this.btnReadDown.Name = "btnReadDown";
            this.btnReadDown.Size = new System.Drawing.Size(50, 25);
            this.btnReadDown.TabIndex = 21;
            this.btnReadDown.Text = "↓";
            this.btnReadDown.UseVisualStyleBackColor = false;
            this.btnReadDown.Click += new System.EventHandler(this.btnReadDown_Click);
            // 
            // btnReadZoomIn
            // 
            this.btnReadZoomIn.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadZoomIn.ForeColor = System.Drawing.Color.White;
            this.btnReadZoomIn.Location = new System.Drawing.Point(140, 19);
            this.btnReadZoomIn.Name = "btnReadZoomIn";
            this.btnReadZoomIn.Size = new System.Drawing.Size(50, 25);
            this.btnReadZoomIn.TabIndex = 20;
            this.btnReadZoomIn.Text = "In";
            this.btnReadZoomIn.UseVisualStyleBackColor = false;
            this.btnReadZoomIn.Click += new System.EventHandler(this.btnReadZoomIn_Click);
            // 
            // btnReadRight
            // 
            this.btnReadRight.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadRight.ForeColor = System.Drawing.Color.White;
            this.btnReadRight.Location = new System.Drawing.Point(140, 54);
            this.btnReadRight.Name = "btnReadRight";
            this.btnReadRight.Size = new System.Drawing.Size(50, 25);
            this.btnReadRight.TabIndex = 19;
            this.btnReadRight.Text = "→";
            this.btnReadRight.UseVisualStyleBackColor = false;
            this.btnReadRight.Click += new System.EventHandler(this.btnReadRight_Click);
            // 
            // btnReadLeft
            // 
            this.btnReadLeft.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnReadLeft.ForeColor = System.Drawing.Color.White;
            this.btnReadLeft.Location = new System.Drawing.Point(28, 54);
            this.btnReadLeft.Name = "btnReadLeft";
            this.btnReadLeft.Size = new System.Drawing.Size(50, 25);
            this.btnReadLeft.TabIndex = 18;
            this.btnReadLeft.Text = "←";
            this.btnReadLeft.UseVisualStyleBackColor = false;
            this.btnReadLeft.Click += new System.EventHandler(this.btnReadLeft_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnOpenMetaFile);
            this.groupBox6.Location = new System.Drawing.Point(6, 19);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(291, 68);
            this.groupBox6.TabIndex = 42;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Open File";
            // 
            // btnOpenMetaFile
            // 
            this.btnOpenMetaFile.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnOpenMetaFile.ForeColor = System.Drawing.Color.White;
            this.btnOpenMetaFile.Location = new System.Drawing.Point(6, 19);
            this.btnOpenMetaFile.Name = "btnOpenMetaFile";
            this.btnOpenMetaFile.Size = new System.Drawing.Size(279, 40);
            this.btnOpenMetaFile.TabIndex = 24;
            this.btnOpenMetaFile.Text = "Open tmeta File";
            this.btnOpenMetaFile.UseVisualStyleBackColor = false;
            this.btnOpenMetaFile.Click += new System.EventHandler(this.btnOpenMetaFile_Click);
            // 
            // pbImg
            // 
            this.pbImg.BackColor = System.Drawing.SystemColors.Control;
            this.pbImg.Location = new System.Drawing.Point(550, 12);
            this.pbImg.Name = "pbImg";
            this.pbImg.Size = new System.Drawing.Size(768, 768);
            this.pbImg.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImg.TabIndex = 41;
            this.pbImg.TabStop = false;
            this.pbImg.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pbImg_MouseDown);
            this.pbImg.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pbImg_MouseMove);
            this.pbImg.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pbImg_MouseUp);
            // 
            // FormNav
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1324, 791);
            this.Controls.Add(this.pbImg);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "FormNav";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormNav";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pbVerify)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pbImg)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnVerifyAsBing;
        private System.Windows.Forms.Button btnVerifyAsNASA;
        private System.Windows.Forms.Button btnSaveToDisk;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.Button btnReadZoomOut;
        private System.Windows.Forms.Button btnReadUp;
        private System.Windows.Forms.Button btnReadDown;
        private System.Windows.Forms.Button btnReadZoomIn;
        private System.Windows.Forms.Button btnReadRight;
        private System.Windows.Forms.Button btnReadLeft;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnOpenMetaFile;
        private System.Windows.Forms.PictureBox pbImg;
        private System.Windows.Forms.PictureBox pbVerify;
        private System.Windows.Forms.TextBox tbTileInfo;
        private System.Windows.Forms.TextBox tbMsg;
        private System.Windows.Forms.Button btnShowMasks;
        private System.Windows.Forms.Button btnClearMasks;
        private System.Windows.Forms.Button btnDeleteLastMask;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbMasks;
        private System.Windows.Forms.Button btnLoadMasks;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox cbDragActionType;
        private System.Windows.Forms.CheckBox cbDistinguishPng;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox tbValidRange;
        private System.Windows.Forms.ComboBox cbGuessedPixelValidRanges;
        private System.Windows.Forms.CheckBox cbViewValidGraph;
        private System.Windows.Forms.TextBox tbUtmCorrectionZone;
        private System.Windows.Forms.Label label2;

    }
}