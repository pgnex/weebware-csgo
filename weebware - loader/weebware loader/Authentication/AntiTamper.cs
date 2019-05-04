using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Threading;
using System.Windows.Forms;
using loader;
using loader.Authentication;
using loader.Functions;
using SafeRequest.NET;

[Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
class AntiTamper
{

    private static Thread instance;

    public static void Start()
    {
        if (instance != null && instance.IsAlive)
            instance.Abort();
        Thread t = new Thread(AntiTamper.Thread);
        t.Start();
        instance = t;
    }

    public static void ip_check() {

        Response response = Networking.check_ban();

        if (response.status) return;

        if (response.GetData<string>("detail") == "connection error") {
            MessageBox.Show("An unexpected error has occured. Server may be down. Please try again later or contact support for further information.");
            Environment.Exit(1337);
        }

        if (response.GetData<string>("detail") == "banned") {
            MessageBox.Show("You have been banned. Contact support for more information.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            Environment.Exit(1337);
        }
    }

    [Obfuscation(Feature = "inline", Exclude = false)]
    public static void IntegrityCheck()
    {
        if (instance == null || !instance.IsAlive)
        {
            Networking.Alert("\nUnable to verify integrity of security thread.");
            MessageBox.Show("unable to verify");
            Environment.Exit(1337);
        }
    }

    public static Thread GetInstance()
    {
        return instance;
    }

    [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
    static extern bool CheckRemoteDebuggerPresent(IntPtr hProcess, ref bool isDebuggerPresent);

    private static List<string> ProcessNames, WindowTitles, FileHashes;

    private static void InitalizeAntis()
    {

        List<Tuple<string, string>> AntiList = new List<Tuple<string, string>>();

        AntiList.Add(Program("Fiddler", "Fiddler"));
        AntiList.Add(Program("dnSpy", "dnSpy"));
        AntiList.Add(Program("dnSpy-x86", "dnSpy"));
        AntiList.Add(Program("ida", ""));
        AntiList.Add(Program("ollydbg", ""));
        AntiList.Add(Program("MegaDumper", "MegaDumper"));
        AntiList.Add(Program("X64NetDumper", ""));
        AntiList.Add(Program("x64dbg", ""));
        AntiList.Add(Program("NETUnpack", ".NET Generic Unpacker"));

        ProcessNames = new List<string>();
        WindowTitles = new List<string>();
        FileHashes = new List<string>();

        foreach (Tuple<string, string> item in AntiList)
        {
            if (!string.IsNullOrEmpty(item.Item1))
                ProcessNames.Add(item.Item1);
            if (!string.IsNullOrEmpty(item.Item2))
                WindowTitles.Add(item.Item2);
        }
    }

    private static Tuple<string, string> Program(string item1, string item2)
    {
        return new Tuple<string, string>(item1, item2);
    }

    private static bool Exit = false;
    private static int ExitCode = 0;
    private static string ExitReason = string.Empty;

    private static void Thread()
    {
        new Thread(() =>
        {

        }).Start();
        InitalizeAntis();
        IntPtr ProcessHandle = Process.GetCurrentProcess().Handle;
        bool isDebuggerPresent = false;
        while (true)
        {
            if (Exit)
            {
                Networking.Alert(ExitReason);
                Environment.Exit(ExitCode);
            }
            CheckRemoteDebuggerPresent(ProcessHandle, ref isDebuggerPresent);
            if (isDebuggerPresent)
            {
                Exit = true;
                ExitCode = 0;
                ExitReason = "Remote debugger detected.";
            }
            Process[] processlist = Process.GetProcesses();
            foreach (Process process in processlist)
            {
                try
                {
                    if (String.IsNullOrEmpty(process.MainWindowTitle)) continue;
                    bool
                        isForbiddenProcessName = false,
                        isForbiddenWindowTitle = false;
                    isForbiddenProcessName = ProcessNames.Contains(process.ProcessName);
                    foreach (string forbiddenTitle in WindowTitles)
                    {
                        if (process.MainWindowTitle.Contains(forbiddenTitle))
                        {
                            isForbiddenWindowTitle = true;
                            break;
                        }
                    }
                    bool isForbidden = isForbiddenProcessName || isForbiddenWindowTitle;
                    if (isForbidden)
                    {
                        Exit = true;
                        ExitCode = process.Id;
                        ExitReason = "Blacklisted process detected.\n" +
                            "Name: " + process.ProcessName +
                            "\nWindow Title: " + process.MainWindowTitle;
                    }
                }
                catch (Exception) { }
            }
            System.Threading.Thread.Sleep(500);
        }
    }

}