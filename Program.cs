using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    static class Program
    {
        public delegate void ShutdownDelegate();
        [DllImport("C:\\Users\\Asus\\source\\repos\\Dll1\\Debug\\DLL1.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SaveDelegate(ShutdownDelegate callback);

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            SaveDelegate(delegate() {
                Console.WriteLine("Called back by unmanaged side ");
            });
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
