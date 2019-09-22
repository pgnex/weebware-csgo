using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Text.RegularExpressions;

namespace dependency_downloader {

    class Program {

        /*
        TODO
        - Add other models
        - Create custom events in Dependency class (completed/progress changed)
        - Report download/extraction progress to console
        - Wait for previous download to complete (use events to trigger next download)
        */

        static string[] archives = new string[] {
            "https://f002.backblazeb2.com/file/justinooo-upload/ShareX/reinakousaka.zip"
        };

        static void Main(string[] args) {

            string installPath = SteamUtils.GameInstallPath(730);
            string extractPath = Path.Combine(installPath ?? string.Empty, "csgo");
            if (installPath == null || !Directory.Exists(extractPath)) {
                Console.WriteLine("Failed to locate \"csgo\" installation directory.");
                Console.ReadKey();
                return;
            }

            foreach (string archive in archives) {
                string file = Path.GetFileName(archive);
                Console.WriteLine("Downloading " + file + "...");
                Dependency dependency = new Dependency(archive, extractPath) {
                    Password = "weebware",
                    DownloadProgressChanged = DownloadProgressChanged,
                    DownloadCompleted = DownloadCompleted
                };
                dependency.Download();
            }

            Console.ReadKey();

        }

        private static void DownloadProgressChanged(Dependency sender, DownloadProgressChangedEventArgs args) {
            
        }

        private static void DownloadCompleted(Dependency sender) {

        }

    }

}