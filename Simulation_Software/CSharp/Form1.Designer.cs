using FB_Example.Properties;
using System.Drawing;

namespace my_interface
{
    partial class Form1
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
            this.UploadDSPBinary = new System.Windows.Forms.Button();
            this.StopDSP = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.rawSerial = new System.Windows.Forms.TextBox();
            this.dspSerial = new System.Windows.Forms.TextBox();
            this.btnDeviceOK = new System.Windows.Forms.Button();
            this.tBSamplingFreq = new System.Windows.Forms.TextBox();
            this.tBFilteringFreq = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tBIntStimInt = new System.Windows.Forms.TextBox();
            this.tBInterSpikePeriod = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.pBLengthInQueue = new System.Windows.Forms.ProgressBar();
            this.pBLengthOutQueue = new System.Windows.Forms.ProgressBar();
            this.label1 = new System.Windows.Forms.Label();
            this.tBNumStim = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.cB_SCU1_HS1 = new System.Windows.Forms.ComboBox();
            this.cB_SCU2_HS1 = new System.Windows.Forms.ComboBox();
            this.cB_SCU2_HS2 = new System.Windows.Forms.ComboBox();
            this.cB_SCU1_HS2 = new System.Windows.Forms.ComboBox();
            this.cB_SCU2_HS4 = new System.Windows.Forms.ComboBox();
            this.cB_SCU1_HS4 = new System.Windows.Forms.ComboBox();
            this.cB_SCU2_HS3 = new System.Windows.Forms.ComboBox();
            this.cB_SCU1_HS3 = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            this.cB_SCU1 = new System.Windows.Forms.CheckBox();
            this.cB_SCU2 = new System.Windows.Forms.CheckBox();
            this.bSTScript = new System.Windows.Forms.Button();
            this.tBAuthKey = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.tBSPort = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.tBTPort = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.btnScriptRunning = new System.Windows.Forms.Button();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.pBLengthBufferQueue = new System.Windows.Forms.ProgressBar();
            this.label21 = new System.Windows.Forms.Label();
            this.tBNumPorts = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.rBRandom = new System.Windows.Forms.RadioButton();
            this.rBSorted = new System.Windows.Forms.RadioButton();
            this.bStartStim = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.tBVpp = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.tBStimLength = new System.Windows.Forms.TextBox();
            this.cBMultiPatternStim = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // UploadDSPBinary
            // 
            this.UploadDSPBinary.Location = new System.Drawing.Point(12, 320);
            this.UploadDSPBinary.Name = "UploadDSPBinary";
            this.UploadDSPBinary.Size = new System.Drawing.Size(216, 46);
            this.UploadDSPBinary.TabIndex = 1;
            this.UploadDSPBinary.Text = "Upload DSP Binary";
            this.UploadDSPBinary.UseVisualStyleBackColor = true;
            this.UploadDSPBinary.Click += new System.EventHandler(this.UploadDSPBinary_Click);
            // 
            // StopDSP
            // 
            this.StopDSP.Enabled = false;
            this.StopDSP.Location = new System.Drawing.Point(236, 320);
            this.StopDSP.Name = "StopDSP";
            this.StopDSP.Size = new System.Drawing.Size(216, 46);
            this.StopDSP.TabIndex = 2;
            this.StopDSP.Text = "Stop DSP";
            this.StopDSP.UseVisualStyleBackColor = true;
            this.StopDSP.Click += new System.EventHandler(this.StopDSP_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(10, 11);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(95, 13);
            this.label7.TabIndex = 16;
            this.label7.Text = "Raw Port (USB-A):";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(10, 37);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(95, 13);
            this.label8.TabIndex = 17;
            this.label8.Text = "DSP Port (USB-B):";
            // 
            // rawSerial
            // 
            this.rawSerial.Location = new System.Drawing.Point(111, 8);
            this.rawSerial.Name = "rawSerial";
            this.rawSerial.Size = new System.Drawing.Size(76, 20);
            this.rawSerial.TabIndex = 18;
            // 
            // dspSerial
            // 
            this.dspSerial.Location = new System.Drawing.Point(111, 34);
            this.dspSerial.Name = "dspSerial";
            this.dspSerial.Size = new System.Drawing.Size(76, 20);
            this.dspSerial.TabIndex = 19;
            // 
            // btnDeviceOK
            // 
            this.btnDeviceOK.Location = new System.Drawing.Point(193, 7);
            this.btnDeviceOK.Name = "btnDeviceOK";
            this.btnDeviceOK.Size = new System.Drawing.Size(42, 48);
            this.btnDeviceOK.TabIndex = 20;
            this.btnDeviceOK.UseVisualStyleBackColor = true;
            // 
            // tBSamplingFreq
            // 
            this.tBSamplingFreq.Location = new System.Drawing.Point(112, 64);
            this.tBSamplingFreq.Name = "tBSamplingFreq";
            this.tBSamplingFreq.Size = new System.Drawing.Size(24, 20);
            this.tBSamplingFreq.TabIndex = 26;
            this.tBSamplingFreq.Text = "25";
            // 
            // tBFilteringFreq
            // 
            this.tBFilteringFreq.Location = new System.Drawing.Point(254, 64);
            this.tBFilteringFreq.Name = "tBFilteringFreq";
            this.tBFilteringFreq.Size = new System.Drawing.Size(33, 20);
            this.tBFilteringFreq.TabIndex = 27;
            this.tBFilteringFreq.Text = "300";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(100, 13);
            this.label3.TabIndex = 28;
            this.label3.Text = "Samping rate [kHz]:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(154, 67);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(98, 13);
            this.label4.TabIndex = 29;
            this.label4.Text = "Filt. frequency [Hz]:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(305, 67);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(111, 13);
            this.label5.TabIndex = 30;
            this.label5.Text = "Inter stim interval [ms]:";
            // 
            // tBIntStimInt
            // 
            this.tBIntStimInt.Location = new System.Drawing.Point(418, 64);
            this.tBIntStimInt.Name = "tBIntStimInt";
            this.tBIntStimInt.Size = new System.Drawing.Size(33, 20);
            this.tBIntStimInt.TabIndex = 31;
            this.tBIntStimInt.Text = "250";
            // 
            // tBInterSpikePeriod
            // 
            this.tBInterSpikePeriod.Location = new System.Drawing.Point(407, 36);
            this.tBInterSpikePeriod.Name = "tBInterSpikePeriod";
            this.tBInterSpikePeriod.Size = new System.Drawing.Size(44, 20);
            this.tBInterSpikePeriod.TabIndex = 33;
            this.tBInterSpikePeriod.Text = "5000";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(301, 39);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(104, 13);
            this.label6.TabIndex = 34;
            this.label6.Text = "Inter stim period [us]:";
            // 
            // pBLengthInQueue
            // 
            this.pBLengthInQueue.Location = new System.Drawing.Point(13, 409);
            this.pBLengthInQueue.Name = "pBLengthInQueue";
            this.pBLengthInQueue.Size = new System.Drawing.Size(142, 10);
            this.pBLengthInQueue.TabIndex = 36;
            // 
            // pBLengthOutQueue
            // 
            this.pBLengthOutQueue.Location = new System.Drawing.Point(310, 409);
            this.pBLengthOutQueue.Maximum = 50;
            this.pBLengthOutQueue.Name = "pBLengthOutQueue";
            this.pBLengthOutQueue.Size = new System.Drawing.Size(142, 10);
            this.pBLengthOutQueue.TabIndex = 37;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(284, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(144, 13);
            this.label1.TabIndex = 38;
            this.label1.Text = "Number of stimuli per pattern:";
            // 
            // tBNumStim
            // 
            this.tBNumStim.Location = new System.Drawing.Point(430, 8);
            this.tBNumStim.Name = "tBNumStim";
            this.tBNumStim.Size = new System.Drawing.Size(21, 20);
            this.tBNumStim.TabIndex = 39;
            this.tBNumStim.Text = "5";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 128);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 13);
            this.label2.TabIndex = 40;
            this.label2.Text = "SCU 1:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(10, 153);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 13);
            this.label9.TabIndex = 41;
            this.label9.Text = "SCU 2:";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(107, 106);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(31, 13);
            this.label10.TabIndex = 42;
            this.label10.Text = "HS 1";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(200, 106);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(31, 13);
            this.label11.TabIndex = 43;
            this.label11.Text = "HS 2";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(295, 106);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(31, 13);
            this.label12.TabIndex = 44;
            this.label12.Text = "HS 3";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(389, 106);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(31, 13);
            this.label13.TabIndex = 45;
            this.label13.Text = "HS 4";
            // 
            // cB_SCU1_HS1
            // 
            this.cB_SCU1_HS1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU1_HS1.Enabled = false;
            this.cB_SCU1_HS1.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU1_HS1.FormattingEnabled = true;
            this.cB_SCU1_HS1.Location = new System.Drawing.Point(73, 125);
            this.cB_SCU1_HS1.Name = "cB_SCU1_HS1";
            this.cB_SCU1_HS1.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU1_HS1.TabIndex = 46;
            // 
            // cB_SCU2_HS1
            // 
            this.cB_SCU2_HS1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU2_HS1.Enabled = false;
            this.cB_SCU2_HS1.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU2_HS1.FormattingEnabled = true;
            this.cB_SCU2_HS1.Location = new System.Drawing.Point(73, 150);
            this.cB_SCU2_HS1.Name = "cB_SCU2_HS1";
            this.cB_SCU2_HS1.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU2_HS1.TabIndex = 47;
            // 
            // cB_SCU2_HS2
            // 
            this.cB_SCU2_HS2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU2_HS2.Enabled = false;
            this.cB_SCU2_HS2.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU2_HS2.FormattingEnabled = true;
            this.cB_SCU2_HS2.Location = new System.Drawing.Point(168, 150);
            this.cB_SCU2_HS2.Name = "cB_SCU2_HS2";
            this.cB_SCU2_HS2.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU2_HS2.TabIndex = 49;
            // 
            // cB_SCU1_HS2
            // 
            this.cB_SCU1_HS2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU1_HS2.Enabled = false;
            this.cB_SCU1_HS2.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU1_HS2.FormattingEnabled = true;
            this.cB_SCU1_HS2.Location = new System.Drawing.Point(168, 125);
            this.cB_SCU1_HS2.Name = "cB_SCU1_HS2";
            this.cB_SCU1_HS2.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU1_HS2.TabIndex = 48;
            // 
            // cB_SCU2_HS4
            // 
            this.cB_SCU2_HS4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU2_HS4.Enabled = false;
            this.cB_SCU2_HS4.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU2_HS4.FormattingEnabled = true;
            this.cB_SCU2_HS4.Location = new System.Drawing.Point(358, 150);
            this.cB_SCU2_HS4.Name = "cB_SCU2_HS4";
            this.cB_SCU2_HS4.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU2_HS4.TabIndex = 53;
            // 
            // cB_SCU1_HS4
            // 
            this.cB_SCU1_HS4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU1_HS4.Enabled = false;
            this.cB_SCU1_HS4.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU1_HS4.FormattingEnabled = true;
            this.cB_SCU1_HS4.Location = new System.Drawing.Point(358, 125);
            this.cB_SCU1_HS4.Name = "cB_SCU1_HS4";
            this.cB_SCU1_HS4.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU1_HS4.TabIndex = 52;
            // 
            // cB_SCU2_HS3
            // 
            this.cB_SCU2_HS3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU2_HS3.Enabled = false;
            this.cB_SCU2_HS3.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU2_HS3.FormattingEnabled = true;
            this.cB_SCU2_HS3.Location = new System.Drawing.Point(263, 150);
            this.cB_SCU2_HS3.Name = "cB_SCU2_HS3";
            this.cB_SCU2_HS3.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU2_HS3.TabIndex = 51;
            // 
            // cB_SCU1_HS3
            // 
            this.cB_SCU1_HS3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cB_SCU1_HS3.Enabled = false;
            this.cB_SCU1_HS3.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cB_SCU1_HS3.FormattingEnabled = true;
            this.cB_SCU1_HS3.Location = new System.Drawing.Point(263, 125);
            this.cB_SCU1_HS3.Name = "cB_SCU1_HS3";
            this.cB_SCU1_HS3.Size = new System.Drawing.Size(93, 19);
            this.cB_SCU1_HS3.TabIndex = 50;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(47, 106);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(26, 13);
            this.label14.TabIndex = 54;
            this.label14.Text = "Use";
            // 
            // cB_SCU1
            // 
            this.cB_SCU1.AutoSize = true;
            this.cB_SCU1.Location = new System.Drawing.Point(52, 127);
            this.cB_SCU1.Name = "cB_SCU1";
            this.cB_SCU1.Size = new System.Drawing.Size(15, 14);
            this.cB_SCU1.TabIndex = 55;
            this.cB_SCU1.UseVisualStyleBackColor = true;
            this.cB_SCU1.CheckedChanged += new System.EventHandler(this.cB_SCU1_CheckedChanged);
            // 
            // cB_SCU2
            // 
            this.cB_SCU2.AutoSize = true;
            this.cB_SCU2.Location = new System.Drawing.Point(52, 152);
            this.cB_SCU2.Name = "cB_SCU2";
            this.cB_SCU2.Size = new System.Drawing.Size(15, 14);
            this.cB_SCU2.TabIndex = 56;
            this.cB_SCU2.UseVisualStyleBackColor = true;
            this.cB_SCU2.CheckedChanged += new System.EventHandler(this.cB_SCU2_CheckedChanged);
            // 
            // bSTScript
            // 
            this.bSTScript.Location = new System.Drawing.Point(285, 241);
            this.bSTScript.Name = "bSTScript";
            this.bSTScript.Size = new System.Drawing.Size(120, 50);
            this.bSTScript.TabIndex = 57;
            this.bSTScript.Text = "Start Transform Script";
            this.bSTScript.UseVisualStyleBackColor = true;
            this.bSTScript.Click += new System.EventHandler(this.button1_Click);
            // 
            // tBAuthKey
            // 
            this.tBAuthKey.Location = new System.Drawing.Point(178, 269);
            this.tBAuthKey.Name = "tBAuthKey";
            this.tBAuthKey.Size = new System.Drawing.Size(101, 20);
            this.tBAuthKey.TabIndex = 59;
            this.tBAuthKey.Text = "lbb_secret";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(119, 272);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(53, 13);
            this.label15.TabIndex = 58;
            this.label15.Text = "Auth Key:";
            // 
            // tBSPort
            // 
            this.tBSPort.Location = new System.Drawing.Point(77, 269);
            this.tBSPort.Name = "tBSPort";
            this.tBSPort.Size = new System.Drawing.Size(36, 20);
            this.tBSPort.TabIndex = 61;
            this.tBSPort.Text = "8750";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(12, 272);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(62, 13);
            this.label16.TabIndex = 60;
            this.label16.Text = "Server port:";
            // 
            // tBTPort
            // 
            this.tBTPort.Location = new System.Drawing.Point(113, 242);
            this.tBTPort.Name = "tBTPort";
            this.tBTPort.Size = new System.Drawing.Size(36, 20);
            this.tBTPort.TabIndex = 63;
            this.tBTPort.Text = "8701";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(9, 245);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(101, 13);
            this.label17.TabIndex = 62;
            this.label17.Text = "Transformation port:";
            // 
            // btnScriptRunning
            // 
            this.btnScriptRunning.BackColor = System.Drawing.Color.Red;
            this.btnScriptRunning.Location = new System.Drawing.Point(410, 241);
            this.btnScriptRunning.Name = "btnScriptRunning";
            this.btnScriptRunning.Size = new System.Drawing.Size(42, 50);
            this.btnScriptRunning.TabIndex = 64;
            this.btnScriptRunning.UseVisualStyleBackColor = false;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(42, 394);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(87, 13);
            this.label18.TabIndex = 65;
            this.label18.Text = "Command queue";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(349, 394);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(63, 13);
            this.label19.TabIndex = 66;
            this.label19.Text = "Data queue";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(203, 394);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(68, 13);
            this.label20.TabIndex = 68;
            this.label20.Text = "Buffer queue";
            // 
            // pBLengthBufferQueue
            // 
            this.pBLengthBufferQueue.Location = new System.Drawing.Point(161, 409);
            this.pBLengthBufferQueue.Maximum = 50;
            this.pBLengthBufferQueue.Name = "pBLengthBufferQueue";
            this.pBLengthBufferQueue.Size = new System.Drawing.Size(142, 10);
            this.pBLengthBufferQueue.TabIndex = 67;
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(167, 245);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(85, 13);
            this.label21.TabIndex = 69;
            this.label21.Text = "Number of ports:";
            // 
            // tBNumPorts
            // 
            this.tBNumPorts.Location = new System.Drawing.Point(254, 242);
            this.tBNumPorts.Name = "tBNumPorts";
            this.tBNumPorts.Size = new System.Drawing.Size(25, 20);
            this.tBNumPorts.TabIndex = 70;
            this.tBNumPorts.Text = "8";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(9, 190);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(99, 13);
            this.label22.TabIndex = 71;
            this.label22.Text = "Random stimulation";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(145, 190);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(90, 13);
            this.label23.TabIndex = 72;
            this.label23.Text = "Sorted stimulation";
            // 
            // rBRandom
            // 
            this.rBRandom.AutoSize = true;
            this.rBRandom.Checked = true;
            this.rBRandom.Location = new System.Drawing.Point(110, 190);
            this.rBRandom.Name = "rBRandom";
            this.rBRandom.Size = new System.Drawing.Size(14, 13);
            this.rBRandom.TabIndex = 73;
            this.rBRandom.TabStop = true;
            this.rBRandom.UseVisualStyleBackColor = true;
            // 
            // rBSorted
            // 
            this.rBSorted.AutoSize = true;
            this.rBSorted.Location = new System.Drawing.Point(130, 190);
            this.rBSorted.Name = "rBSorted";
            this.rBSorted.Size = new System.Drawing.Size(14, 13);
            this.rBSorted.TabIndex = 74;
            this.rBSorted.TabStop = true;
            this.rBSorted.UseVisualStyleBackColor = true;
            // 
            // bStartStim
            // 
            this.bStartStim.Location = new System.Drawing.Point(236, 441);
            this.bStartStim.Name = "bStartStim";
            this.bStartStim.Size = new System.Drawing.Size(216, 46);
            this.bStartStim.TabIndex = 75;
            this.bStartStim.Text = "Send 8 command";
            this.bStartStim.UseVisualStyleBackColor = true;
            this.bStartStim.Click += new System.EventHandler(this.bStartStim_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(15, 441);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(36, 20);
            this.textBox1.TabIndex = 76;
            this.textBox1.Text = "8701";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(57, 441);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(36, 20);
            this.textBox2.TabIndex = 77;
            this.textBox2.Text = "8701";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(100, 441);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(36, 20);
            this.textBox3.TabIndex = 78;
            this.textBox3.Text = "8701";
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(141, 441);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(36, 20);
            this.textBox4.TabIndex = 79;
            this.textBox4.Text = "8701";
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(15, 467);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(36, 20);
            this.textBox5.TabIndex = 80;
            this.textBox5.Text = "8701";
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(57, 467);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(36, 20);
            this.textBox6.TabIndex = 81;
            this.textBox6.Text = "8701";
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(100, 467);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(36, 20);
            this.textBox7.TabIndex = 82;
            this.textBox7.Text = "8701";
            // 
            // textBox8
            // 
            this.textBox8.Location = new System.Drawing.Point(141, 467);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(36, 20);
            this.textBox8.TabIndex = 83;
            this.textBox8.Text = "8701";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(238, 190);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(64, 13);
            this.label24.TabIndex = 84;
            this.label24.Text = "|   Vpp [mV]:";
            // 
            // tBVpp
            // 
            this.tBVpp.Location = new System.Drawing.Point(304, 187);
            this.tBVpp.Name = "tBVpp";
            this.tBVpp.Size = new System.Drawing.Size(33, 20);
            this.tBVpp.TabIndex = 85;
            this.tBVpp.Text = "1000";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(343, 190);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(63, 13);
            this.label25.TabIndex = 86;
            this.label25.Text = "Length [us]:";
            // 
            // tBStimLength
            // 
            this.tBStimLength.Location = new System.Drawing.Point(407, 187);
            this.tBStimLength.Name = "tBStimLength";
            this.tBStimLength.Size = new System.Drawing.Size(44, 20);
            this.tBStimLength.TabIndex = 87;
            this.tBStimLength.Text = "240";
            // 
            // cBMultiPatternStim
            // 
            this.cBMultiPatternStim.AutoSize = true;
            this.cBMultiPatternStim.Location = new System.Drawing.Point(170, 212);
            this.cBMultiPatternStim.Name = "cBMultiPatternStim";
            this.cBMultiPatternStim.Size = new System.Drawing.Size(139, 17);
            this.cBMultiPatternStim.TabIndex = 88;
            this.cBMultiPatternStim.Text = "Multi-Pattern Stimulation";
            this.cBMultiPatternStim.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 430);
            this.Controls.Add(this.cBMultiPatternStim);
            this.Controls.Add(this.tBStimLength);
            this.Controls.Add(this.label25);
            this.Controls.Add(this.tBVpp);
            this.Controls.Add(this.label24);
            this.Controls.Add(this.textBox8);
            this.Controls.Add(this.textBox7);
            this.Controls.Add(this.textBox6);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.bStartStim);
            this.Controls.Add(this.rBSorted);
            this.Controls.Add(this.rBRandom);
            this.Controls.Add(this.label23);
            this.Controls.Add(this.label22);
            this.Controls.Add(this.tBNumPorts);
            this.Controls.Add(this.label21);
            this.Controls.Add(this.label20);
            this.Controls.Add(this.pBLengthBufferQueue);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.btnScriptRunning);
            this.Controls.Add(this.tBTPort);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.tBSPort);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.tBAuthKey);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.bSTScript);
            this.Controls.Add(this.cB_SCU2);
            this.Controls.Add(this.cB_SCU1);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.cB_SCU2_HS4);
            this.Controls.Add(this.cB_SCU1_HS4);
            this.Controls.Add(this.cB_SCU2_HS3);
            this.Controls.Add(this.cB_SCU1_HS3);
            this.Controls.Add(this.cB_SCU2_HS2);
            this.Controls.Add(this.cB_SCU1_HS2);
            this.Controls.Add(this.cB_SCU2_HS1);
            this.Controls.Add(this.cB_SCU1_HS1);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tBNumStim);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pBLengthOutQueue);
            this.Controls.Add(this.pBLengthInQueue);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tBInterSpikePeriod);
            this.Controls.Add(this.tBIntStimInt);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tBFilteringFreq);
            this.Controls.Add(this.tBSamplingFreq);
            this.Controls.Add(this.btnDeviceOK);
            this.Controls.Add(this.dspSerial);
            this.Controls.Add(this.rawSerial);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.StopDSP);
            this.Controls.Add(this.UploadDSPBinary);
            this.Name = "Form1";
            this.Text = "Control MEA2100 DSP";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button UploadDSPBinary;
        private System.Windows.Forms.Button StopDSP;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox rawSerial;
        private System.Windows.Forms.TextBox dspSerial;
        private System.Windows.Forms.Button btnDeviceOK;
        private System.Windows.Forms.TextBox tBSamplingFreq;
        private System.Windows.Forms.TextBox tBFilteringFreq;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tBIntStimInt;
        private System.Windows.Forms.TextBox tBInterSpikePeriod;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ProgressBar pBLengthInQueue;
        private System.Windows.Forms.ProgressBar pBLengthOutQueue;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tBNumStim;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox cB_SCU1_HS1;
        private System.Windows.Forms.ComboBox cB_SCU2_HS1;
        private System.Windows.Forms.ComboBox cB_SCU2_HS2;
        private System.Windows.Forms.ComboBox cB_SCU1_HS2;
        private System.Windows.Forms.ComboBox cB_SCU2_HS4;
        private System.Windows.Forms.ComboBox cB_SCU1_HS4;
        private System.Windows.Forms.ComboBox cB_SCU2_HS3;
        private System.Windows.Forms.ComboBox cB_SCU1_HS3;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.CheckBox cB_SCU1;
        private System.Windows.Forms.CheckBox cB_SCU2;
        private System.Windows.Forms.Button bSTScript;
        private System.Windows.Forms.TextBox tBAuthKey;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox tBSPort;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox tBTPort;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Button btnScriptRunning;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.ProgressBar pBLengthBufferQueue;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox tBNumPorts;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.RadioButton rBRandom;
        private System.Windows.Forms.RadioButton rBSorted;
        private System.Windows.Forms.Button bStartStim;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox tBVpp;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.TextBox tBStimLength;
        private System.Windows.Forms.CheckBox cBMultiPatternStim;
    }
}

