using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using LanSharing.Properties;

namespace LanSharing
{
    public partial class Alert : Form
    {

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void pause_server();

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void recover_server();

        public Alert()
        {
            InitializeComponent();

            if (Convert.ToBoolean(Settings.Default[Constants.STATUS])) {
                statusBtn.Text = @"Go Offline";
                statusLabel.Text = @"Online";
            } else {
                statusBtn.Text = @"Go Online";
                statusLabel.Text = @"Offline";
            }
        }

        private void Button1_Click(object sender, EventArgs e) {

        }

        private void StatusBtn_Click(object sender, EventArgs e) {
            if (Convert.ToBoolean(Settings.Default[Constants.STATUS])) {
                pause_server();
                statusBtn.Text = @"Go Online";
                statusLabel.Text = @"Offline";
                Settings.Default[Constants.STATUS] = false;
            } else {
                recover_server();
                statusBtn.Text = @"Go Offline";
                statusLabel.Text = @"Online";
                Settings.Default[Constants.STATUS] = true;
            }

            Settings.Default.Save();
        }

        private int interval = 0;
        private void Show_alert_Tick(object sender, EventArgs e)  {
            if (this.Top < 60)
            {
                this.Top += interval;
                interval += 2;
            } else
            {
                show_alert.Stop();
            }
        }

        private void Alert_Load(object sender, EventArgs e)
        {
            this.Top = Screen.PrimaryScreen.Bounds.Height - this.Height - 35;
            this.Left = Screen.PrimaryScreen.Bounds.Width - this.Width - 15;
        }

        private void CloseStatus_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
