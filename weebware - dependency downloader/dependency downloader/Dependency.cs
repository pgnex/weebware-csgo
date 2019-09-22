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

        public DownloadProgressChangedEventHandler DownloadProgressChanged = null;
        public AsyncCompletedEventHandler DownloadCompleted = null;

        public Dependency(string url, string path) {
            this.url = url;
            this.path = path;
        }

        public void Download(bool extractOnCompletion = true) {
            archivePath = Path.GetTempFileName();
            using (WebClient web = new WebClient()) {

                if (DownloadProgressChanged != null)
                    web.DownloadProgressChanged += DownloadProgressChanged;

                if (DownloadCompleted != null)
                    web.DownloadFileCompleted += DownloadCompleted;

                if (extractOnCompletion)
                    web.DownloadFileCompleted += ExtractFiles;

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
        }

    }

}
