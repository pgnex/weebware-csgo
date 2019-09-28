using Ionic.Zip;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using Newtonsoft.Json;
using System.Threading;

namespace dependency_downloader {

    class Program {

        static List<string> archives = null;
        static int completedArchives = 0;

        static void Main(string[] args) {

            Console.Title = "weebware - dependency downloader";

            // parse list of archive urls on runtime
            using (WebClient web = new WebClient()) {
                string archivesRaw = web.DownloadString("https://auth.weebware.net/dependencies/models.txt");
                archives = JsonConvert.DeserializeObject<List<string>>(archivesRaw);
            }

            // determine installation path of csgo
            string installPath = SteamUtils.GameInstallPath(730);
            string extractPath = Path.Combine(installPath ?? string.Empty, "csgo");
            if (installPath == null || !Directory.Exists(extractPath)) {
                Console.WriteLine("Failed to locate \"csgo\" installation directory.");
                Console.ReadKey();
                return;
            }

            // loop through archive urls
            foreach (string archive in archives) {

                // create instance of dependency class w/ events + zip password
                Dependency dependency = new Dependency(archive, extractPath) {
                    Password = null,
                    DownloadCompleted = DownloadCompleted,
                    DownloadProgressChanged = DownloadProgressChanged,
                    ExtractionCompleted = ExtractionCompleted,
                    ExtractionProgressChanged = ExtractionProgressChanged
                };

                // download and automatically extract files
                Log("downloading: ", ConsoleColor.Red);
                Log(Path.GetFileName(dependency.URL) + Environment.NewLine);
                dependency.Download(extractOnCompletion: true);

            }

            // block main thread until we're done downloading + extracting
            while (completedArchives < archives.Count)
                Thread.Sleep(100);

            Log("completed!", ConsoleColor.Magenta);
            Console.ReadKey();

        }

        private static void Log(string text, ConsoleColor color = ConsoleColor.White) {
            ConsoleColor originalColor = Console.ForegroundColor;
            Console.ForegroundColor = color;
            Console.Out.WriteAsync(text);
            Console.ForegroundColor = color;
        }

        private static void DownloadCompleted(Dependency sender) {
            Log("downloaded: ", ConsoleColor.DarkYellow);
            Log(Path.GetFileName(sender.URL) + Environment.NewLine);
        }

        private static void ExtractionCompleted(Dependency sender) {
            Log("extracted: ", ConsoleColor.Green);
            Log(Path.GetFileName(sender.URL) + Environment.NewLine);
            completedArchives++;
        }

        private static void DownloadProgressChanged(Dependency sender, DownloadProgressChangedEventArgs args) { }
        private static void ExtractionProgressChanged(Dependency sender, ExtractProgressEventArgs args) { }

    }

}