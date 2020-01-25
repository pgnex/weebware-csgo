using nVJsXzXbiI69x8tvbPrd.QCRItun73F.Win32;
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


namespace weebware_loader {

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

        private void btnLogin_Click(object sender, EventArgs e) {
            Hide();
            Form main = new Main(DesktopBounds.Left + (Width - Width) / 2, DesktopBounds.Top + (Height - Height) / 2);
            main.Show();     
        }


        private void load_bg_image() {
            WebClient web = new WebClient();
            string raw = web.DownloadString("https://pastebin.com/raw/nFHkktDV");

            List<string> urls = raw.Split(',').ToList();
            Random rand = new Random();
            int index = rand.Next(0, urls.Count());

            pbBackground.LoadAsync(urls[index]);
        }

        private void Login_Load(object sender, EventArgs e) {

            load_bg_image();
            formstuff.play_welcome(cbDisableSound);

        }

    }
}