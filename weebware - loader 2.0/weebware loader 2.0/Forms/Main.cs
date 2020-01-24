using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using weebware_loader.Custom_Components;
using weebware_loader.General;

namespace weebware_loader.Forms {

    public partial class Main : Form {

        public Main(int px, int py) {

            InitializeComponent();

            Location = new Point(px, py);

            PrivateFontCollection pfc = formstuff.createfont();

            lblWeebware.Font = new Font(pfc.Families[0], 12.5f, FontStyle.Bold);
            lblStatus.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            lblWelcomeBack.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            btnInject.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);
            btnExit.Font = new Font(pfc.Families[0], 9, FontStyle.Bold);

            lblCheatSelected.Text = String.Format("{0} Selected", CheatSelection.cheat_names[CheatSelection.cheat_selection_index]);

            formstuff.movable(lblWeebware, this);
            formstuff.movable(pnlBackground, this);
            formstuff.movable(pbLogo, this);

            Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlBackground.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlSelContainer.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, pnlSelContainer.Width, pnlSelContainer.Height, 6, 6));
            pnlInfo.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, pnlInfo.Width, pnlInfo.Height, 6, 6));

            btnInject.FlatAppearance.BorderSize = 0;
            btnInject.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, btnInject.Width, btnInject.Height, 6, 6));

            btnExit.FlatAppearance.BorderSize = 0;
            btnExit.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, btnExit.Width, btnExit.Height, 6, 6));

            pbLogo.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbLogo.PixelOffsetMode = PixelOffsetMode.HighQuality;

            CheatSelection.InitSelections(pnlSelContainer, lblCheatSelected, 2);
        }


        private void btnExit_Click(object sender, EventArgs e) { Environment.Exit(1337); }
        private void Main_FormClosed(object sender, FormClosedEventArgs e) { Environment.Exit(1337); }

        int session_max_expire = 300000;
        private void tmrExpire_Tick(object sender, EventArgs e) {
            lblSessionExp.Text = String.Format("Your session will expire in: {0} minutes", Math.Ceiling(session_max_expire / 60000f));
            session_max_expire -= 10000;
        }
    }
}