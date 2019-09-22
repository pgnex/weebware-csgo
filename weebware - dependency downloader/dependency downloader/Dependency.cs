using System;
using System.Net;
using System.ComponentModel;
using Ionic.Zip;
using System.IO;

namespace dependency_downloader {

    class Dependency {

        private string _url;
        public string URL {
            set {
                if (Uri.IsWellFormedUriString(value, UriKind.Absolute))
                    _url = value;
                else
                    throw new FormatException("Invalid URI format.");
            }
        }

        public string Password { get; set; }
        public string ExtractToPath { get; internal set; }
        public string ArchivePath { get; internal set; }

        public delegate void DownloadProgressChangedHandler(Dependency sender, DownloadProgressChangedEventArgs args);
        public DownloadProgressChangedHandler DownloadProgressChanged = null;

        public delegate void DownloadCompletedHandler(Dependency sender);
        public DownloadCompletedHandler DownloadCompleted = null;

        public Dependency(string url, string extractToPath) {
            URL = url;
            ExtractToPath = extractToPath;
        }

        public void Download(bool extractOnCompletion = true) {
            ArchivePath = Path.GetTempFileName();
            using (WebClient web = new WebClient()) {

                if (extractOnCompletion)
                    web.DownloadFileCompleted += ExtractFiles;

                web.DownloadFileCompleted += (s, e) => DownloadCompleted?.Invoke(this);
                web.DownloadProgressChanged += (s, e) => DownloadProgressChanged?.Invoke(this, e);
                web.DownloadFileAsync(new Uri(_url), ArchivePath);

            }
        }

        private void ExtractFiles(object sender, AsyncCompletedEventArgs e) {
            using (ZipFile archive = new ZipFile(ArchivePath)) {
                if (Password != null) {
                    archive.Password = Password;
                    archive.Encryption = EncryptionAlgorithm.PkzipWeak;
                }
                archive.ExtractAll(ExtractToPath, ExtractExistingFileAction.OverwriteSilently);
            }
            File.Delete(ArchivePath);
        }

    }

}
