namespace TileImageryLoader
{
    partial class FormMain
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
            this.tbMsg = new System.Windows.Forms.TextBox();
            this.gbGenerateLoadList = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label14 = new System.Windows.Forms.Label();
            this.tbFixDoqqLineLength = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.tbFixDoqqHeaderLength = new System.Windows.Forms.TextBox();
            this.btnFixDoqqHeader = new System.Windows.Forms.Button();
            this.btnUtmCorrectFromOldIndex = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbAddHeaderSourceName = new System.Windows.Forms.TextBox();
            this.tbAddHeaderLoadlistId = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnAddHeader = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.tbAddHeaderOriginId = new System.Windows.Forms.TextBox();
            this.tbAddHeaderOutputDir = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tbAddHeaderSourceDate = new System.Windows.Forms.TextBox();
            this.gbLoadListUTM = new System.Windows.Forms.GroupBox();
            this.tbSrcProj = new System.Windows.Forms.TextBox();
            this.btnGetUtmFromSourceFile = new System.Windows.Forms.Button();
            this.label17 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.tbDestProj = new System.Windows.Forms.TextBox();
            this.gbScanDir = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbScanDirPathToScan = new System.Windows.Forms.TextBox();
            this.cbScanDirOnly1SubDir = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbScanDirPathEndsWith = new System.Windows.Forms.TextBox();
            this.btnScanDir = new System.Windows.Forms.Button();
            this.tbTips = new System.Windows.Forms.TextBox();
            this.gbMessage = new System.Windows.Forms.GroupBox();
            this.btnOpenNavigator = new System.Windows.Forms.Button();
            this.btnGenerateMultipleOverviews = new System.Windows.Forms.Button();
            this.btnGenerateOverview = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tpLoadList = new System.Windows.Forms.TabPage();
            this.tpLoad = new System.Windows.Forms.TabPage();
            this.gbStartLoad = new System.Windows.Forms.GroupBox();
            this.btnLoad = new System.Windows.Forms.Button();
            this.cbDestProjection = new System.Windows.Forms.ComboBox();
            this.cbSourceProjection = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.btnAbortLoad = new System.Windows.Forms.Button();
            this.tbLoaderStatus = new System.Windows.Forms.TextBox();
            this.tpView = new System.Windows.Forms.TabPage();
            this.tpBuildIndex = new System.Windows.Forms.TabPage();
            this.label10 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tbConnectionString = new System.Windows.Forms.TextBox();
            this.btnBuildIndexExpandDir = new System.Windows.Forms.Button();
            this.btnBuildIndexStart = new System.Windows.Forms.Button();
            this.tbBuildIndexDirs = new System.Windows.Forms.TextBox();
            this.gbGenerateLoadList.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.gbLoadListUTM.SuspendLayout();
            this.gbScanDir.SuspendLayout();
            this.gbMessage.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tpLoadList.SuspendLayout();
            this.tpLoad.SuspendLayout();
            this.gbStartLoad.SuspendLayout();
            this.tpView.SuspendLayout();
            this.tpBuildIndex.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbMsg
            // 
            this.tbMsg.BackColor = System.Drawing.Color.White;
            this.tbMsg.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbMsg.Location = new System.Drawing.Point(3, 36);
            this.tbMsg.Multiline = true;
            this.tbMsg.Name = "tbMsg";
            this.tbMsg.ReadOnly = true;
            this.tbMsg.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbMsg.Size = new System.Drawing.Size(969, 161);
            this.tbMsg.TabIndex = 1;
            // 
            // gbGenerateLoadList
            // 
            this.gbGenerateLoadList.Controls.Add(this.groupBox2);
            this.gbGenerateLoadList.Controls.Add(this.groupBox1);
            this.gbGenerateLoadList.Controls.Add(this.gbLoadListUTM);
            this.gbGenerateLoadList.Controls.Add(this.gbScanDir);
            this.gbGenerateLoadList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbGenerateLoadList.Location = new System.Drawing.Point(3, 3);
            this.gbGenerateLoadList.Name = "gbGenerateLoadList";
            this.gbGenerateLoadList.Size = new System.Drawing.Size(961, 491);
            this.gbGenerateLoadList.TabIndex = 2;
            this.gbGenerateLoadList.TabStop = false;
            this.gbGenerateLoadList.Text = "LoadList Creator";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label14);
            this.groupBox2.Controls.Add(this.tbFixDoqqLineLength);
            this.groupBox2.Controls.Add(this.label13);
            this.groupBox2.Controls.Add(this.tbFixDoqqHeaderLength);
            this.groupBox2.Controls.Add(this.btnFixDoqqHeader);
            this.groupBox2.Controls.Add(this.btnUtmCorrectFromOldIndex);
            this.groupBox2.Location = new System.Drawing.Point(552, 19);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(230, 306);
            this.groupBox2.TabIndex = 25;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Other Operations";
            // 
            // label14
            // 
            this.label14.BackColor = System.Drawing.Color.Navy;
            this.label14.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.ForeColor = System.Drawing.Color.White;
            this.label14.Location = new System.Drawing.Point(92, 129);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(80, 20);
            this.label14.TabIndex = 29;
            this.label14.Text = "Line Length";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbFixDoqqLineLength
            // 
            this.tbFixDoqqLineLength.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbFixDoqqLineLength.Location = new System.Drawing.Point(92, 152);
            this.tbFixDoqqLineLength.Name = "tbFixDoqqLineLength";
            this.tbFixDoqqLineLength.Size = new System.Drawing.Size(80, 21);
            this.tbFixDoqqLineLength.TabIndex = 28;
            // 
            // label13
            // 
            this.label13.BackColor = System.Drawing.Color.Navy;
            this.label13.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.ForeColor = System.Drawing.Color.White;
            this.label13.Location = new System.Drawing.Point(6, 129);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(80, 20);
            this.label13.TabIndex = 27;
            this.label13.Text = "Header Length";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbFixDoqqHeaderLength
            // 
            this.tbFixDoqqHeaderLength.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbFixDoqqHeaderLength.Location = new System.Drawing.Point(6, 152);
            this.tbFixDoqqHeaderLength.Name = "tbFixDoqqHeaderLength";
            this.tbFixDoqqHeaderLength.Size = new System.Drawing.Size(80, 21);
            this.tbFixDoqqHeaderLength.TabIndex = 26;
            // 
            // btnFixDoqqHeader
            // 
            this.btnFixDoqqHeader.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnFixDoqqHeader.ForeColor = System.Drawing.Color.White;
            this.btnFixDoqqHeader.Location = new System.Drawing.Point(6, 75);
            this.btnFixDoqqHeader.Name = "btnFixDoqqHeader";
            this.btnFixDoqqHeader.Size = new System.Drawing.Size(166, 50);
            this.btnFixDoqqHeader.TabIndex = 25;
            this.btnFixDoqqHeader.Text = "Fix end of doqq header (files from stage 1 loadlist)";
            this.btnFixDoqqHeader.UseVisualStyleBackColor = false;
            this.btnFixDoqqHeader.Click += new System.EventHandler(this.btnFixDoqqHeader_Click);
            // 
            // btnUtmCorrectFromOldIndex
            // 
            this.btnUtmCorrectFromOldIndex.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnUtmCorrectFromOldIndex.ForeColor = System.Drawing.Color.White;
            this.btnUtmCorrectFromOldIndex.Location = new System.Drawing.Point(6, 19);
            this.btnUtmCorrectFromOldIndex.Name = "btnUtmCorrectFromOldIndex";
            this.btnUtmCorrectFromOldIndex.Size = new System.Drawing.Size(166, 50);
            this.btnUtmCorrectFromOldIndex.TabIndex = 24;
            this.btnUtmCorrectFromOldIndex.Text = "Correct Utm LoadLists by C:\\dnsindex";
            this.btnUtmCorrectFromOldIndex.UseVisualStyleBackColor = false;
            this.btnUtmCorrectFromOldIndex.Click += new System.EventHandler(this.btnUtmCorrectFromOldIndex_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.tbAddHeaderSourceName);
            this.groupBox1.Controls.Add(this.tbAddHeaderLoadlistId);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.btnAddHeader);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.tbAddHeaderOriginId);
            this.groupBox1.Controls.Add(this.tbAddHeaderOutputDir);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.tbAddHeaderSourceDate);
            this.groupBox1.Location = new System.Drawing.Point(6, 207);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(540, 177);
            this.groupBox1.TabIndex = 20;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Stage 3. Add Header";
            // 
            // label5
            // 
            this.label5.BackColor = System.Drawing.Color.Navy;
            this.label5.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(6, 120);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(80, 20);
            this.label5.TabIndex = 24;
            this.label5.Text = "Source Name";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbAddHeaderSourceName
            // 
            this.tbAddHeaderSourceName.Location = new System.Drawing.Point(92, 120);
            this.tbAddHeaderSourceName.Name = "tbAddHeaderSourceName";
            this.tbAddHeaderSourceName.Size = new System.Drawing.Size(120, 20);
            this.tbAddHeaderSourceName.TabIndex = 23;
            // 
            // tbAddHeaderLoadlistId
            // 
            this.tbAddHeaderLoadlistId.Location = new System.Drawing.Point(91, 42);
            this.tbAddHeaderLoadlistId.Name = "tbAddHeaderLoadlistId";
            this.tbAddHeaderLoadlistId.Size = new System.Drawing.Size(120, 20);
            this.tbAddHeaderLoadlistId.TabIndex = 22;
            // 
            // label3
            // 
            this.label3.BackColor = System.Drawing.Color.Navy;
            this.label3.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(5, 42);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(80, 20);
            this.label3.TabIndex = 21;
            this.label3.Text = "Loadlist Id";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnAddHeader
            // 
            this.btnAddHeader.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnAddHeader.ForeColor = System.Drawing.Color.White;
            this.btnAddHeader.Location = new System.Drawing.Point(6, 146);
            this.btnAddHeader.Name = "btnAddHeader";
            this.btnAddHeader.Size = new System.Drawing.Size(80, 22);
            this.btnAddHeader.TabIndex = 20;
            this.btnAddHeader.Text = "Add Header";
            this.btnAddHeader.UseVisualStyleBackColor = false;
            this.btnAddHeader.Click += new System.EventHandler(this.btnAddHeader_Click);
            // 
            // label8
            // 
            this.label8.BackColor = System.Drawing.Color.Navy;
            this.label8.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.ForeColor = System.Drawing.Color.White;
            this.label8.Location = new System.Drawing.Point(5, 68);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(80, 20);
            this.label8.TabIndex = 17;
            this.label8.Text = "Source Date";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbAddHeaderOriginId
            // 
            this.tbAddHeaderOriginId.Location = new System.Drawing.Point(92, 94);
            this.tbAddHeaderOriginId.Name = "tbAddHeaderOriginId";
            this.tbAddHeaderOriginId.Size = new System.Drawing.Size(120, 20);
            this.tbAddHeaderOriginId.TabIndex = 19;
            this.tbAddHeaderOriginId.Enter += new System.EventHandler(this.tbOriginId_Enter);
            this.tbAddHeaderOriginId.Leave += new System.EventHandler(this.tbOriginId_Leave);
            // 
            // tbAddHeaderOutputDir
            // 
            this.tbAddHeaderOutputDir.Location = new System.Drawing.Point(91, 16);
            this.tbAddHeaderOutputDir.Name = "tbAddHeaderOutputDir";
            this.tbAddHeaderOutputDir.Size = new System.Drawing.Size(442, 20);
            this.tbAddHeaderOutputDir.TabIndex = 14;
            // 
            // label7
            // 
            this.label7.BackColor = System.Drawing.Color.Navy;
            this.label7.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.Color.White;
            this.label7.Location = new System.Drawing.Point(5, 16);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(80, 20);
            this.label7.TabIndex = 15;
            this.label7.Text = "Output Dir";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label9
            // 
            this.label9.BackColor = System.Drawing.Color.Navy;
            this.label9.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.ForeColor = System.Drawing.Color.White;
            this.label9.Location = new System.Drawing.Point(6, 94);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(80, 20);
            this.label9.TabIndex = 18;
            this.label9.Text = "Source Id";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbAddHeaderSourceDate
            // 
            this.tbAddHeaderSourceDate.Location = new System.Drawing.Point(91, 68);
            this.tbAddHeaderSourceDate.Name = "tbAddHeaderSourceDate";
            this.tbAddHeaderSourceDate.Size = new System.Drawing.Size(120, 20);
            this.tbAddHeaderSourceDate.TabIndex = 16;
            this.tbAddHeaderSourceDate.Enter += new System.EventHandler(this.tbSourceSetDate_Enter);
            this.tbAddHeaderSourceDate.Leave += new System.EventHandler(this.tbSourceSetDate_Leave);
            // 
            // gbLoadListUTM
            // 
            this.gbLoadListUTM.Controls.Add(this.tbSrcProj);
            this.gbLoadListUTM.Controls.Add(this.btnGetUtmFromSourceFile);
            this.gbLoadListUTM.Controls.Add(this.label17);
            this.gbLoadListUTM.Controls.Add(this.label15);
            this.gbLoadListUTM.Controls.Add(this.tbDestProj);
            this.gbLoadListUTM.Location = new System.Drawing.Point(6, 97);
            this.gbLoadListUTM.Name = "gbLoadListUTM";
            this.gbLoadListUTM.Size = new System.Drawing.Size(540, 104);
            this.gbLoadListUTM.TabIndex = 4;
            this.gbLoadListUTM.TabStop = false;
            this.gbLoadListUTM.Text = "Stage 2. Get Utm Info From Source Files";
            // 
            // tbSrcProj
            // 
            this.tbSrcProj.Location = new System.Drawing.Point(91, 18);
            this.tbSrcProj.Name = "tbSrcProj";
            this.tbSrcProj.Size = new System.Drawing.Size(442, 20);
            this.tbSrcProj.TabIndex = 22;
            this.tbSrcProj.Enter += new System.EventHandler(this.tbSrcProj_Enter);
            this.tbSrcProj.Leave += new System.EventHandler(this.tbSrcProj_Leave);
            // 
            // btnGetUtmFromSourceFile
            // 
            this.btnGetUtmFromSourceFile.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnGetUtmFromSourceFile.ForeColor = System.Drawing.Color.White;
            this.btnGetUtmFromSourceFile.Location = new System.Drawing.Point(5, 72);
            this.btnGetUtmFromSourceFile.Name = "btnGetUtmFromSourceFile";
            this.btnGetUtmFromSourceFile.Size = new System.Drawing.Size(80, 22);
            this.btnGetUtmFromSourceFile.TabIndex = 16;
            this.btnGetUtmFromSourceFile.Text = "Get Utm Info";
            this.btnGetUtmFromSourceFile.UseVisualStyleBackColor = false;
            this.btnGetUtmFromSourceFile.Click += new System.EventHandler(this.btnGetUtmFromSourceFile_Click);
            // 
            // label17
            // 
            this.label17.BackColor = System.Drawing.Color.Navy;
            this.label17.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.ForeColor = System.Drawing.Color.White;
            this.label17.Location = new System.Drawing.Point(5, 18);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(80, 20);
            this.label17.TabIndex = 21;
            this.label17.Text = "Source Proj";
            this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label15
            // 
            this.label15.BackColor = System.Drawing.Color.Navy;
            this.label15.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.ForeColor = System.Drawing.Color.White;
            this.label15.Location = new System.Drawing.Point(5, 45);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(80, 20);
            this.label15.TabIndex = 17;
            this.label15.Text = "Dest Proj";
            this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbDestProj
            // 
            this.tbDestProj.Location = new System.Drawing.Point(91, 45);
            this.tbDestProj.Name = "tbDestProj";
            this.tbDestProj.Size = new System.Drawing.Size(442, 20);
            this.tbDestProj.TabIndex = 18;
            this.tbDestProj.Text = "+proj=utm +zone=17 +south +datum=NAD83";
            // 
            // gbScanDir
            // 
            this.gbScanDir.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.gbScanDir.Controls.Add(this.label2);
            this.gbScanDir.Controls.Add(this.tbScanDirPathToScan);
            this.gbScanDir.Controls.Add(this.cbScanDirOnly1SubDir);
            this.gbScanDir.Controls.Add(this.label1);
            this.gbScanDir.Controls.Add(this.tbScanDirPathEndsWith);
            this.gbScanDir.Controls.Add(this.btnScanDir);
            this.gbScanDir.Location = new System.Drawing.Point(6, 19);
            this.gbScanDir.Name = "gbScanDir";
            this.gbScanDir.Size = new System.Drawing.Size(540, 72);
            this.gbScanDir.TabIndex = 16;
            this.gbScanDir.TabStop = false;
            this.gbScanDir.Text = "Stage 1. Scan Dir";
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.Navy;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(6, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 20);
            this.label2.TabIndex = 20;
            this.label2.Text = "Dir To Scan";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbScanDirPathToScan
            // 
            this.tbScanDirPathToScan.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbScanDirPathToScan.Location = new System.Drawing.Point(92, 16);
            this.tbScanDirPathToScan.Name = "tbScanDirPathToScan";
            this.tbScanDirPathToScan.Size = new System.Drawing.Size(337, 21);
            this.tbScanDirPathToScan.TabIndex = 19;
            this.tbScanDirPathToScan.Click += new System.EventHandler(this.tbScanDirPathToScan_Click);
            // 
            // cbScanDirOnly1SubDir
            // 
            this.cbScanDirOnly1SubDir.AutoSize = true;
            this.cbScanDirOnly1SubDir.BackColor = System.Drawing.Color.DarkBlue;
            this.cbScanDirOnly1SubDir.ForeColor = System.Drawing.Color.White;
            this.cbScanDirOnly1SubDir.Location = new System.Drawing.Point(435, 20);
            this.cbScanDirOnly1SubDir.Name = "cbScanDirOnly1SubDir";
            this.cbScanDirOnly1SubDir.Size = new System.Drawing.Size(98, 17);
            this.cbScanDirOnly1SubDir.TabIndex = 18;
            this.cbScanDirOnly1SubDir.Text = "Only *.1 SubDir";
            this.cbScanDirOnly1SubDir.UseVisualStyleBackColor = false;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Navy;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(6, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 20);
            this.label1.TabIndex = 17;
            this.label1.Text = "Path End With";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbScanDirPathEndsWith
            // 
            this.tbScanDirPathEndsWith.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbScanDirPathEndsWith.Location = new System.Drawing.Point(92, 42);
            this.tbScanDirPathEndsWith.Name = "tbScanDirPathEndsWith";
            this.tbScanDirPathEndsWith.Size = new System.Drawing.Size(102, 21);
            this.tbScanDirPathEndsWith.TabIndex = 16;
            // 
            // btnScanDir
            // 
            this.btnScanDir.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnScanDir.ForeColor = System.Drawing.Color.White;
            this.btnScanDir.Location = new System.Drawing.Point(218, 42);
            this.btnScanDir.Name = "btnScanDir";
            this.btnScanDir.Size = new System.Drawing.Size(80, 22);
            this.btnScanDir.TabIndex = 15;
            this.btnScanDir.Text = "Scan";
            this.btnScanDir.UseVisualStyleBackColor = false;
            this.btnScanDir.Click += new System.EventHandler(this.btnScanDir_Click);
            // 
            // tbTips
            // 
            this.tbTips.BackColor = System.Drawing.Color.White;
            this.tbTips.Dock = System.Windows.Forms.DockStyle.Top;
            this.tbTips.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbTips.ForeColor = System.Drawing.Color.Red;
            this.tbTips.Location = new System.Drawing.Point(3, 16);
            this.tbTips.Name = "tbTips";
            this.tbTips.ReadOnly = true;
            this.tbTips.Size = new System.Drawing.Size(969, 20);
            this.tbTips.TabIndex = 3;
            // 
            // gbMessage
            // 
            this.gbMessage.Controls.Add(this.tbMsg);
            this.gbMessage.Controls.Add(this.tbTips);
            this.gbMessage.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.gbMessage.Location = new System.Drawing.Point(0, 523);
            this.gbMessage.Name = "gbMessage";
            this.gbMessage.Size = new System.Drawing.Size(975, 200);
            this.gbMessage.TabIndex = 5;
            this.gbMessage.TabStop = false;
            this.gbMessage.Text = "Message";
            // 
            // btnOpenNavigator
            // 
            this.btnOpenNavigator.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnOpenNavigator.ForeColor = System.Drawing.Color.White;
            this.btnOpenNavigator.Location = new System.Drawing.Point(8, 6);
            this.btnOpenNavigator.Name = "btnOpenNavigator";
            this.btnOpenNavigator.Size = new System.Drawing.Size(160, 40);
            this.btnOpenNavigator.TabIndex = 37;
            this.btnOpenNavigator.Text = "Open Navigator";
            this.btnOpenNavigator.UseVisualStyleBackColor = false;
            this.btnOpenNavigator.Click += new System.EventHandler(this.btnOpenNavigator_Click);
            // 
            // btnGenerateMultipleOverviews
            // 
            this.btnGenerateMultipleOverviews.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnGenerateMultipleOverviews.ForeColor = System.Drawing.Color.White;
            this.btnGenerateMultipleOverviews.Location = new System.Drawing.Point(8, 98);
            this.btnGenerateMultipleOverviews.Name = "btnGenerateMultipleOverviews";
            this.btnGenerateMultipleOverviews.Size = new System.Drawing.Size(160, 40);
            this.btnGenerateMultipleOverviews.TabIndex = 26;
            this.btnGenerateMultipleOverviews.Text = "Generate Multiple Overviews";
            this.btnGenerateMultipleOverviews.UseVisualStyleBackColor = false;
            this.btnGenerateMultipleOverviews.Click += new System.EventHandler(this.btnGenerateMultipleOverviews_Click);
            // 
            // btnGenerateOverview
            // 
            this.btnGenerateOverview.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnGenerateOverview.ForeColor = System.Drawing.Color.White;
            this.btnGenerateOverview.Location = new System.Drawing.Point(8, 52);
            this.btnGenerateOverview.Name = "btnGenerateOverview";
            this.btnGenerateOverview.Size = new System.Drawing.Size(160, 40);
            this.btnGenerateOverview.TabIndex = 25;
            this.btnGenerateOverview.Text = "Generate Overview";
            this.btnGenerateOverview.UseVisualStyleBackColor = false;
            this.btnGenerateOverview.Click += new System.EventHandler(this.btnGenerateOverview_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tpLoadList);
            this.tabControl1.Controls.Add(this.tpLoad);
            this.tabControl1.Controls.Add(this.tpView);
            this.tabControl1.Controls.Add(this.tpBuildIndex);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(975, 523);
            this.tabControl1.TabIndex = 7;
            // 
            // tpLoadList
            // 
            this.tpLoadList.Controls.Add(this.gbGenerateLoadList);
            this.tpLoadList.Location = new System.Drawing.Point(4, 22);
            this.tpLoadList.Name = "tpLoadList";
            this.tpLoadList.Padding = new System.Windows.Forms.Padding(3);
            this.tpLoadList.Size = new System.Drawing.Size(967, 497);
            this.tpLoadList.TabIndex = 0;
            this.tpLoadList.Text = "LoadList";
            this.tpLoadList.UseVisualStyleBackColor = true;
            // 
            // tpLoad
            // 
            this.tpLoad.Controls.Add(this.gbStartLoad);
            this.tpLoad.Location = new System.Drawing.Point(4, 22);
            this.tpLoad.Name = "tpLoad";
            this.tpLoad.Size = new System.Drawing.Size(967, 497);
            this.tpLoad.TabIndex = 3;
            this.tpLoad.Text = "Load";
            this.tpLoad.UseVisualStyleBackColor = true;
            // 
            // gbStartLoad
            // 
            this.gbStartLoad.Controls.Add(this.btnLoad);
            this.gbStartLoad.Controls.Add(this.cbDestProjection);
            this.gbStartLoad.Controls.Add(this.cbSourceProjection);
            this.gbStartLoad.Controls.Add(this.label11);
            this.gbStartLoad.Controls.Add(this.label12);
            this.gbStartLoad.Controls.Add(this.btnAbortLoad);
            this.gbStartLoad.Controls.Add(this.tbLoaderStatus);
            this.gbStartLoad.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbStartLoad.Location = new System.Drawing.Point(0, 0);
            this.gbStartLoad.Name = "gbStartLoad";
            this.gbStartLoad.Size = new System.Drawing.Size(967, 497);
            this.gbStartLoad.TabIndex = 15;
            this.gbStartLoad.TabStop = false;
            this.gbStartLoad.Text = "Start to Load";
            // 
            // btnLoad
            // 
            this.btnLoad.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnLoad.ForeColor = System.Drawing.Color.White;
            this.btnLoad.Location = new System.Drawing.Point(11, 73);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(154, 30);
            this.btnLoad.TabIndex = 37;
            this.btnLoad.Text = "Load";
            this.btnLoad.UseVisualStyleBackColor = false;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // cbDestProjection
            // 
            this.cbDestProjection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbDestProjection.FormattingEnabled = true;
            this.cbDestProjection.Items.AddRange(new object[] {
            "UTM",
            "NASA",
            "BING"});
            this.cbDestProjection.Location = new System.Drawing.Point(85, 46);
            this.cbDestProjection.Name = "cbDestProjection";
            this.cbDestProjection.Size = new System.Drawing.Size(80, 21);
            this.cbDestProjection.TabIndex = 36;
            this.cbDestProjection.SelectedIndexChanged += new System.EventHandler(this.cbDestProjection_SelectedIndexChanged);
            // 
            // cbSourceProjection
            // 
            this.cbSourceProjection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSourceProjection.FormattingEnabled = true;
            this.cbSourceProjection.Items.AddRange(new object[] {
            "UTM",
            "NASA"});
            this.cbSourceProjection.Location = new System.Drawing.Point(85, 17);
            this.cbSourceProjection.Name = "cbSourceProjection";
            this.cbSourceProjection.Size = new System.Drawing.Size(80, 21);
            this.cbSourceProjection.TabIndex = 35;
            this.cbSourceProjection.SelectedIndexChanged += new System.EventHandler(this.cbSourceProjection_SelectedIndexChanged);
            // 
            // label11
            // 
            this.label11.BackColor = System.Drawing.Color.Navy;
            this.label11.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.ForeColor = System.Drawing.Color.White;
            this.label11.Location = new System.Drawing.Point(8, 19);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(70, 20);
            this.label11.TabIndex = 34;
            this.label11.Text = "Source";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label12
            // 
            this.label12.BackColor = System.Drawing.Color.Navy;
            this.label12.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.ForeColor = System.Drawing.Color.White;
            this.label12.Location = new System.Drawing.Point(8, 45);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(70, 20);
            this.label12.TabIndex = 33;
            this.label12.Text = "Destination";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnAbortLoad
            // 
            this.btnAbortLoad.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnAbortLoad.ForeColor = System.Drawing.Color.White;
            this.btnAbortLoad.Location = new System.Drawing.Point(11, 109);
            this.btnAbortLoad.Name = "btnAbortLoad";
            this.btnAbortLoad.Size = new System.Drawing.Size(154, 30);
            this.btnAbortLoad.TabIndex = 29;
            this.btnAbortLoad.Text = "Abort Load";
            this.btnAbortLoad.UseVisualStyleBackColor = false;
            this.btnAbortLoad.Click += new System.EventHandler(this.btnAbortLoad_Click);
            // 
            // tbLoaderStatus
            // 
            this.tbLoaderStatus.BackColor = System.Drawing.Color.White;
            this.tbLoaderStatus.Font = new System.Drawing.Font("Microsoft YaHei", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbLoaderStatus.Location = new System.Drawing.Point(171, 16);
            this.tbLoaderStatus.Multiline = true;
            this.tbLoaderStatus.Name = "tbLoaderStatus";
            this.tbLoaderStatus.ReadOnly = true;
            this.tbLoaderStatus.Size = new System.Drawing.Size(788, 475);
            this.tbLoaderStatus.TabIndex = 28;
            // 
            // tpView
            // 
            this.tpView.Controls.Add(this.btnOpenNavigator);
            this.tpView.Controls.Add(this.btnGenerateMultipleOverviews);
            this.tpView.Controls.Add(this.btnGenerateOverview);
            this.tpView.Location = new System.Drawing.Point(4, 22);
            this.tpView.Name = "tpView";
            this.tpView.Padding = new System.Windows.Forms.Padding(3);
            this.tpView.Size = new System.Drawing.Size(967, 497);
            this.tpView.TabIndex = 1;
            this.tpView.Text = "View";
            this.tpView.UseVisualStyleBackColor = true;
            // 
            // tpBuildIndex
            // 
            this.tpBuildIndex.Controls.Add(this.label10);
            this.tpBuildIndex.Controls.Add(this.label4);
            this.tpBuildIndex.Controls.Add(this.tbConnectionString);
            this.tpBuildIndex.Controls.Add(this.btnBuildIndexExpandDir);
            this.tpBuildIndex.Controls.Add(this.btnBuildIndexStart);
            this.tpBuildIndex.Controls.Add(this.tbBuildIndexDirs);
            this.tpBuildIndex.Location = new System.Drawing.Point(4, 22);
            this.tpBuildIndex.Name = "tpBuildIndex";
            this.tpBuildIndex.Size = new System.Drawing.Size(967, 497);
            this.tpBuildIndex.TabIndex = 2;
            this.tpBuildIndex.Text = "Build Index";
            this.tpBuildIndex.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.BackColor = System.Drawing.Color.Navy;
            this.label10.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.ForeColor = System.Drawing.Color.White;
            this.label10.Location = new System.Drawing.Point(8, 14);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(80, 20);
            this.label10.TabIndex = 22;
            this.label10.Text = "Conn String";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.BackColor = System.Drawing.Color.Navy;
            this.label4.Font = new System.Drawing.Font("Times New Roman", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(8, 40);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 20);
            this.label4.TabIndex = 21;
            this.label4.Text = "Data Dir";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbConnectionString
            // 
            this.tbConnectionString.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbConnectionString.Location = new System.Drawing.Point(94, 13);
            this.tbConnectionString.Name = "tbConnectionString";
            this.tbConnectionString.Size = new System.Drawing.Size(573, 21);
            this.tbConnectionString.TabIndex = 20;
            this.tbConnectionString.Text = "DRIVER={SQL Server};SERVER=(local);DATABASE=DBName;Trusted_Connection=Yes;";
            // 
            // btnBuildIndexExpandDir
            // 
            this.btnBuildIndexExpandDir.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnBuildIndexExpandDir.ForeColor = System.Drawing.Color.White;
            this.btnBuildIndexExpandDir.Location = new System.Drawing.Point(94, 216);
            this.btnBuildIndexExpandDir.Name = "btnBuildIndexExpandDir";
            this.btnBuildIndexExpandDir.Size = new System.Drawing.Size(100, 30);
            this.btnBuildIndexExpandDir.TabIndex = 19;
            this.btnBuildIndexExpandDir.Text = "Expand Dir";
            this.btnBuildIndexExpandDir.UseVisualStyleBackColor = false;
            this.btnBuildIndexExpandDir.Click += new System.EventHandler(this.btnBuildIndexExpandDir_Click);
            // 
            // btnBuildIndexStart
            // 
            this.btnBuildIndexStart.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.btnBuildIndexStart.ForeColor = System.Drawing.Color.White;
            this.btnBuildIndexStart.Location = new System.Drawing.Point(200, 216);
            this.btnBuildIndexStart.Name = "btnBuildIndexStart";
            this.btnBuildIndexStart.Size = new System.Drawing.Size(100, 30);
            this.btnBuildIndexStart.TabIndex = 18;
            this.btnBuildIndexStart.Text = "Build Index";
            this.btnBuildIndexStart.UseVisualStyleBackColor = false;
            this.btnBuildIndexStart.Click += new System.EventHandler(this.btnBuildIndexStart_Click);
            // 
            // tbBuildIndexDirs
            // 
            this.tbBuildIndexDirs.Location = new System.Drawing.Point(94, 40);
            this.tbBuildIndexDirs.Multiline = true;
            this.tbBuildIndexDirs.Name = "tbBuildIndexDirs";
            this.tbBuildIndexDirs.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbBuildIndexDirs.Size = new System.Drawing.Size(573, 170);
            this.tbBuildIndexDirs.TabIndex = 3;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(975, 723);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.gbMessage);
            this.MaximizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TileImageryLoader";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.gbGenerateLoadList.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.gbLoadListUTM.ResumeLayout(false);
            this.gbLoadListUTM.PerformLayout();
            this.gbScanDir.ResumeLayout(false);
            this.gbScanDir.PerformLayout();
            this.gbMessage.ResumeLayout(false);
            this.gbMessage.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tpLoadList.ResumeLayout(false);
            this.tpLoad.ResumeLayout(false);
            this.gbStartLoad.ResumeLayout(false);
            this.gbStartLoad.PerformLayout();
            this.tpView.ResumeLayout(false);
            this.tpBuildIndex.ResumeLayout(false);
            this.tpBuildIndex.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox tbMsg;
        private System.Windows.Forms.GroupBox gbGenerateLoadList;
        private System.Windows.Forms.TextBox tbTips;
        private System.Windows.Forms.GroupBox gbLoadListUTM;
        private System.Windows.Forms.GroupBox gbMessage;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tpLoadList;
        private System.Windows.Forms.TabPage tpView;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbAddHeaderOutputDir;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbAddHeaderSourceDate;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbAddHeaderOriginId;
        private System.Windows.Forms.TabPage tpBuildIndex;
        private System.Windows.Forms.TextBox tbBuildIndexDirs;
        private System.Windows.Forms.Button btnBuildIndexStart;
        private System.Windows.Forms.TabPage tpLoad;
        private System.Windows.Forms.GroupBox gbStartLoad;
        private System.Windows.Forms.Button btnAbortLoad;
        private System.Windows.Forms.TextBox tbLoaderStatus;
        private System.Windows.Forms.Button btnUtmCorrectFromOldIndex;
        private System.Windows.Forms.Button btnBuildIndexExpandDir;
        private System.Windows.Forms.Button btnScanDir;
        private System.Windows.Forms.GroupBox gbScanDir;
        private System.Windows.Forms.CheckBox cbScanDirOnly1SubDir;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbScanDirPathEndsWith;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnAddHeader;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbScanDirPathToScan;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnGetUtmFromSourceFile;
        private System.Windows.Forms.TextBox tbConnectionString;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox cbDestProjection;
        private System.Windows.Forms.ComboBox cbSourceProjection;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnFixDoqqHeader;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox tbFixDoqqHeaderLength;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox tbFixDoqqLineLength;
        private System.Windows.Forms.Button btnGenerateOverview;
        private System.Windows.Forms.Button btnGenerateMultipleOverviews;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbAddHeaderSourceName;
        private System.Windows.Forms.TextBox tbAddHeaderLoadlistId;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbSrcProj;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox tbDestProj;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button btnOpenNavigator;
    }
}

