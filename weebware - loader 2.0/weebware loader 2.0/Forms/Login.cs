using loader;
using loader.Authentication;
using nVJsXzXbiI69x8tvbPrd.QCRItun73F.Win32;
using SafeRequest;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Net;
using System.Windows.Forms;
using weebware_loader.Forms;
using weebware_loader.General;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using weebware_loader.Custom_Components;
using System.Reflection;

namespace weebware_loader {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public partial class Login : Form {

        PrivateFontCollection pfc = formstuff.createfont();
        public Login() {

            InitializeComponent();

            lblLoginBtn.Font = new Font(pfc.Families[0], 13, FontStyle.Bold);
            txtUsername.Font = new Font(pfc.Families[0], 10, FontStyle.Bold);
            txtPassword.Font = new Font(pfc.Families[0], 10, FontStyle.Bold);
            btnLogin.Font = new Font(pfc.Families[0], 10, FontStyle.Bold);
            cbRememberMe.Font = new Font(pfc.Families[0], 7.25f, FontStyle.Bold);
            cbDisableSound.Font = new Font(pfc.Families[0], 7.25f, FontStyle.Bold);
            lblDisableSound.Font = new Font(pfc.Families[0], 8.25f, FontStyle.Regular);

            lblLoginBtn.TabStop = true;
            pnlUsername.TabStop = true;
            pnlPassword.TabStop = true;
            pnlSlide.TabStop = true;
            pnlContainer.TabStop = true;

            formstuff.movable(pbBackground, this);
            formstuff.movable(pnlSlide, this);
            formstuff.movable(pbLogo, this);


            Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlContainer.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            lblLoginBtn.Parent = pbBackground;
            cbDisableSound.Parent = pbBackground;
            lblDisableSound.Parent = pbBackground;

            pbLogo.Parent = pnlSlide;
            pbLogo.Location = new Point((228 - pbLogo.Width) / 2, 35);
            pbLogo.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbLogo.PixelOffsetMode = PixelOffsetMode.HighQuality;

            pnlUsername.Parent = pnlSlide;
            pnlUsername.Location = new Point((228 - pnlUsername.Width) / 2, 100);
            pnlUsername.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, pnlUsername.Width, pnlUsername.Height, 6, 6));

            pnlPassword.Parent = pnlSlide;
            pnlPassword.Location = new Point((228 - pnlPassword.Width) / 2, 140);
            pnlPassword.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, pnlPassword.Width, pnlPassword.Height, 6, 6));

            btnLogin.Parent = pnlSlide;
            btnLogin.FlatAppearance.BorderSize = 0;
            btnLogin.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, btnLogin.Width, btnLogin.Height, 6, 6));
            btnLogin.Location = new Point((pnlPassword.Location.X + pnlPassword.Width) - btnLogin.Width, 180);

            cbRememberMe.Parent = pnlSlide;
            cbRememberMe.Location = new Point(pnlPassword.Location.X, 187);

            pbBackground.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbBackground.PixelOffsetMode = PixelOffsetMode.HighQuality;

            txtUsername.PlaceHolder = "Username";
            txtPassword.PlaceHolder = "Password";
            txtPassword.usePasswordChar = true;

            if (Properties.Settings.Default.username != string.Empty
                && Properties.Settings.Default.password != string.Empty) {
                cbRememberMe.Checked = true;
                txtUsername.Text = Properties.Settings.Default.username;
                txtPassword.Text = Properties.Settings.Default.password;
                txtPassword.PasswordChar = '*';
            }

        }

        int total_pixels = 0;
        private void animation_timer_Tick(object sender, EventArgs e) {
            // main panel & label
            if (total_pixels <= 228) {
                try {
                    pnlSlide.Width += 6;
                    pnlSlide.Location = new Point(pnlSlide.Location.X - 6, pnlSlide.Location.Y);
                } catch (Exception ex) {
                    MessageBox.Show(ex.Message, "weebware", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            total_pixels += 6;
        }

        private void lblLoginBtn_Click(object sender, EventArgs e) { animation_timer.Start(); pnlSlide.Visible = true; lblLoginBtn.Hide(); txtUsername.Select(); }
        private void lblLoginBtn_MouseEnter(object sender, EventArgs e) { Cursor = Cursors.Hand; }
        private void lblLoginBtn_MouseLeave(object sender, EventArgs e) { Cursor = DefaultCursor; }
        private void lblDisableSound_Click(object sender, EventArgs e) { cbDisableSound.Checked = !cbDisableSound.Checked; }
        private void cbDisableSound_CheckedChanged(object sender, EventArgs e) {
            Properties.Settings.Default.disablesounds = cbDisableSound.Checked;
            Properties.Settings.Default.Save();
        }

        private void FailedSafeLogin(Response response) {
            string issue = response.GetData<string>("detail");
            switch (issue) {
                case "invalid account":
                case "invalid password":
                    MessageBox.Show("Incorrect username or password.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                case "invalid request":
                case "connection error":
                    Utils.ConnectionError();
                    return;
                case "server offline":
                    MessageBox.Show($"The server is currently disabled.\nReason: {response.GetData<string>("reason")}", "Server Disabled", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                case "update loader":
                    MessageBox.Show("Please update by signing in at weebware.net and downloading a new loader.", "Update Required", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                case "banned":
                    MessageBox.Show("You have been banned.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                case "hwid mismatch":
                    MessageBox.Show("Your PC is not authorized. Sign in at weebware.net to request a reset", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                case "sub invalid":
                    MessageBox.Show("Your subscription is invalid or expired.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
            }
        }

        private void btnLogin_Click(object sender, EventArgs e) {

            Response response = Networking.SafeLogin(txtUsername.Text, txtPassword.Text);

            if (response == null) {
                Utils.ConnectionError();
                return;
            }

            if (!response.status) {
                FailedSafeLogin(response);
                return;
            }

            if (response.status) {
                // successful login

                if (cbRememberMe.Checked) {
                    Properties.Settings.Default.username = txtUsername.Text;
                    Properties.Settings.Default.password = txtPassword.Text;
                    Properties.Settings.Default.Save();
                }

                Hide();
                LoginResponse loginResponse = JsonConvert.DeserializeObject<LoginResponse>(response.raw);
                Form main = new Main(DesktopBounds.Left + (Width - Width) / 2, DesktopBounds.Top + (Height - Height) / 2, loginResponse);
                main.Show();
            }


        }


        private void set_background_image() {
            string[] images = Directory.GetFiles(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "/weebware/images");
            int index = new Random().Next(0, images.Count());
            pbBackground.LoadAsync(images[index]);
        }

        private void Login_Load(object sender, EventArgs e) {
            set_background_image();
            formstuff.play_welcome(cbDisableSound);

        }

        private void Login_FormClosed(object sender, FormClosedEventArgs e) {
            Environment.Exit(1337);
        }
    }
}