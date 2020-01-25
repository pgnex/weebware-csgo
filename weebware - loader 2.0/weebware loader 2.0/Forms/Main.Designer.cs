namespace weebware_loader.Forms {
    partial class Main {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.pnlBackground = new System.Windows.Forms.Panel();
            this.pnlInfo = new System.Windows.Forms.Panel();
            this.lblSessionExp = new System.Windows.Forms.Label();
            this.lblCheatSelected = new System.Windows.Forms.Label();
            this.lblWelcomeBack = new System.Windows.Forms.Label();
            this.lblStatus = new System.Windows.Forms.Label();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnInject = new System.Windows.Forms.Button();
            this.pnlSelContainer = new System.Windows.Forms.Panel();
            this.lblWeebware = new System.Windows.Forms.Label();
            this.pbLogo = new weebware_loader.CPictureBox();
            this.tmrExpire = new System.Windows.Forms.Timer(this.components);
            this.lblCheatStatus = new System.Windows.Forms.Label();
            this.pnlBackground.SuspendLayout();
            this.pnlInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).BeginInit();
            this.SuspendLayout();
            // 
            // pnlBackground
            // 
            this.pnlBackground.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(4)))), ((int)(((byte)(4)))), ((int)(((byte)(4)))));
            this.pnlBackground.Controls.Add(this.lblCheatStatus);
            this.pnlBackground.Controls.Add(this.pnlInfo);
            this.pnlBackground.Controls.Add(this.lblStatus);
            this.pnlBackground.Controls.Add(this.btnExit);
            this.pnlBackground.Controls.Add(this.btnInject);
            this.pnlBackground.Controls.Add(this.pnlSelContainer);
            this.pnlBackground.Controls.Add(this.lblWeebware);
            this.pnlBackground.Controls.Add(this.pbLogo);
            this.pnlBackground.Location = new System.Drawing.Point(4, 3);
            this.pnlBackground.Name = "pnlBackground";
            this.pnlBackground.Size = new System.Drawing.Size(483, 327);
            this.pnlBackground.TabIndex = 0;
            // 
            // pnlInfo
            // 
            this.pnlInfo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(34)))), ((int)(((byte)(34)))), ((int)(((byte)(34)))));
            this.pnlInfo.Controls.Add(this.lblSessionExp);
            this.pnlInfo.Controls.Add(this.lblCheatSelected);
            this.pnlInfo.Controls.Add(this.lblWelcomeBack);
            this.pnlInfo.Location = new System.Drawing.Point(25, 219);
            this.pnlInfo.Name = "pnlInfo";
            this.pnlInfo.Size = new System.Drawing.Size(437, 84);
            this.pnlInfo.TabIndex = 11;
            // 
            // lblSessionExp
            // 
            this.lblSessionExp.AutoSize = true;
            this.lblSessionExp.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 9.75F);
            this.lblSessionExp.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.lblSessionExp.Location = new System.Drawing.Point(13, 59);
            this.lblSessionExp.Name = "lblSessionExp";
            this.lblSessionExp.Size = new System.Drawing.Size(252, 13);
            this.lblSessionExp.TabIndex = 2;
            this.lblSessionExp.Text = "Your session will expire in: 5 minutes ";
            // 
            // lblCheatSelected
            // 
            this.lblCheatSelected.AutoSize = true;
            this.lblCheatSelected.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 9.75F);
            this.lblCheatSelected.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.lblCheatSelected.Location = new System.Drawing.Point(13, 34);
            this.lblCheatSelected.Name = "lblCheatSelected";
            this.lblCheatSelected.Size = new System.Drawing.Size(129, 13);
            this.lblCheatSelected.TabIndex = 1;
            this.lblCheatSelected.Text = "Currently Selected";
            // 
            // lblWelcomeBack
            // 
            this.lblWelcomeBack.AutoSize = true;
            this.lblWelcomeBack.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 9.75F);
            this.lblWelcomeBack.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.lblWelcomeBack.Location = new System.Drawing.Point(13, 11);
            this.lblWelcomeBack.Name = "lblWelcomeBack";
            this.lblWelcomeBack.Size = new System.Drawing.Size(193, 13);
            this.lblWelcomeBack.TabIndex = 0;
            this.lblWelcomeBack.Text = "Welcome back to weebware, ";
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.BackColor = System.Drawing.Color.Transparent;
            this.lblStatus.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 8F, System.Drawing.FontStyle.Bold);
            this.lblStatus.ForeColor = System.Drawing.Color.White;
            this.lblStatus.Location = new System.Drawing.Point(21, 187);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(56, 11);
            this.lblStatus.TabIndex = 10;
            this.lblStatus.Text = "Status: ";
            // 
            // btnExit
            // 
            this.btnExit.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(54)))), ((int)(((byte)(54)))), ((int)(((byte)(54)))));
            this.btnExit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnExit.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 9.75F);
            this.btnExit.ForeColor = System.Drawing.Color.White;
            this.btnExit.Location = new System.Drawing.Point(347, 127);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(115, 40);
            this.btnExit.TabIndex = 9;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = false;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnInject
            // 
            this.btnInject.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(218)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.btnInject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnInject.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 9.75F);
            this.btnInject.ForeColor = System.Drawing.Color.White;
            this.btnInject.Location = new System.Drawing.Point(347, 81);
            this.btnInject.Name = "btnInject";
            this.btnInject.Size = new System.Drawing.Size(115, 40);
            this.btnInject.TabIndex = 8;
            this.btnInject.Text = "Inject";
            this.btnInject.UseVisualStyleBackColor = false;
            this.btnInject.Click += new System.EventHandler(this.btnInject_Click);
            // 
            // pnlSelContainer
            // 
            this.pnlSelContainer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.pnlSelContainer.Location = new System.Drawing.Point(25, 81);
            this.pnlSelContainer.Name = "pnlSelContainer";
            this.pnlSelContainer.Size = new System.Drawing.Size(264, 86);
            this.pnlSelContainer.TabIndex = 6;
            // 
            // lblWeebware
            // 
            this.lblWeebware.AutoSize = true;
            this.lblWeebware.BackColor = System.Drawing.Color.Transparent;
            this.lblWeebware.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblWeebware.ForeColor = System.Drawing.Color.White;
            this.lblWeebware.Location = new System.Drawing.Point(21, 32);
            this.lblWeebware.Name = "lblWeebware";
            this.lblWeebware.Size = new System.Drawing.Size(110, 19);
            this.lblWeebware.TabIndex = 5;
            this.lblWeebware.Text = "weebware";
            // 
            // pbLogo
            // 
            this.pbLogo.BackColor = System.Drawing.Color.Black;
            this.pbLogo.Image = global::weebware_loader.Properties.Resources.logo;
            this.pbLogo.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Default;
            this.pbLogo.Location = new System.Drawing.Point(382, 28);
            this.pbLogo.Name = "pbLogo";
            this.pbLogo.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Default;
            this.pbLogo.Size = new System.Drawing.Size(80, 32);
            this.pbLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbLogo.TabIndex = 4;
            this.pbLogo.TabStop = false;
            // 
            // tmrExpire
            // 
            this.tmrExpire.Enabled = true;
            this.tmrExpire.Interval = 10000;
            this.tmrExpire.Tick += new System.EventHandler(this.tmrExpire_Tick);
            // 
            // lblCheatStatus
            // 
            this.lblCheatStatus.AutoSize = true;
            this.lblCheatStatus.BackColor = System.Drawing.Color.Transparent;
            this.lblCheatStatus.Font = new System.Drawing.Font("UD Digi Kyokasho NP-R", 8F, System.Drawing.FontStyle.Bold);
            this.lblCheatStatus.ForeColor = System.Drawing.Color.Lime;
            this.lblCheatStatus.Location = new System.Drawing.Point(63, 187);
            this.lblCheatStatus.Name = "lblCheatStatus";
            this.lblCheatStatus.Size = new System.Drawing.Size(77, 11);
            this.lblCheatStatus.TabIndex = 12;
            this.lblCheatStatus.Text = "Undetected";
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(217)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.ClientSize = new System.Drawing.Size(492, 334);
            this.Controls.Add(this.pnlBackground);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.pnlBackground.ResumeLayout(false);
            this.pnlBackground.PerformLayout();
            this.pnlInfo.ResumeLayout(false);
            this.pnlInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnlBackground;
        private CPictureBox pbLogo;
        private System.Windows.Forms.Label lblWeebware;
        private System.Windows.Forms.Panel pnlSelContainer;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnInject;
        private System.Windows.Forms.Panel pnlInfo;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Label lblWelcomeBack;
        private System.Windows.Forms.Label lblSessionExp;
        private System.Windows.Forms.Label lblCheatSelected;
        private System.Windows.Forms.Timer tmrExpire;
        private System.Windows.Forms.Label lblCheatStatus;
    }
}