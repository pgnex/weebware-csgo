using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Management;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using loader.Functions;

namespace loader
{
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    public class HWID
    {

        private static string GenerateSHA512String(string inputString)
        {
            AntiTamper.IntegrityCheck();
            SHA512 sha512 = SHA512Managed.Create();

            byte[] bytes = Encoding.UTF8.GetBytes(inputString);
            byte[] hash = sha512.ComputeHash(bytes);
            StringBuilder sb = new StringBuilder();

            for (int i = 0; i <= hash.Length - 1; i++) sb.Append(hash[i].ToString("X2"));
            AntiTamper.IntegrityCheck();
            return sb.ToString();
        }

        private static string cpuID()
        {
            AntiTamper.IntegrityCheck();
            string cpuInfo = "";
            ManagementClass managClass = new ManagementClass("win32_processor");

            ManagementObjectCollection managCollec = managClass.GetInstances();

            foreach (ManagementObject managObj in managCollec)
            {
                cpuInfo = managObj.Properties["Revision"].Value.ToString() + managObj.Properties["processorID"].Value.ToString();
                break;
            }
            AntiTamper.IntegrityCheck();
            return cpuInfo;
        }


        public static string getHWID()
        {
            AntiTamper.IntegrityCheck();
            string original;
            try
            {
                original = cpuID() + Environment.MachineName;
            }
            catch (Exception)
            {
                original = Environment.MachineName;
            }
            AntiTamper.IntegrityCheck();
            return Utils.EncryptString(Settings.HWIDkey, "QqxG1lLzeIliTzrBCbJ3C5fG6rwJOubM", original);
        }
    }
}
