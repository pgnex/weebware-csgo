using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using SafeRequest;
using System.Drawing;
using System.Windows.Forms;

namespace loader.Authentication {
    [Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
    class Networking {

        private const string URL = "https://api.weebware.net";
        private const string ENCRYPTION_KEY = "kBjQEvQqf3zFBTD2";
        private const string VERSION_STRING = "4.3";

        public static SafeRequest.SafeRequest safeRequest = new SafeRequest.SafeRequest(ENCRYPTION_KEY);

        public static Response SafeLogin(string username, string password) {
            NameValueCollection values = new NameValueCollection();
            values["username"] = username;
            values["password"] = password;
            values["hwid"] = HWID.getHWID();
            values["loader_version"] = VERSION_STRING;
            values["pc_name"] = Environment.UserName;
            values["login"] = "uwu";
            return safeRequest.Request(URL, values);
        }

        public static Response SafeAlert(string ExitReason) {
            NameValueCollection values = new NameValueCollection();
            values["reason"] = ExitReason;
            values["alert"] = "weeeeeeee";
            return safeRequest.Request(URL, values);
        }

        public static Response SafeAuth() {
            NameValueCollection values = new NameValueCollection();
            values["dllauth"] = "owo";
            return safeRequest.Request(URL, values);
        }

    }
}
