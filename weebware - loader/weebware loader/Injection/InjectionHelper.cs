using nVJsXzXbiI69x8tvbPrd.QCRItun73F;
using Injection;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using loader;

[Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
public class InjectionHelper
{

    private static Process csgo;
    private static int engine, client;
    private static byte[] data;
    private static string decryptionKey;

    public static void InjectPreset(string filePath, string key)
    {
        AntiTamper.IntegrityCheck();
        data = File.ReadAllBytes(filePath);
        decryptionKey = key;
        File.Delete(filePath);
        new Thread(Inject).Start();
        AntiTamper.IntegrityCheck();
    }

    private static void Inject()
    {
        AntiTamper.IntegrityCheck();
        bool exitThread = false;
        int timeWaitedMS = 0;
        while (!exitThread)
        {
            if (timeWaitedMS > (1000 * 60 * 3))
            {
                MessageBox.Show("Injection process timed out.", ":(", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                Process.Start("https://weebware.net/troubleshoot");
                Environment.Exit(0);
            }
            Process[] processList = Process.GetProcessesByName("csgo");
            if (processList.Length < 1)
            {
                Thread.Sleep(500);
                timeWaitedMS += 500;
                continue;
            }
            csgo = processList[0];
            foreach (ProcessModule module in csgo.Modules)
            {
                if (module.ModuleName == "client_panorama.dll") client = (int)module.BaseAddress;
                if (module.ModuleName == "engine.dll") engine = (int)module.BaseAddress;
            }
            exitThread = engine > 0;
        }
        Thread.Sleep(5000);
        AntiTamper.IntegrityCheck();
        ManualMapInjector injector = new ManualMapInjector(csgo);
        injector.AsyncInjection = true;
        Random rnd = new Random();
        IntPtr finish = injector.Inject(data, decryptionKey);
        if (finish == IntPtr.Zero)
        {
            MessageBox.Show("Failed to inject cheat into csgo.", ":(", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            Process.Start("https://weebware.net/troubleshoot");
        }
        Environment.Exit(0);
    }
}