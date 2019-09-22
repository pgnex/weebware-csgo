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
            get { return _url; }
        }

        public string Password { get; set; }
        public string ExtractToPath { get; internal set; }
        public string ArchivePath { get; internal set; }
        public bool DeleteAfterExtract { get; set; } = true;

        public delegate void DownloadCompletedHandler(Dependency sender);
        public DownloadCompletedHandler DownloadCompleted = null;
            
        public delegate void DownloadProgressHandler(Dependency sender, DownloadProgressChangedEventArgs args);
        public DownloadProgressHandler DownloadProgressChanged = null;

        public delegate void ExtractionCompletedHandler(Dependency sender);
        public ExtractionCompletedHandler ExtractionCompleted = null;

        public delegate void ExtractionProgressHandler(Dependency sender, ExtractProgressEventArgs args);
        public ExtractionProgressHandler ExtractionProgressChanged = null;

        public Dependency(string url, string extractToPath) {
            URL = url;
            ExtractToPath = extractToPath;
        }

        public void Download(bool extractOnCompletion = true) {
            ArchivePath = Path.GetTempFileName();
            using (WebClient web = new WebClient()) {

                web.DownloadFileCompleted += (s, e) => DownloadCompleted?.Invoke(this);
                web.DownloadProgressChanged += (s, e) => DownloadProgressChanged?.Invoke(this, e);

                if (extractOnCompletion)
                    web.DownloadFileCompleted += ExtractFiles;

                web.DownloadFileAsync(new Uri(_url), ArchivePath);

            }
        }

        private void ExtractFiles(object sender, AsyncCompletedEventArgs args) {

            using (ZipFile archive = new ZipFile(ArchivePath)) {
                if (Password != null) {
                    archive.Password = Password;
                    archive.Encryption = EncryptionAlgorithm.PkzipWeak;
                }
                archive.ExtractProgress += (s, e) => ExtractionProgressChanged?.Invoke(this, e);
                archive.ExtractAll(ExtractToPath, ExtractExistingFileAction.OverwriteSilently);
            }

            if (DeleteAfterExtract)
                try { File.Delete(ArchivePath); }
                catch (Exception) { }

            ExtractionCompleted?.Invoke(this);

        }
    }

}
