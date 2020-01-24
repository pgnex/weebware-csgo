using System;
using System.Drawing;
using System.Windows.Forms;

namespace weebware_loader.General {
    public partial class PlaceholderTextbox : TextBox {
        private static bool TextReset = false;
        public string placeHolder = "Placeholder";

        public bool usePasswordChar { get; set; } = false;

        public string PlaceHolder {
            get { return placeHolder; }
            set {
                placeHolder = value;
                KeyPress += new KeyPressEventHandler(PlaceholderTextbox_KeyPress);
                Enter += new EventHandler(PlaceholderTextbox_Enter);
                Leave += new EventHandler(PlaceholderTextbox_Leave);
                Click += new EventHandler(PlaceholderTextbox_Click);
                MouseEnter += new EventHandler(PlaceholderTextbox_MouseEnter);
                MouseLeave += new EventHandler(PlaceholderTextbox_MouseLeave);
            }
        }


        private void PlaceholderTextbox_KeyPress(object sender, KeyPressEventArgs e) {
            if (TextLength <= 1 && e.KeyChar == '\b') {

                if (usePasswordChar)
                    PasswordChar = '\0';

                Text = PlaceHolder;
                ForeColor = Color.FromArgb(117, 117, 117);
                TextReset = false;
                return;
            }

            if (TextReset) {

                if (Text == "")
                    return;

                if (Text == PlaceHolder)
                    TextReset = false;

                return;
            }

            if (!TextReset) {

                if (e.KeyChar == '\b')
                    return;

                if (usePasswordChar)
                    PasswordChar = '*';

                Text = "";
                ForeColor = Color.FromArgb(167, 167, 167);
                TextReset = true;

            }
        }

        private void PlaceholderTextbox_Enter(object sender, EventArgs e) {
            if (Text == PlaceHolder)
                Select(0, 0);
        }

        private void PlaceholderTextbox_Leave(object sender, EventArgs e) {
            if (Text == "" || Text == PlaceHolder) {
                Text = PlaceHolder;
                ForeColor = Color.FromArgb(117, 117, 117);
            }
        }

        private void PlaceholderTextbox_Click(object sender, EventArgs e) {
            if (Text == PlaceHolder)
                Select(0, 0);
        }

        private void PlaceholderTextbox_MouseEnter(object sender, EventArgs e) {
            Cursor = Cursors.Arrow;

        }

        private void PlaceholderTextbox_MouseLeave(object sender, EventArgs e) {
            Cursor = Cursors.Default;

        }

    }
}