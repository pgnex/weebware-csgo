using loader;
using SafeRequest.NET;
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Net;
using System.Reflection;
using System.Windows.Forms;

namespace weebware_loader.Forms
{
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public partial class Main : Form
    {
        string downloadLink = string.Empty; string decryptionKey = string.Empty; string cheatFile = string.Empty;

        public Main(Response response) {
            InitializeComponent();
            this.Movable(pictureBox1);
            pnlCheat0.BackColor = Color.FromArgb(52, 53, 49);

            AntiTamper.IntegrityCheck();
            DateTime current = TimeZoneInfo.ConvertTimeBySystemTimeZoneId(DateTime.UtcNow, "Eastern Standard Time");
            string expireDate = string.Empty;

            if ((response.GetData<long>("time_left")) == 2000000000) {
                expireDate = "Never";
            }
            else {
                DateTime end = current.AddSeconds(response.GetData<long>("time_left"));
                expireDate = end.ToString("MMM d, yyyy @ hh:mm tt");
            }

            downloadLink = response.GetData<string>("download");
            decryptionKey = response.GetData<string>("key");
            lblVersion.Text = "Version: " + response.GetData<string>("version");
            lblCheat0.Text = response.GetData<string>("name");
            pnlCheat0.BackColor = Color.DarkSlateBlue;
            AntiTamper.IntegrityCheck();
            lblLoggedIn.Text = "Logged in as: " + response.GetData<string>("username");
            lblExpireDate.Text = "Expires: " + expireDate;


            switch (response.GetData<string>("detected")) {
                case "undetected":
                    lblStatus.Text = "Status: Undetected";
                    lblStatus.ForeColor = Color.Lime;
                    break;
                case "unknown":
                    lblStatus.Text = "Status: Unknown";
                    lblStatus.ForeColor = Color.Yellow;
                    break;
                case "detected":
                    lblStatus.Text = "Status: Detected";
                    lblStatus.ForeColor = Color.Red;
                    break;
            }
        }


        private void btnExit_Click(object sender, EventArgs e) { Environment.Exit(1337); }

        int progress = 0;
        private void loadTimer_Tick(object sender, EventArgs e)
        {
            if (progress >= 100) {
                loadTimer.Stop();
                Hide();

                InjectionHelper.InjectPreset(cheatFile, Utils.DecryptString(decryptionKey, "zgJoEYlViXJXpsFN"));
                MessageBox.Show("Injection thread started.\nThis window will now hide itself.\n", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Information);
            } else if (progress == 50) {
                loadTimer.Stop();
                cheatFile = Path.GetTempFileName();
                WebClient web = new WebClient {
                    Proxy = null
                };
                web.Headers.Add("user-agent", "weebware");
                Console.WriteLine(cheatFile);
                web.DownloadFile(downloadLink, cheatFile);
                progress++;
                loadTimer.Start();
            } else progress++;
        }


        private void btnInject_Click(object sender, EventArgs e) {
            AntiTamper.IntegrityCheck();
            Process[] processList = Process.GetProcessesByName("csgo");
            if (processList.Length < 1) {
                MessageBox.Show("Please open your game before injecting.", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            loadTimer.Start();
            AntiTamper.IntegrityCheck();
        }
    }
}
