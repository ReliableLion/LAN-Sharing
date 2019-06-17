using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using LanSharing.Properties;

namespace LanSharing
{
    static class Program
    {
        public delegate void ShutdownDelegate();
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        private static extern void save_exception_callback(ShutdownDelegate callback);

        private static ShutdownDelegate shutdownDelegate;

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool start_lan_sharing(string path);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool start_discovery_service(string username, string avatar);

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static Mutex mutex = new Mutex(true, "{8F6F0AC4-B2F1-45ad-A8CF-72F04E6BDE8F}");
        [STAThread]
        static void Main(string[] args)
        {
            if(args.Length != 0)
                foreach (var arg in args)
                {
                    Console.Out.WriteLine(arg);
                }
            if (mutex.WaitOne(TimeSpan.Zero, true))
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                // Used to receive windows messages
                MessageWindow window = new MessageWindow();

                shutdownDelegate = delegate()
                {
                    var form = new ErrorForm();
                    form.Show();
                };

                save_exception_callback(shutdownDelegate);

                string path;
                string username;
                string avatar;

                try
                {
                    if (Settings.Default[Constants.PATH].ToString() == "") {
                        Settings.Default[Constants.PATH] = UsersFolder.GetPath(KnownFolder.Downloads);
                    }
                    if (Settings.Default[Constants.USERNAME].ToString() == "") {
                        Settings.Default[Constants.USERNAME] = "DefaultUser";
                    }
                    if (Settings.Default[Constants.AVATAR].ToString() == "") {
                        Settings.Default[Constants.AVATAR] = "man";
                    }
                    Settings.Default.Save();

                    path = Settings.Default[Constants.PATH].ToString();
                    username = Settings.Default[Constants.USERNAME].ToString();
                    avatar = Settings.Default[Constants.AVATAR].ToString();
                }
                catch (ConfigurationErrorsException)
                {
                    path = UsersFolder.GetPath(KnownFolder.Downloads);
                    username = "Default User";
                    avatar = "man";
                }

                if (!start_lan_sharing(path))
                    Application.Exit();

                DiscoveryUser.Instance.Online();
                if (!start_discovery_service(username, avatar))
                    Application.Exit();

                if (args.Length > 0) {
                    int hWnd = NativeMethods.getWindowId(null, "PDS-LanSharing application");
                    //result = NativeMethods.sendWindowsMessage(hWnd, NativeMethods.WM_USER, 123, 456);
                    NativeMethods.sendWindowsStringMessage(hWnd, 0, args[0]);
                }

                Application.Run(new MainForm());
            }
            else {
                // send our Win32 message to make the currently running instance
                // jump on top of all the other windows
                if (args.Length <= 0)
                    return;

                var result = 0;
                    var hWnd = NativeMethods.getWindowId(null, "PDS-LanSharing application");
                    //result = NativeMethods.sendWindowsMessage(hWnd, NativeMethods.WM_USER, 123, 456);
                    result = NativeMethods.sendWindowsStringMessage(hWnd, 0, args[0]);

                if (result != 1) {
                    MessageBox.Show("Some error occurs. Restart Lan Sharing", "Receiveing file", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, (MessageBoxOptions)0x40000);
                }
            }
        }
    }
}
