using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    class MessageWindow : Form {

        [DllImport("user32.dll")]
        static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        public MessageWindow() {
            var accessHandle = this.Handle;
            this.Text = @"PDS-LanSharing application";
        }

        protected override void OnHandleCreated(EventArgs e) {
            base.OnHandleCreated(e);
            ChangeToMessageOnlyWindow();         
        }

        private void ChangeToMessageOnlyWindow() {
            IntPtr HWND_MESSAGE = new IntPtr(-3);
            SetParent(this.Handle, HWND_MESSAGE);         
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            switch (m.Msg)
            {
                case NativeMethods.WM_USER:
                    MessageBox.Show("Message recieved: " + m.WParam + " - " + m.LParam, "Receiveing file", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, (MessageBoxOptions)0x40000);
                    break;
                case NativeMethods.WM_COPYDATA:
                    NativeMethods.COPYDATASTRUCT mystr = new NativeMethods.COPYDATASTRUCT();
                    Type mytype = mystr.GetType();
                    mystr = (NativeMethods.COPYDATASTRUCT)m.GetLParam(mytype);
                    Console.Out.WriteLine("RECEIVED:  " + mystr.lpData);
                    //MessageBox.Show("Sending: " + mystr.lpData, "Receiveing file", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, (MessageBoxOptions)0x40000);
                    DiscoveryForm discoveryForm = new DiscoveryForm(mystr.lpData);
                    discoveryForm.Show();
                    break;
            }
            m.Result = new IntPtr(1);
        }
    }
}
