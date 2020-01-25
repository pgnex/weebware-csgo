using loader;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows.Forms;

[assembly: Obfuscation(Feature = "code control flow obfuscation", Exclude = false)]
namespace weebware_loader {
    static class Program {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            AntiTamper.Start();
            Utils.CreateFiles();
            Application.Run(new Login());
        }
    }
}
