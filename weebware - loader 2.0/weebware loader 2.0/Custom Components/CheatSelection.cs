using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms;
using weebware_loader.Forms;
using weebware_loader.General;

namespace weebware_loader.Custom_Components {
    class CheatSelection {

        public static Bitmap[] icons = { Properties.Resources.csgo_icon, Properties.Resources.filler_icon };
        public static string[] cheat_names = { "Counter-Strike: Global Offensive", "Unavailable" };
        public static int cheat_selection_index = 0;
        public static Panel[] panels = { new Panel(), new Panel() };
        public static Label lblSelectedCheat = null;

        public static void InitSelections(Panel parent, Label lblSelected, int count) {
            PrivateFontCollection pfc = formstuff.createfont();

            if (lblSelectedCheat == null)
                lblSelectedCheat = lblSelected;

            for (int i = 0; i < count; i++) {
                Panel p = new Panel();
                p = CreateSelectionPanel(
                    new Point(0, i),
                    new Size(parent.Width, parent.Height / 2),
                    (i == 0)
                );

                CPictureBox pbIcon = new CPictureBox();
                pbIcon = CreateSelectionPb(
                    p,
                    icons[i]
                );
                p.Controls.Add(pbIcon);

                Label label = new Label();
                label = CreateSelectionLabel(
                    p,
                    new Font(pfc.Families[0], 9, FontStyle.Bold),
                    cheat_names[i],
                    pbIcon.Location.X,
                    pbIcon.Width
                );

                switch (i) {
                    case 0:
                        p.Click += cheatSelection1;
                        pbIcon.Click += cheatSelection1;
                        label.Click += cheatSelection1;
                        break;
                    case 1:
                        p.Click += cheatSelection2;
                        pbIcon.Click += cheatSelection2;
                        label.Click += cheatSelection2;
                        break;
                }
                panels[i] = p;
                parent.Controls.Add(p);
            }
        }

        private static Panel CreateSelectionPanel(Point start, Size size, bool active) {      

            Panel panel = new Panel();
            panel.Name = "pnl";
            panel.BorderStyle = BorderStyle.None;
            panel.Location = new Point(start.X, start.Y * size.Height);
            panel.Size = new Size(size.Width, size.Height);
            panel.BackColor = active ? Color.FromArgb(54, 54, 54) : Color.FromArgb(34, 34, 34);
            panel.ForeColor = Color.Black;

            return panel;
        }

        private static CPictureBox CreateSelectionPb(Panel parent, Image icon) {
            CPictureBox pbIcon = new CPictureBox();
            pbIcon.Name = "pb";
            pbIcon.Image = icon;
            pbIcon.BackColor = Color.FromArgb(54, 54, 54);
            pbIcon.SizeMode = PictureBoxSizeMode.StretchImage;
            pbIcon.Size = new Size(28, 28);
            pbIcon.Location = new Point(12, (parent.Height - pbIcon.Height) / 2);
            pbIcon.Region = Region.FromHrgn(imports.CreateRoundRectRgn(0, 0, pbIcon.Width, pbIcon.Height, 3, 3));
            pbIcon.InterpolationMode = InterpolationMode.HighQualityBilinear;
            pbIcon.PixelOffsetMode = PixelOffsetMode.HighQuality;
            parent.Controls.Add(pbIcon);

            return pbIcon;
        }

        private static Label CreateSelectionLabel(Panel parent, Font font, string text, int pbIconX, int pbIconWidth) {
            Label label = new Label();
            label.Name = "lbl";
            label.AutoSize = true;
            label.Font = font;
            label.Text = text;
            label.Location = new Point((pbIconX + pbIconWidth) + 8, ((parent.Height - label.Height) / 2) + 3);
            label.ForeColor = Color.FromArgb(137, 137, 137);
            label.BackColor = Color.Transparent;
            parent.Controls.Add(label);

            return label;
        }

        private static void cheatSelection1(object sender, EventArgs e) {
            cheat_selection_index = 0;
            lblSelectedCheat.Text = String.Format("{0} Selected", cheat_names[cheat_selection_index]);
            panels[cheat_selection_index + 1].BackColor = Color.FromArgb(34, 34, 34);
            panels[cheat_selection_index].BackColor = Color.FromArgb(54, 54, 54);
        }

        private static void cheatSelection2(object sender, EventArgs e) {
            cheat_selection_index = 1;
            lblSelectedCheat.Text = String.Format("{0} Selected", cheat_names[cheat_selection_index]);
            panels[cheat_selection_index - 1].BackColor = Color.FromArgb(34, 34, 34);
            panels[cheat_selection_index].BackColor = Color.FromArgb(54, 54, 54);
        }
    }
}
