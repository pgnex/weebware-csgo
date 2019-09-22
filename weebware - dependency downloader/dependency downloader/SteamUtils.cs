using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace dependency_downloader {

    public static class SteamUtils {

        public static string SteamInstallPath() {

            object installPath = Registry.GetValue(
                @"HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Steam",
                "InstallPath",
                null
            );

            return (installPath != null) ?
                Convert.ToString(installPath) : null;

        }

        public static List<string> GetLibraryFolders() {

            List<string> folders = new List<string>();
            folders.Add(SteamInstallPath());

            string vdfPath = Path.Combine(folders[0], @"steamapps\libraryfolders.vdf");
            string vdfData = File.ReadAllText(vdfPath);

            MatchCollection folderMatches = Regex.Matches(vdfData, "\\n\\s*\"\\d+\"\\s+\"(.*?)\"");
            foreach (Match folderMatch in folderMatches) {
                Group folder = folderMatch.Groups[1];
                folders.Add(folder.Value.Replace("\\\\", "\\"));
            }

            return folders;

        }

        public static string GameInstallPath(int appID) {

            List<string> folders = GetLibraryFolders();
            string relativeManifestPath = string.Format(@"steamapps\appmanifest_{0}.acf", appID);

            foreach (string folder in folders) {

                string manifestPath = Path.Combine(folder, relativeManifestPath);
                if (!File.Exists(manifestPath))
                    continue;

                string manifestData = File.ReadAllText(manifestPath);
                Match installDirMatch = Regex.Match(manifestData, "\\s*\"installdir\"\\s+\"(.*?)\"", RegexOptions.Singleline);
                string relativeInstallDir = string.Format(@"steamapps\common\{0}", installDirMatch.Groups[1].Value);

                string installPath = Path.Combine(folder, relativeInstallDir);
                return installPath;

            }

            return null;

        }

    }

}
