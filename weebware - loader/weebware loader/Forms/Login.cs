using loader;
using loader.Authentication;
using loader.Functions;
using SafeRequest.NET;
using System;
using System.Drawing;
using System.IO;
using System.Net;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using weebware_loader.Forms;

namespace weebware_loader {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public partial class Login : Form {
        public Login() {
            InitializeComponent();
            if (Properties.Settings.Default.username != string.Empty) {
                txtUsername.Text = Properties.Settings.Default.username;
                txtPassword.Text = Properties.Settings.Default.password;
                txtPassword.Select();
            }
            tmrBorder.Start();
            this.Movable(pictureBox1);
            pnlBar.BackColor = Utils.get_color("topbar");
            pnlPasswordTxt.BackColor = Utils.get_color("txtboxbg");
            pnlUsernameTxt.BackColor = Utils.get_color("txtboxbg");

            lblClose.LabelButton(Color.Red);
        }

        private void tmrBorder_Tick(object sender, EventArgs e) { UpdateBorders(); }

        private void UpdateBorders() {
            if (txtPassword.Focused == true) pnlPasswordTxt.Show();
            else pnlPasswordTxt.Hide();

            if (txtUsername.Focused == true) pnlUsernameTxt.Show();
            else pnlUsernameTxt.Hide();
        }

        private void label4_Click(object sender, EventArgs e) { Environment.Exit(1337); }

        private void btnLogin_Click(object sender, EventArgs e) {
            AntiTamper.IntegrityCheck();
            Response response = Networking.SafeLogin(txtUsername.Text, txtPassword.Text);
            if (response != null) {
                if (!response.status) {
                    AntiTamper.IntegrityCheck();
                    string issue = response.GetData<string>("detail");
                    WebClient web = new WebClient();
                    switch (issue) {
                        case "connection error":
                        default:
                            Utils.UnknownError();
                            return;
                        case "no account":
                        case "wrong password":
                            MessageBox.Show("Incorrect username or password.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            txtPassword.Clear();
                            return;
                        case "sub invalid":
                            MessageBox.Show("Your subscription is invalid or expired.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        case "hwid mismatch":
                            MessageBox.Show("Your PC is not authorized.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        case "update":
                            MessageBox.Show("Please update by signing in at weebware.net and downloading a new loader.");
                            return;
                        case "banned":
                            MessageBox.Show("You have been banned. Contact support for more information.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        case "server offline":
                            MessageBox.Show($"The server is currently disabled.\nReason: {response.GetData<string>("reason")}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            break;
                    }
                } else if ((response.status) && response.GetData<string>("username") == txtUsername.Text && response.GetData<long>("time_left") > 0) {


                    if (cbRememberMe.Checked) {
                        Properties.Settings.Default.username = txtUsername.Text;
                        Properties.Settings.Default.password = txtPassword.Text;
                        Properties.Settings.Default.Save();
                    }

                    MessageBox.Show($"Successfully logged in.\nWelcome back, {response.GetData<string>("username")}!", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Information);

                    if (response.GetData<string>("notify") != File.ReadAllText(@"c:\weebware\dependencies\notify")) {
                        MessageBox.Show(response.GetData<string>("notify"), "weebware");
                        File.WriteAllText(@"c:\weebware\dependencies\notify", response.GetData<string>("notify"));
                    }


                    if (response.GetData<bool>("email")) {
                        Settings.username = response.GetData<string>("username");
                        Form emailForm = new Emailnput(response);
                        emailForm.Show();
                        Hide();
                    }
                    else {
                        Form mainForm = new Main(response);
                        AntiTamper.IntegrityCheck();
                        mainForm.Show();
                        Hide();
                    }

                } else { Utils.UnknownError(); }
            } else { Utils.UnknownError(); }
        }
    }
}
