namespace weebware_loader.Forms
{
    partial class Main
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblVersion = new System.Windows.Forms.Label();
            this.lblStatus = new System.Windows.Forms.Label();
            this.lblExpireDate = new System.Windows.Forms.Label();
            this.lblLoggedIn = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.pnlCheat0 = new System.Windows.Forms.Panel();
            this.lblCheat0 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnInject = new System.Windows.Forms.Button();
            this.btnExit = new System.Windows.Forms.Button();
            this.loadTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.pnlCheat0.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(382, 244);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(53)))), ((int)(((byte)(49)))));
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.lblVersion);
            this.panel1.Controls.Add(this.lblStatus);
            this.panel1.Controls.Add(this.lblExpireDate);
            this.panel1.Controls.Add(this.lblLoggedIn);
            this.panel1.Location = new System.Drawing.Point(12, 153);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(240, 82);
            this.panel1.TabIndex = 1;
            // 
            // lblVersion
            // 
            this.lblVersion.AutoSize = true;
            this.lblVersion.BackColor = System.Drawing.Color.Transparent;
            this.lblVersion.Font = new System.Drawing.Font("Verdana", 8F);
            this.lblVersion.ForeColor = System.Drawing.Color.White;
            this.lblVersion.Location = new System.Drawing.Point(2, 37);
            this.lblVersion.Name = "lblVersion";
            this.lblVersion.Size = new System.Drawing.Size(63, 13);
            this.lblVersion.TabIndex = 23;
            this.lblVersion.Text = "Version: -";
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.BackColor = System.Drawing.Color.Transparent;
            this.lblStatus.Font = new System.Drawing.Font("Verdana", 8F);
            this.lblStatus.ForeColor = System.Drawing.Color.White;
            this.lblStatus.Location = new System.Drawing.Point(2, 53);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(106, 13);
            this.lblStatus.TabIndex = 22;
            this.lblStatus.Text = "Account Status: -";
            // 
            // lblExpireDate
            // 
            this.lblExpireDate.AutoSize = true;
            this.lblExpireDate.BackColor = System.Drawing.Color.Transparent;
            this.lblExpireDate.Font = new System.Drawing.Font("Verdana", 8F);
            this.lblExpireDate.ForeColor = System.Drawing.Color.White;
            this.lblExpireDate.Location = new System.Drawing.Point(2, 21);
            this.lblExpireDate.Name = "lblExpireDate";
            this.lblExpireDate.Size = new System.Drawing.Size(63, 13);
            this.lblExpireDate.TabIndex = 21;
            this.lblExpireDate.Text = "Expires: -";
            // 
            // lblLoggedIn
            // 
            this.lblLoggedIn.AutoSize = true;
            this.lblLoggedIn.BackColor = System.Drawing.Color.Transparent;
            this.lblLoggedIn.Font = new System.Drawing.Font("Verdana", 8F);
            this.lblLoggedIn.ForeColor = System.Drawing.Color.White;
            this.lblLoggedIn.Location = new System.Drawing.Point(2, 4);
            this.lblLoggedIn.Name = "lblLoggedIn";
            this.lblLoggedIn.Size = new System.Drawing.Size(93, 13);
            this.lblLoggedIn.TabIndex = 20;
            this.lblLoggedIn.Text = "Logged in as: -";
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(53)))), ((int)(((byte)(49)))));
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.pnlCheat0);
            this.panel2.Location = new System.Drawing.Point(12, 41);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(240, 96);
            this.panel2.TabIndex = 2;
            // 
            // pnlCheat0
            // 
            this.pnlCheat0.BackColor = System.Drawing.Color.DarkSlateBlue;
            this.pnlCheat0.Controls.Add(this.lblCheat0);
            this.pnlCheat0.Location = new System.Drawing.Point(-1, 1);
            this.pnlCheat0.Name = "pnlCheat0";
            this.pnlCheat0.Size = new System.Drawing.Size(240, 19);
            this.pnlCheat0.TabIndex = 0;
            // 
            // lblCheat0
            // 
            this.lblCheat0.AutoSize = true;
            this.lblCheat0.BackColor = System.Drawing.Color.Transparent;
            this.lblCheat0.Font = new System.Drawing.Font("Verdana", 8F);
            this.lblCheat0.ForeColor = System.Drawing.Color.White;
            this.lblCheat0.Location = new System.Drawing.Point(3, 3);
            this.lblCheat0.Name = "lblCheat0";
            this.lblCheat0.Size = new System.Drawing.Size(45, 13);
            this.lblCheat0.TabIndex = 13;
            this.lblCheat0.Text = "cheat0";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(21)))), ((int)(((byte)(21)))), ((int)(((byte)(21)))));
            this.label2.Font = new System.Drawing.Font("Verdana", 8F);
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.label2.Location = new System.Drawing.Point(11, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(102, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Cheat Selection:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(21)))), ((int)(((byte)(21)))), ((int)(((byte)(21)))));
            this.label1.Font = new System.Drawing.Font("Verdana", 8F);
            this.label1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.label1.Location = new System.Drawing.Point(11, 138);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(109, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "User Information:";
            // 
            // btnInject
            // 
            this.btnInject.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(53)))), ((int)(((byte)(49)))));
            this.btnInject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnInject.ForeColor = System.Drawing.SystemColors.ControlDark;
            this.btnInject.Location = new System.Drawing.Point(260, 41);
            this.btnInject.Name = "btnInject";
            this.btnInject.Size = new System.Drawing.Size(112, 23);
            this.btnInject.TabIndex = 18;
            this.btnInject.Text = "Inject";
            this.btnInject.UseVisualStyleBackColor = false;
            this.btnInject.Click += new System.EventHandler(this.btnInject_Click);
            // 
            // btnExit
            // 
            this.btnExit.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(53)))), ((int)(((byte)(49)))));
            this.btnExit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnExit.ForeColor = System.Drawing.SystemColors.ControlDark;
            this.btnExit.Location = new System.Drawing.Point(260, 70);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(112, 23);
            this.btnExit.TabIndex = 19;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = false;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // loadTimer
            // 
            this.loadTimer.Interval = 5;
            this.loadTimer.Tick += new System.EventHandler(this.loadTimer_Tick);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 244);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.btnInject);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Main";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.pnlCheat0.ResumeLayout(false);
            this.pnlCheat0.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel pnlCheat0;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblCheat0;
        private System.Windows.Forms.Button btnInject;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Timer loadTimer;
        private System.Windows.Forms.Label lblLoggedIn;
        private System.Windows.Forms.Label lblExpireDate;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Label lblVersion;
    }
}