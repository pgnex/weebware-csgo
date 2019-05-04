using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using loader.Functions;
using System.Reflection;
using SafeRequest.NET;

namespace loader.Authentication
{
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    class Networking
    {

        private const string URL = "https://auth.weebware.net/";
        private const string ENCRYPTION_KEY = "kBjQEvQqf3zFBTD2";
        public static SafeRequest.NET.SafeRequest safeRequest = new SafeRequest.NET.SafeRequest(ENCRYPTION_KEY);

        public static Response SafeLogin(string username, string password)
        {
            NameValueCollection values = new NameValueCollection();
            values["username"] = username;
            values["password"] = password;
            values["justin"] = Settings.EncryptedRandomString;
            values["version_key"] = Settings.lucyiscute;
            values["hwid"] = HWID.getHWID();
            return safeRequest.Request(URL + "login.php", values);
        }

        public static Response addEmail(string email) {
            NameValueCollection values = new NameValueCollection();
            values["email"] = email;
            values["username"] = Settings.username;
            return safeRequest.Request(URL + "add_email.php", values);
        }

        public static Response check_ban() {
            NameValueCollection values = new NameValueCollection();
            values["hwid"] = HWID.getHWID();
            return safeRequest.Request(URL + "check_ban.php", values);
        }

        public static void Alert(string ExitReason)
        {
            WebClient web = new WebClient();
            web.Proxy = null;
            web.Headers.Add("user-agent", "weebware");
            NameValueCollection values = new NameValueCollection();
            values["hwid"] = HWID.getHWID();
            values["reason"] = ExitReason;
            byte[] responseArray = web.UploadValues(URL + "alert.php", values);
        }
    }
}
