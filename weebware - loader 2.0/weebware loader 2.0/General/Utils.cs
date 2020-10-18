using loader.Authentication;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace loader {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public static class Utils {

        public static long GetTime() {

            long currentTime = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            return currentTime;
        }

        public static string Reverse(string s) {
            AntiTamper.IntegrityCheck();
            char[] charArray = s.ToCharArray();
            Array.Reverse(charArray);
            AntiTamper.IntegrityCheck();
            return new string(charArray);
        }

        public static void RandomizeTitle(this System.Windows.Forms.Form f) {
            Random rnd = new Random();
            const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            f.Text = new string(Enumerable.Repeat(chars, rnd.Next(10, 20))
              .Select(s => s[rnd.Next(s.Length)]).ToArray());
        }

        private static Random random = new Random();
        public static string RandomString(int length) {
            const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            return new string(Enumerable.Repeat(chars, length)
              .Select(s => s[random.Next(s.Length)]).ToArray());
        }


        public static void ConnectionError() {
            MessageBox.Show("A connection error has occurred.", "weebware", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }

    // class for startup..
    public static class Setup {

        private class Path {
            public static string directory_path;
            public static List<string> req_files = new List<string>();
        }

        private static string api_path = "https://api.weebware.net/dependencies/";
        private static string appdata_path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "/weebware";
        private static WebClient web = new WebClient();

        public static void Init() {
            // verify we have connection to api
            if (!Networking.VerifyConnection()) {
                MessageBox.Show("Problem connecting to API. Please try again later, or contact support.");
                Environment.Exit(1337);
                return;
            }

            if (!DependanciesExist())
                Install();
        }

        private static void Install() {
            // create missing directories in appdata..
            string directories = web.DownloadString(api_path + "directories.txt");
            foreach (string path in directories.Split(','))
                if (!Directory.Exists(appdata_path + path))
                    Directory.CreateDirectory(appdata_path + path);
            
            // create missing files in appdata..
            string files = web.DownloadString(api_path + "files.txt");
            foreach (string file in files.Split(','))
                if (!File.Exists(appdata_path + file))
                    web.DownloadFile(api_path + file, appdata_path + file);
        }

        private static bool DependanciesExist() {
            return Directory.Exists(appdata_path);
        }


    }


    // crypto stuff
    public static class Encryption {
        private const string initVector = "rtUAIkm859Vxvmrg";
        private const int keysize = 256;

        public static string EncryptString(string Ky, string Iv, string encryptText) {
            AntiTamper.IntegrityCheck();
            var sToEncrypt = encryptText;

            var myRijndael = new RijndaelManaged() {
                Padding = PaddingMode.Zeros,
                Mode = CipherMode.CBC,
                KeySize = 256,
                BlockSize = 256
            };

            var key = Encoding.ASCII.GetBytes(Ky);
            var IV = Encoding.ASCII.GetBytes(Iv);
            var encryptor = myRijndael.CreateEncryptor(key, IV);
            var msEncrypt = new MemoryStream();
            var csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write);
            var toEncrypt = Encoding.ASCII.GetBytes(sToEncrypt);
            csEncrypt.Write(toEncrypt, 0, toEncrypt.Length);
            csEncrypt.FlushFinalBlock();
            var encrypted = msEncrypt.ToArray();
            AntiTamper.IntegrityCheck();
            return (Convert.ToBase64String(encrypted));
        }

        public static string DecryptString(string cipherText, string passPhrase) {
            AntiTamper.IntegrityCheck();
            byte[] initVectorBytes = Encoding.UTF8.GetBytes(initVector);
            byte[] cipherTextBytes = Convert.FromBase64String(cipherText);
            PasswordDeriveBytes password = new PasswordDeriveBytes(passPhrase, null);
            byte[] keyBytes = password.GetBytes(keysize / 8);
            RijndaelManaged symmetricKey = new RijndaelManaged();
            symmetricKey.Mode = CipherMode.CBC;
            ICryptoTransform decryptor = symmetricKey.CreateDecryptor(keyBytes, initVectorBytes);
            MemoryStream memoryStream = new MemoryStream(cipherTextBytes);
            CryptoStream cryptoStream = new CryptoStream(memoryStream, decryptor, CryptoStreamMode.Read);
            byte[] plainTextBytes = new byte[cipherTextBytes.Length];
            int decryptedByteCount = cryptoStream.Read(plainTextBytes, 0, plainTextBytes.Length);
            memoryStream.Close();
            cryptoStream.Close();
            AntiTamper.IntegrityCheck();
            return Encoding.UTF8.GetString(plainTextBytes, 0, decryptedByteCount);
        }

        public static byte[] decryptToBytes(string filePath, string password) {
            try {
                AntiTamper.IntegrityCheck();
                byte[] passwordBytes = System.Text.Encoding.UTF8.GetBytes(password);
                byte[] salt = new byte[32];
                FileStream fsCrypt = new FileStream(filePath, FileMode.Open);
                fsCrypt.Read(salt, 0, salt.Length);
                RijndaelManaged AES = new RijndaelManaged();
                AES.KeySize = 256;
                AES.BlockSize = 128;
                var key = new Rfc2898DeriveBytes(passwordBytes, salt, 50000);
                AES.Key = key.GetBytes(AES.KeySize / 8);
                AES.IV = key.GetBytes(AES.BlockSize / 8);
                AES.Padding = PaddingMode.PKCS7;
                AES.Mode = CipherMode.CFB;

                CryptoStream cs = new CryptoStream(fsCrypt, AES.CreateDecryptor(), CryptoStreamMode.Read);
                MemoryStream ms = new MemoryStream();
                int read;
                byte[] buffer = new byte[1048576];
                while ((read = cs.Read(buffer, 0, buffer.Length)) > 0) ms.Write(buffer, 0, read);
                fsCrypt.Close();
                ms.Close();
                AntiTamper.IntegrityCheck();
                return ms.ToArray();
            } catch (Exception) {
                MessageBox.Show("Unable to decrypt.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return null;
        }
    }
}
