using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace weebware_loader.General {
    public class CheatInfo {
        public string name { get; set; }
        public string status { get; set; }
        public string download { get; set; }
        public string key { get; set; }
        public string version { get; set; }
        public bool enabled { get; set; }
        public string process { get; set; }
        public string modules { get; set; }
    }

    public class LoginResponse {
        public bool status { get; set; }
        public string message { get; set; }
        public IList<CheatInfo> cheatInfo { get; set; }
        public string authentication_key { get; set; }
    }
}
