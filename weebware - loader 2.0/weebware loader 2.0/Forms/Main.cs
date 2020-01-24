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
using weebware_loader_2._0.General;

namespace weebware_loader_2._0.Forms {
    public partial class Main : Form {

        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidthEllipse, int nHeightEllipse);

        public Main(int px, int py) {

            InitializeComponent();

            formstuff.movable(lblWeebware, this);
            formstuff.movable(pnlBackground, this);
            formstuff.movable(pbLogo, this);

            Location = new Point(px, py);

            Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlBackground.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 6, 6));
            pnlSelContainer.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, pnlSelContainer.Width, pnlSelContainer.Height, 6, 6));
            pnlInfo.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, pnlInfo.Width, pnlInfo.Height, 6, 6));

            btnInject.FlatAppearance.BorderSize = 0;
            btnInject.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, btnInject.Width, btnInject.Height, 6, 6));

            btnExit.FlatAppearance.BorderSize = 0;
            btnExit.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, btnExit.Width, btnExit.Height, 6, 6));

            pbLogo.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbLogo.PixelOffsetMode = PixelOffsetMode.HighQuality;
        }

        private void btnExit_Click(object sender, EventArgs e) { Environment.Exit(1337); }
    }
}
