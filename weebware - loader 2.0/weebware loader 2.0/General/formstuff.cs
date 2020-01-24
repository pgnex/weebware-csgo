using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Media;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace weebware_loader.General {
    class formstuff {

        public static PrivateFontCollection createfont() {
            PrivateFontCollection pfc = new PrivateFontCollection();
            using (Stream fontstream = Assembly.GetExecutingAssembly().GetManifestResourceStream("weebware_loader.Resources.CircularStd.ttf")) {
                IntPtr data = Marshal.AllocCoTaskMem(Convert.ToInt32(fontstream.Length));
                int bufsize = BitConverter.ToInt32(BitConverter.GetBytes(fontstream.Length), 0);
                byte[] fontdata = new byte[bufsize];
                fontstream.Read(fontdata, 0, Convert.ToInt32(fontstream.Length));
                Marshal.Copy(fontdata, 0, data, Convert.ToInt32(fontstream.Length));
                pfc.AddMemoryFont(data, Convert.ToInt32(fontstream.Length));
                fontstream.Close();
                Marshal.FreeCoTaskMem(data);
            }

            return pfc;
        }

        public static void play_welcome(CheckBox checkBox) {

            try {
                checkBox.Checked = Properties.Settings.Default.disablesounds;
            } catch (Exception) {
                Properties.Settings.Default.disablesounds = false;
                Properties.Settings.Default.Save();
            }

            if (!Properties.Settings.Default.disablesounds) {
                Stream str = Properties.Resources.welcome;
                SoundPlayer snd = new SoundPlayer(str);
                snd.Play();
            }
        }


        // this is for making the form components movable since we arent using an actual window
        private static bool dragging = false;
        private static int xoff = 0, yoff = 0;

        private static void mousedown(Form f) {
            dragging = true;
            xoff = Cursor.Position.X - f.Location.X;
            yoff = Cursor.Position.Y - f.Location.Y;
        }

        private static void mousemove(Form f) {
            if (dragging) {
                f.Location = new Point(Cursor.Position.X - xoff, Cursor.Position.Y - yoff);
                f.Update();
            }
        }

        private static void mouseup() {
            dragging = false;
        }


        // create events.
        public static void movable(Label label, Form f) {
            label.MouseDown += (s, e) => mousedown(f);
            label.MouseMove += (s, e) => mousemove(f);
            label.MouseUp += (s, e) => mouseup();
        }

        public static void movable(Panel panel, Form f) {
            panel.MouseDown += (s, e) => mousedown(f);
            panel.MouseMove += (s, e) => mousemove(f);
            panel.MouseUp += (s, e) => mouseup();
        }

        public static void movable(PictureBox pbox, Form f) {
            pbox.MouseDown += (s, e) => mousedown(f);
            pbox.MouseMove += (s, e) => mousemove(f);
            pbox.MouseUp += (s, e) => mouseup();
        }

    }
}
