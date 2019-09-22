using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.IO;
using System.Text.RegularExpressions;

namespace dependency_downloader {

    class Program {

        static string[] archives = new string[] {
            "https://f002.backblazeb2.com/file/justinooo-upload/ShareX/reinakousaka.zip"
        };

        static void Main(string[] args) {

            string installPath = SteamUtils.GameInstallPath(730);
            string extractPath = Path.Combine(installPath, "csgo");
            if (!Directory.Exists(extractPath)) {
                Console.WriteLine("Failed to locate \"csgo\" installation directory.");
                Console.ReadKey();
                return;
            }

            foreach (string archive in archives) {
                string file = Path.GetFileName(archive);
                Console.WriteLine("Downloading " + file + "...");
                Dependency dependency = new Dependency(archive, extractPath) {
                    Password = "weebware"
                };
                dependency.Download();
            }

            Console.ReadKey();

        }

    }

}