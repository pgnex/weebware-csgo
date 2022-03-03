using loader;
using System;
using System.Reflection;
using System.Threading;
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
           // AntiTamper.Start();
            Setup.Init();
            Application.Run(new Login());
        }
    }
}
