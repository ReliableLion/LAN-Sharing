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
using Microsoft.Win32;

namespace LanSharing
{
    public partial class Form1 : Form
    {

        private System.Windows.Forms.ContextMenu contextMenu1;
        private System.Windows.Forms.MenuItem menuItem1;

        private const string MenuName = "Software\\Classes\\*\\shell\\LanSharing";
        private const string Command = "Software\\Classes\\*\\shell\\LanSharing\\command";

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string libname);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern bool FreeLibrary(IntPtr hModule);

        public Form1()
        {
            InitializeComponent();
            sidePanel.Height = homeButton.Height;
            sidePanel.Top = homeButton.Top;
            profileControl1.BringToFront();
            notifyIcon1.Visible = true;
            Icon = notifyIcon1.Icon;

            this.contextMenu1 = new System.Windows.Forms.ContextMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();

            // Initialize contextMenu1
            this.contextMenu1.MenuItems.AddRange(
                new System.Windows.Forms.MenuItem[] {this.menuItem1});

            // Initialize menuItem1
            this.menuItem1.Index = 0;
            this.menuItem1.Text = @"Exit";
            this.menuItem1.Click += new System.EventHandler(this.menuItem1_Click);
            notifyIcon1.ContextMenu = contextMenu1;

            AddRightClickItem();

            ////Load
            //IntPtr Handle = LoadLibrary("C:\\Users\\Asus\\source\\repos\\Dll1\\Debug\\DLL1.dll");
            //if (Handle == IntPtr.Zero) {
            //    int errorCode = Marshal.GetLastWin32Error();
            //    throw new Exception(string.Format("Failed to load library (ErrorCode: {0})",errorCode));
            //}
        }

        private void ExitApplication() {
            //Free
            //if(Handle != IntPtr.Zero)
            //    FreeLibrary(Handle);
            Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            notifyIcon1.BalloonTipTitle = @"Lan Sharing";
            notifyIcon1.BalloonTipText = @"Lan Sharing is running in Background";
        }

        private void Panel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Label1_Click(object sender, EventArgs e)
        {

        }

        private void PictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void HomeButton_Click(object sender, EventArgs e)
        {
            sidePanel.Height = homeButton.Height;
            sidePanel.Top = homeButton.Top;
            profileControl1.BringToFront();
        }

        private void SettingsButton_Click(object sender, EventArgs e)
        {
            sidePanel.Height = settingsButton.Height;
            sidePanel.Top = settingsButton.Top;
            settingsControl1.BringToFront();
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                ShowIcon = false;
                notifyIcon1.ShowBalloonTip(1000);
            }
        }

        private void NotifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Icon = notifyIcon1.Icon;
            ShowInTaskbar = true;
            WindowState = FormWindowState.Normal;
            this.Show();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason != CloseReason.UserClosing) return;
            e.Cancel = true;
            ShowInTaskbar = false;
            WindowState = FormWindowState.Minimized;
        }

        private void menuItem1_Click(object Sender, EventArgs e)
        {
            ExitApplication();
        }

        private void AddRightClickItem()
        {
            RegistryKey registryCmd = null;
            var registryMenu = Registry.CurrentUser.OpenSubKey(MenuName);
            if(registryMenu != null)
                return;

            try
            {
                registryMenu = Registry.CurrentUser.CreateSubKey(MenuName);
                registryMenu?.SetValue("","Share with");
                registryMenu?.SetValue("Icon", "C:\\Users\\Asus\\Documents\\Malnati\\icon_lan_sharing.ico");
                registryCmd = Registry.CurrentUser.CreateSubKey(Command);
                registryCmd?.SetValue("", "");
            }
            catch(Exception ex)
            {
                MessageBox.Show(this,ex.ToString());
            }
            finally       
            {
                registryMenu?.Close();
                registryCmd?.Close();
            }        
        }
    }

}
