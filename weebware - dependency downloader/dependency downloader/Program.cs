using Ionic.Zip;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using Newtonsoft.Json;

namespace dependency_downloader {

    class Program {

        static void Main(string[] args) {

            List<string> archives = null;
            using (WebClient web = new WebClient()) {
                string archivesRaw = web.DownloadString("https://pastebin.com/raw/E4iLmrFG");
                archives = JsonConvert.DeserializeObject<List<string>>(archivesRaw);
            }

            string installPath = SteamUtils.GameInstallPath(730);
            string extractPath = Path.Combine(installPath ?? string.Empty, "csgo");
            if (installPath == null || !Directory.Exists(extractPath)) {
                Console.WriteLine("Failed to locate \"csgo\" installation directory.");
                Console.ReadKey();
                return;
            }

            foreach (string archive in archives) {
                Dependency dependency = new Dependency(archive, extractPath) {
                    Password = "justinooo",
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

        private static void DownloadCompleted(Dependency sender) {
            Console.WriteLine("downloaded: " + Path.GetFileName(sender.URL));
        }

        private static void ExtractionCompleted(Dependency sender) {
            Console.WriteLine("extracted: " + Path.GetFileName(sender.URL));
        }

        private static void DownloadProgressChanged(Dependency sender, DownloadProgressChangedEventArgs args) { }
        private static void ExtractionProgressChanged(Dependency sender, ExtractProgressEventArgs args) { }

    }

}