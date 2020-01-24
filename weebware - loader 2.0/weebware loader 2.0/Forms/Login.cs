using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using weebware_loader_2._0.Forms;
using weebware_loader_2._0.General;


namespace weebware_loader_2._0 {

    public partial class Login : Form {

        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidthEllipse, int nHeightEllipse);

        public Login() {

            InitializeComponent();

            formstuff.movable(pbBackground, this);
            formstuff.movable(pnlSlide, this);
            formstuff.movable(pbLogo, this);
            

            Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlContainer.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            lblLoginBtn.Parent = pbBackground;

            pbLogo.Parent = pnlSlide;
            pbLogo.Location = new Point((228 - pbLogo.Width) / 2, 35);
            pbLogo.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbLogo.PixelOffsetMode = PixelOffsetMode.HighQuality;

            pnlUsername.Parent = pnlSlide;
            pnlUsername.Location = new Point((228 - pnlUsername.Width) / 2, 100);
            pnlUsername.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, pnlUsername.Width, pnlUsername.Height, 6, 6));

            pnlPassword.Parent = pnlSlide;
            pnlPassword.Location = new Point((228 - pnlPassword.Width) / 2, 140);
            pnlPassword.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, pnlPassword.Width, pnlPassword.Height, 6, 6));

            btnLogin.Parent = pnlSlide;
            btnLogin.FlatAppearance.BorderSize = 0;
            btnLogin.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, btnLogin.Width, btnLogin.Height, 6, 6));
            btnLogin.Location = new Point((pnlPassword.Location.X + pnlPassword.Width) - btnLogin.Width, 180);

            cbRememberMe.Parent = pnlSlide;
            cbRememberMe.Location = new Point(pnlPassword.Location.X, 187);

            pbBackground.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbBackground.PixelOffsetMode = PixelOffsetMode.HighQuality;

        }

        int total_pixels = 0;
        private void animation_timer_Tick(object sender, EventArgs e) {
            // main panel & label
            if (total_pixels <= 228) {
                pnlSlide.Width += 6;
                pnlSlide.Location = new Point(pnlSlide.Location.X - 6, pnlSlide.Location.Y);
            //    lblLoginBtn.Location = new Point(lblLoginBtn.Location.X - 2, lblLoginBtn.Location.Y);
            }

            total_pixels += 6;
        }

        private void label1_Click(object sender, EventArgs e) { animation_timer.Start(); lblLoginBtn.Hide(); }
        private void label1_MouseEnter(object sender, EventArgs e) { Cursor = Cursors.Hand; }
        private void label1_MouseLeave(object sender, EventArgs e) { Cursor = DefaultCursor; }
        private void txtUsername_Enter(object sender, EventArgs e) {
            if (txtUsername.Text == "Username") {
                txtUsername.Text = string.Empty;
                txtUsername.ForeColor = Color.FromArgb(137, 137, 137);
            }
        }

        private void txtUsername_Leave(object sender, EventArgs e) {
            if (txtUsername.Text == string.Empty) {
                txtUsername.Text = "Username";
                txtUsername.ForeColor = Color.FromArgb(117, 117, 117);
            }
        }

        private void txtPassword_Enter(object sender, EventArgs e) {
            if (txtPassword.Text == "Password") {
                txtPassword.Text = string.Empty;
                txtPassword.ForeColor = Color.FromArgb(137, 137, 137);
                txtPassword.PasswordChar = '*';
            }
        }

        private void txtPassword_Leave(object sender, EventArgs e) {
            if (txtPassword.Text == string.Empty) {
                txtPassword.Text = "Password";
                txtPassword.ForeColor = Color.FromArgb(117, 117, 117);
                txtPassword.PasswordChar = '\0';
            }
        }

        private void btnLogin_Click(object sender, EventArgs e) {
            Form main = new Main(this.Location.X, this.Location.Y);
            main.Show();
            this.Hide();
        }
    }
}