using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LanSharing
{
// this class just wraps some Win32 stuff that we're going to use
    internal class NativeMethods
    {
        [DllImport("User32.dll")]
        private static extern int RegisterWindowMessage(string lpString);
        
        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        public static extern Int32 FindWindow(String lpClassName, String lpWindowName);
        
        //For use with WM_COPYDATA and COPYDATASTRUCT
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(int hWnd, int Msg, int wParam, ref COPYDATASTRUCT lParam);
        
        //For use with WM_COPYDATA and COPYDATASTRUCT
        [DllImport("User32.dll", EntryPoint = "PostMessage")]
        public static extern int PostMessage(int hWnd, int Msg, int wParam, ref COPYDATASTRUCT lParam);
        
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(int hWnd, int Msg, int wParam, int lParam);
        
        [DllImport("User32.dll", EntryPoint = "PostMessage")]
        public static extern int PostMessage(int hWnd, int Msg, int wParam, int lParam);

        [DllImport("User32.dll", EntryPoint = "SetForegroundWindow")]
        public static extern bool SetForegroundWindow(int hWnd);

        public const int WM_USER = 0x400;
        public const int WM_COPYDATA = 0x4A;
        public const int HWND_BROADCAST = 0xffff;

        //Used for WM_COPYDATA for string messages
        public struct COPYDATASTRUCT
        {
            public IntPtr dwData;
            public int cbData;
            [MarshalAs(UnmanagedType.LPStr)]
            public string lpData;
        }

        public static int sendWindowsStringMessage(int hWnd, int wParam, string msg)
        {
            int result = 0;
            
            if (hWnd > 0)
            {
                byte[] sarr = System.Text.Encoding.Default.GetBytes(msg);
                int len = sarr.Length;
                COPYDATASTRUCT cds;
                cds.dwData = (IntPtr)100;
                cds.lpData = msg;
                cds.cbData = len + 1;
                result = SendMessage(hWnd, WM_COPYDATA, wParam, ref cds);
            }
            
            return result;
        }
        
        public static int sendWindowsMessage(int hWnd, int Msg, int wParam, int lParam)
        {
            int result = 0;
            
            if (hWnd > 0)
            {
                result = SendMessage(hWnd, Msg, wParam, lParam);
            }
            
            return result;
        }

        public static int getWindowId(string className, string windowName)
        {
            return FindWindow(className, windowName);
        }

        public bool bringAppToFront(int hWnd)
        {
            return SetForegroundWindow(hWnd);
        }
    
    }
}
