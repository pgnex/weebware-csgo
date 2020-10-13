using loader;
using loader.Authentication;
using Newtonsoft.Json.Linq;
using nVJsXzXbiI69x8tvbPrd.QCRItun73F.Win32;
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.IO;
using System.Net;
using System.Reflection;
using System.Threading;
using System.Windows.Forms;
using weebware_loader.General;

using static weebware_loader.Custom_Components.CheatSelection;

namespace weebware_loader.Forms {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public partial class Main : Form {

        public Main(int px, int py, LoginResponse cheatData) {

            InitializeComponent();

            Location = new Point(px, py);

            PrivateFontCollection pfc = formstuff.createfont();

            lblWeebware.Font = new Font(pfc.Families[0], 12.5f, FontStyle.Bold);
            lblStatus.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            lblCheatStatus.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            lblWelcomeBack.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            btnInject.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            btnExit.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            lblCheatSelected.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            lblSessionExp.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);

            formstuff.movable(lblWeebware, this);
            formstuff.movable(pnlBackground, this);
            formstuff.movable(pbLogo, this);

            Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlBackground.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlSelContainer.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, pnlSelContainer.Width, pnlSelContainer.Height, 6, 6));
            pnlInfo.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, pnlInfo.Width, pnlInfo.Height, 6, 6));

            btnInject.FlatAppearance.BorderSize = 0;
            btnInject.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, btnInject.Width, btnInject.Height, 6, 6));

            btnExit.FlatAppearance.BorderSize = 0;
            btnExit.Region = Region.FromHrgn(Imports.CreateRoundRectRgn(0, 0, btnExit.Width, btnExit.Height, 6, 6));

            pbLogo.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbLogo.PixelOffsetMode = PixelOffsetMode.HighQuality;

            foreach (CheatInfo cheat in cheatData.cheatInfo) {
                cheat_names.Add(cheat.name);
                cheat_status.Add(cheat.status);
                cheat_downloads.Add(cheat.download);
                cheat_keys.Add(cheat.key);
                cheat_version.Add(cheat.version);
                cheat_enabled.Add(cheat.enabled);
            }
            cheat_name_labels.Add(lblCheatSelected);
            cheat_name_labels.Add(lblCheatStatus);
            InitSelections(pnlSelContainer, cheatData.cheatInfo.Count);

            lblCheatSelected.Text = string.Format("Currently Selected: {0}", cheat_names[cheat_selection_index]);
        }


        private void btnExit_Click(object sender, EventArgs e) { Environment.Exit(1337); }
        private void Main_FormClosed(object sender, FormClosedEventArgs e) { Environment.Exit(1337); }

        int session_max_expire = 300000;
        private void tmrExpire_Tick(object sender, EventArgs e) {
            lblSessionExp.Text = String.Format("Your session will expire in: {0} minutes", Math.Ceiling(session_max_expire / 60000f));
            session_max_expire -= 10000;

            if (session_max_expire <= 0) {
                lblSessionExp.Text = "Your current session has expired. Please restart launcher";
                lblSessionExp.ForeColor = Color.Red;
                tmrExpire.Stop();
            }
        }

        string cheatFile = string.Empty;
        private void InitInject() {
            WebClient web = new WebClient { Proxy = null };
            web.Headers.Add("user-agent", "weebware");
            cheatFile = Path.GetTempFileName();
            web.DownloadFile(cheat_downloads[cheat_selection_index], cheatFile);
            InjectionHelper.InjectPreset(cheatFile, Utils.DecryptString(cheat_keys[cheat_selection_index], "zgJoEYlViXJXpsFN"));
        }

        private void btnInject_Click(object sender, EventArgs e) {
            AntiTamper.IntegrityCheck();
            if (!cheat_enabled[cheat_selection_index]) {
                MessageBox.Show("Please select a valid cheat.\n", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            new Thread(InitInject).Start();
            MessageBox.Show("Injection thread started.\nThis window will now hide itself.\n", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Hide();
        }
    }
}