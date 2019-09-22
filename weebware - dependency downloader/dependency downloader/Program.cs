using Ionic.Zip;
using System;
using System.IO;
using System.Net;

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
                Dependency dependency = new Dependency(archive, extractPath) {
                    Password = "weebware",
                    DownloadCompleted = DownloadCompleted,
                    DownloadProgressChanged = DownloadProgressChanged,
                    ExtractionCompleted = ExtractionCompleted,
                    ExtractionProgressChanged = ExtractionProgressChanged
                };
                Console.WriteLine("Downloading " + Path.GetFileName(dependency.URL) + "...");
                dependency.Download();
            }

            Console.ReadKey();

        }

        private static void DownloadProgressChanged(Dependency sender, DownloadProgressChangedEventArgs args) {
            
        }

        private static void DownloadCompleted(Dependency sender) {
            Console.WriteLine("downloaded: " + Path.GetFileName(sender.URL));
        }

        private static void ExtractionProgressChanged(Dependency sender, ExtractProgressEventArgs args) {

        }

        private static void ExtractionCompleted(Dependency sender) {
            Console.WriteLine("extracted: " + Path.GetFileName(sender.URL));
        }

    }

}