using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using SafeRequest.NET;
using System.Drawing;
using System.Windows.Forms;

namespace loader.Authentication {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    class Networking {

        private const string URL = "https://api.weebware.net";
        private const string ENCRYPTION_KEY = "kBjQEvQqf3zFBTD2";
        private const string VERSION_STRING = "4.0";

        public static SafeRequest.NET.SafeRequest safeRequest = new SafeRequest.NET.SafeRequest(ENCRYPTION_KEY);

        public static Response SafeLogin(string username, string password) {
            NameValueCollection values = new NameValueCollection();
            values["username"] = username;
            values["password"] = password;
            values["hwid"] = HWID.getHWID();
            values["loader_version"] = VERSION_STRING;
            values["login"] = "yeet";
            return safeRequest.Request(URL, values);
        }

        public static Response SafeAlert(string ExitReason) {
            NameValueCollection values = new NameValueCollection();
            values["reason"] = ExitReason;
            values["alert"] = "weeeeeeee";
            return safeRequest.Request(URL, values);
        }

    }
}
