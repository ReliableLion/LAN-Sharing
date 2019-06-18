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
                DiscoveryUser.go_online();
            } else {
                statusBtn.Text = @"Go Online";
                statusLabel.Text = @"Offline";
                DiscoveryUser.hide_me();
            }

            statusBtn.UseVisualStyleBackColor = false;
        }

        private void Button1_Click(object sender, EventArgs e) {

        }

        private void StatusBtn_Click(object sender, EventArgs e) {
            if (Convert.ToBoolean(Settings.Default[Constants.STATUS])) {
                pause_server();
                statusBtn.Text = @"Go Online";
                statusLabel.Text = @"Offline";
                Settings.Default[Constants.STATUS] = false;
                DiscoveryUser.hide_me();
            } else {
                recover_server();
                statusBtn.Text = @"Go Offline";
                statusLabel.Text = @"Online";
                Settings.Default[Constants.STATUS] = true;
                DiscoveryUser.go_online();
            }

            Settings.Default.Save();
        }

        private void Alert_Load(object sender, EventArgs e)
        {
            Color col = statusBtn.BackColor;
            statusBtn.BackColor = Color.FromArgb(50, 0, 0, 0);

            this.Top = Screen.PrimaryScreen.Bounds.Height - this.Height - 35;
            this.Left = Screen.PrimaryScreen.Bounds.Width - this.Width - 15;
        }

        private void CloseStatus_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
