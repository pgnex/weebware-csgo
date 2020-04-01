using weebware_loader.Custom_Components;
using weebware_loader.General;

namespace weebware_loader {
    partial class Login {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Login));
            this.animation_timer = new System.Windows.Forms.Timer(this.components);
            this.pnlSlide = new System.Windows.Forms.Panel();
            this.pnlUsername = new System.Windows.Forms.Panel();
            this.txtUsername = new weebware_loader.General.PlaceholderTextbox();
            this.pnlPassword = new System.Windows.Forms.Panel();
            this.txtPassword = new weebware_loader.General.PlaceholderTextbox();
            this.pnlContainer = new System.Windows.Forms.Panel();
            this.lblDisableSound = new weebware_loader.Custom_Components.CustomLabel();
            this.cbDisableSound = new System.Windows.Forms.CheckBox();
            this.cbRememberMe = new System.Windows.Forms.CheckBox();
            this.btnLogin = new System.Windows.Forms.Button();
            this.pbLogo = new weebware_loader.CPictureBox();
            this.lblLoginBtn = new weebware_loader.Custom_Components.CustomLabel();
            this.pbBackground = new weebware_loader.CPictureBox();
            this.pnlUsername.SuspendLayout();
            this.pnlPassword.SuspendLayout();
            this.pnlContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbBackground)).BeginInit();
            this.SuspendLayout();
            // 
            // animation_timer
            // 
            this.animation_timer.Interval = 1;
            this.animation_timer.Tick += new System.EventHandler(this.animation_timer_Tick);
            // 
            // pnlSlide
            // 
            this.pnlSlide.Location = new System.Drawing.Point(482, 0);
            this.pnlSlide.Name = "pnlSlide";
            this.pnlSlide.Size = new System.Drawing.Size(1, 259);
            this.pnlSlide.TabIndex = 2;
            this.pnlSlide.Visible = false;
            // 
            // pnlUsername
            // 
            this.pnlUsername.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.pnlUsername.Controls.Add(this.txtUsername);
            this.pnlUsername.Location = new System.Drawing.Point(305, 120);
            this.pnlUsername.Name = "pnlUsername";
            this.pnlUsername.Size = new System.Drawing.Size(170, 30);
            this.pnlUsername.TabIndex = 5;
            // 
            // txtUsername
            // 
            this.txtUsername.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.txtUsername.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtUsername.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtUsername.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.txtUsername.Location = new System.Drawing.Point(11, 7);
            this.txtUsername.Name = "txtUsername";
            this.txtUsername.PlaceHolder = "Placeholder";
            this.txtUsername.Size = new System.Drawing.Size(148, 15);
            this.txtUsername.TabIndex = 0;
            this.txtUsername.Text = "Username";
            this.txtUsername.usePasswordChar = false;
            // 
            // pnlPassword
            // 
            this.pnlPassword.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.pnlPassword.Controls.Add(this.txtPassword);
            this.pnlPassword.Location = new System.Drawing.Point(305, 165);
            this.pnlPassword.Name = "pnlPassword";
            this.pnlPassword.Size = new System.Drawing.Size(170, 30);
            this.pnlPassword.TabIndex = 6;
            // 
            // txtPassword
            // 
            this.txtPassword.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.txtPassword.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtPassword.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txtPassword.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.txtPassword.Location = new System.Drawing.Point(11, 7);
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.PlaceHolder = "Placeholder";
            this.txtPassword.Size = new System.Drawing.Size(148, 15);
            this.txtPassword.TabIndex = 1;
            this.txtPassword.Text = "Password";
            this.txtPassword.usePasswordChar = false;
            // 
            // pnlContainer
            // 
            this.pnlContainer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(4)))), ((int)(((byte)(4)))), ((int)(((byte)(4)))));
            this.pnlContainer.Controls.Add(this.lblDisableSound);
            this.pnlContainer.Controls.Add(this.cbDisableSound);
            this.pnlContainer.Controls.Add(this.cbRememberMe);
            this.pnlContainer.Controls.Add(this.btnLogin);
            this.pnlContainer.Controls.Add(this.pnlPassword);
            this.pnlContainer.Controls.Add(this.pnlUsername);
            this.pnlContainer.Controls.Add(this.pbLogo);
            this.pnlContainer.Controls.Add(this.pnlSlide);
            this.pnlContainer.Controls.Add(this.lblLoginBtn);
            this.pnlContainer.Controls.Add(this.pbBackground);
            this.pnlContainer.Location = new System.Drawing.Point(4, 3);
            this.pnlContainer.Name = "pnlContainer";
            this.pnlContainer.Size = new System.Drawing.Size(483, 259);
            this.pnlContainer.TabIndex = 1;
            // 
            // lblDisableSound
            // 
            this.lblDisableSound.AutoSize = true;
            this.lblDisableSound.BackColor = System.Drawing.Color.Transparent;
            this.lblDisableSound.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblDisableSound.ForeColor = System.Drawing.Color.White;
            this.lblDisableSound.Location = new System.Drawing.Point(23, 240);
            this.lblDisableSound.Name = "lblDisableSound";
            this.lblDisableSound.OutlineForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(191)))), ((int)(((byte)(82)))), ((int)(((byte)(173)))));
            this.lblDisableSound.OutlineWidth = 1F;
            this.lblDisableSound.Size = new System.Drawing.Size(152, 13);
            this.lblDisableSound.TabIndex = 9;
            this.lblDisableSound.Text = "Disable Launcher Sounds";
            this.lblDisableSound.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
            this.lblDisableSound.Click += new System.EventHandler(this.lblDisableSound_Click);
            // 
            // cbDisableSound
            // 
            this.cbDisableSound.AutoSize = true;
            this.cbDisableSound.BackColor = System.Drawing.Color.Transparent;
            this.cbDisableSound.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.cbDisableSound.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(218)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.cbDisableSound.Location = new System.Drawing.Point(7, 239);
            this.cbDisableSound.Name = "cbDisableSound";
            this.cbDisableSound.Size = new System.Drawing.Size(15, 14);
            this.cbDisableSound.TabIndex = 7;
            this.cbDisableSound.UseVisualStyleBackColor = false;
            this.cbDisableSound.CheckedChanged += new System.EventHandler(this.cbDisableSound_CheckedChanged);
            // 
            // cbRememberMe
            // 
            this.cbRememberMe.AutoSize = true;
            this.cbRememberMe.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.cbRememberMe.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(137)))), ((int)(((byte)(137)))), ((int)(((byte)(137)))));
            this.cbRememberMe.Location = new System.Drawing.Point(305, 219);
            this.cbRememberMe.Name = "cbRememberMe";
            this.cbRememberMe.Size = new System.Drawing.Size(95, 17);
            this.cbRememberMe.TabIndex = 2;
            this.cbRememberMe.Text = "Remember Me";
            this.cbRememberMe.UseVisualStyleBackColor = true;
            // 
            // btnLogin
            // 
            this.btnLogin.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(218)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.btnLogin.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLogin.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F);
            this.btnLogin.ForeColor = System.Drawing.Color.White;
            this.btnLogin.Location = new System.Drawing.Point(403, 210);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(70, 30);
            this.btnLogin.TabIndex = 3;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = false;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // pbLogo
            // 
            this.pbLogo.BackColor = System.Drawing.Color.Black;
            this.pbLogo.Image = ((System.Drawing.Image)(resources.GetObject("pbLogo.Image")));
            this.pbLogo.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Default;
            this.pbLogo.Location = new System.Drawing.Point(336, 42);
            this.pbLogo.Name = "pbLogo";
            this.pbLogo.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Default;
            this.pbLogo.Size = new System.Drawing.Size(80, 32);
            this.pbLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbLogo.TabIndex = 3;
            this.pbLogo.TabStop = false;
            // 
            // lblLoginBtn
            // 
            this.lblLoginBtn.AutoSize = true;
            this.lblLoginBtn.BackColor = System.Drawing.Color.Transparent;
            this.lblLoginBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblLoginBtn.ForeColor = System.Drawing.Color.White;
            this.lblLoginBtn.Location = new System.Drawing.Point(203, 120);
            this.lblLoginBtn.Name = "lblLoginBtn";
            this.lblLoginBtn.OutlineForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(218)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.lblLoginBtn.OutlineWidth = 2F;
            this.lblLoginBtn.Size = new System.Drawing.Size(72, 24);
            this.lblLoginBtn.TabIndex = 0;
            this.lblLoginBtn.Text = "LOGIN";
            this.lblLoginBtn.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
            this.lblLoginBtn.Click += new System.EventHandler(this.lblLoginBtn_Click);
            this.lblLoginBtn.MouseEnter += new System.EventHandler(this.lblLoginBtn_MouseEnter);
            this.lblLoginBtn.MouseLeave += new System.EventHandler(this.lblLoginBtn_MouseLeave);
            // 
            // pbBackground
            // 
            this.pbBackground.Image = global::weebware_loader.Properties.Resources.defaultbg;
            this.pbBackground.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Default;
            this.pbBackground.Location = new System.Drawing.Point(0, 0);
            this.pbBackground.Name = "pbBackground";
            this.pbBackground.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Default;
            this.pbBackground.Size = new System.Drawing.Size(483, 259);
            this.pbBackground.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbBackground.TabIndex = 0;
            this.pbBackground.TabStop = false;
            // 
            // Login
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(217)))), ((int)(((byte)(80)))), ((int)(((byte)(196)))));
            this.ClientSize = new System.Drawing.Size(492, 266);
            this.Controls.Add(this.pnlContainer);
            this.ForeColor = System.Drawing.Color.Cornsilk;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Login";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.Login_Load);
            this.pnlUsername.ResumeLayout(false);
            this.pnlUsername.PerformLayout();
            this.pnlPassword.ResumeLayout(false);
            this.pnlPassword.PerformLayout();
            this.pnlContainer.ResumeLayout(false);
            this.pnlContainer.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbBackground)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Timer animation_timer;
        private CPictureBox pbBackground;
        private CustomLabel lblLoginBtn;
        private System.Windows.Forms.Panel pnlSlide;
        private CPictureBox pbLogo;
        private System.Windows.Forms.Panel pnlUsername;
        private PlaceholderTextbox txtUsername;
        private System.Windows.Forms.Panel pnlPassword;
        private PlaceholderTextbox txtPassword;
        private System.Windows.Forms.Panel pnlContainer;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.CheckBox cbRememberMe;
        private System.Windows.Forms.CheckBox cbDisableSound;
        private CustomLabel lblDisableSound;
    }
}

