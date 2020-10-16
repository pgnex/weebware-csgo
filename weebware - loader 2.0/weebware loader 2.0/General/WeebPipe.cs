using loader.Authentication;
using SafeRequest;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace weebware_loader.General {
    public class WeebPipe {

        // Creates a PipeSecurity that allows users read/write access
        private static PipeSecurity CreateSystemIOPipeSecurity() {
            PipeSecurity pipeSecurity = new PipeSecurity();

            var id = new SecurityIdentifier(WellKnownSidType.AuthenticatedUserSid, null);

            // Allow Everyone read and write access to the pipe. 
            pipeSecurity.SetAccessRule(new PipeAccessRule(id, PipeAccessRights.ReadWrite, AccessControlType.Allow));

            return pipeSecurity;
        }


        private static void PipeWrite(string msg, NamedPipeServerStream authPipe) {
            StreamWriter writer = new StreamWriter(authPipe);
            writer.Write(msg);
            writer.Write((char)0);
            writer.Flush();
            authPipe.WaitForPipeDrain();
        }

        private static string PipeRead(NamedPipeServerStream authPipe) {
            StreamReader streamReader = new StreamReader(authPipe);
            return streamReader.ReadLine();
        }

        private static NamedPipeServerStream CreatePipe() {
            PipeSecurity pipeSecurity = CreateSystemIOPipeSecurity();
            return new NamedPipeServerStream("weeb-pipe", PipeDirection.InOut, 1, PipeTransmissionMode.Byte, PipeOptions.Asynchronous, 0x4000, 0x400, pipeSecurity, HandleInheritability.Inheritable);
        }


        public static void Authenticate() {
            NamedPipeServerStream authPipe = CreatePipe();
            authPipe.WaitForConnection();

            string raw = PipeRead(authPipe);
            if (raw.Length > 0) {
                Response response = Networking.SafeAuth(raw);
                PipeWrite(response.message, authPipe);
            }
            authPipe.Dispose();
        }
    }
}
