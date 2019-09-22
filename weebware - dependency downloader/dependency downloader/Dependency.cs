using System;
using System.Net;
using System.ComponentModel;
using Ionic.Zip;
using System.IO;

namespace dependency_downloader {

    class Dependency {

        public string Password = null;

        private string url;
        private string path;
        private string archivePath;

        public delegate void DownloadProgressChangedHandler(Dependency sender, DownloadProgressChangedEventArgs args);
        public DownloadProgressChangedHandler DownloadProgressChanged = null;

        public delegate void DownloadCompletedHandler(Dependency sender);
        public DownloadCompletedHandler DownloadCompleted = null;

        public Dependency(string url, string path) {
            this.url = url;
            this.path = path;
        }

        public void Download(bool extractOnCompletion = true) {
            archivePath = Path.GetTempFileName();
            using (WebClient web = new WebClient()) {

                if (extractOnCompletion)
                    web.DownloadFileCompleted += ExtractFiles;

                web.DownloadFileCompleted += (s, e) => DownloadCompleted?.Invoke(this);
                web.DownloadProgressChanged += (s, e) => DownloadProgressChanged?.Invoke(this, e);

                web.DownloadFileAsync(new Uri(url), archivePath);

            }
        }

        private void ExtractFiles(object sender, AsyncCompletedEventArgs e) {
            using (ZipFile archive = new ZipFile(archivePath)) {

                if (Password != null) {
                    archive.Password = Password;
                    archive.Encryption = EncryptionAlgorithm.PkzipWeak;
                }

                archive.ExtractAll(path, ExtractExistingFileAction.OverwriteSilently);

            }
            File.Delete(archivePath);
        }

    }

}
