using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using LanSharing.Properties;
using Microsoft.Win32;

namespace LanSharing
{
    public partial class MainForm : Form
    {

        private System.Windows.Forms.ContextMenu contextMenu1;
        private System.Windows.Forms.MenuItem menuItem1;
        private static Alert alert = new Alert();

        private const string MenuName = "Software\\Classes\\*\\shell\\LanSharing";
        private const string Command = "Software\\Classes\\*\\shell\\LanSharing\\command";
        private const string MenuNameDir = "Software\\Classes\\Directory\\shell\\LanSharing";
        private const string CommandDir = "Software\\Classes\\Directory\\shell\\LanSharing\\command";

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate bool AutoDownloadDelegate(string fileName);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_accept_callback(AutoDownloadDelegate callback);
        private static AutoDownloadDelegate autoDownloadDelegate_;

        public MainForm()
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

            autoDownloadDelegate_ = (fileName) =>
            {

                bool autoDownload;

                try  
                {  
                    autoDownload = Convert.ToBoolean(Settings.Default[Constants.AUTO_DOWNLOAD]);
                }  
                catch (ConfigurationErrorsException)
                {
                    autoDownload = true;
                }

                if (autoDownload)
                    return true;

                var text = "Do you want to accept the following file: " + fileName;

                DialogResult dialogResult = MessageBox.Show(text, @"File accept", MessageBoxButtons.YesNo);

                switch (dialogResult)
                {
                    case DialogResult.Yes:
                        return true;
                    case DialogResult.No:
                        return false;
                    default:
                        return false;
                }
            };

            save_accept_callback(autoDownloadDelegate_);

            //ProgressForm newForm = new ProgressForm();
            //ProgressBar progressBar = new ProgressBar();
            //progressBar.Text = "Sending ";
            //progressBar.Show();
            //progressBar.BringToFront();
            //progressBar.Visible = true;
            ////progressBar.Location = new System.Drawing.Point(20, 20);
            //newForm.addProgressbar(progressBar);
            //newForm.Show();

            //ProgressBar progressBar2 = new ProgressBar();
            //progressBar2.Text = "Sending ";
            //progressBar2.Show();
            //progressBar2.BringToFront();
            //progressBar2.Visible = true;
            //newForm.addProgressbar(progressBar2);

            //this.Controls.Add(progressBar);
            // progressBars.Add(id, progressBar);
        }

        private void ExitApplication() {
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
            var registryMenuDir = Registry.CurrentUser.OpenSubKey(MenuNameDir);
            if(registryMenu != null && registryMenuDir != null)
                return;

            try
            {
                var appPath = "\"" + System.Reflection.Assembly.GetExecutingAssembly().Location + "\"";
                var iconPath = "\"" +
                    System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) +
                    "\\icon_lan_sharing.ico" + "\"";
                registryMenu = Registry.CurrentUser.CreateSubKey(MenuName);
                registryMenu?.SetValue("","Share with");
                registryMenu?.SetValue("Icon", iconPath);
                registryCmd = Registry.CurrentUser.CreateSubKey(Command);
                registryCmd?.SetValue("", appPath + " \"%1\"");

                registryMenuDir = Registry.CurrentUser.CreateSubKey(MenuNameDir);
                registryMenuDir?.SetValue("","Share with");
                registryMenuDir?.SetValue("Icon", iconPath );
                registryMenuDir = Registry.CurrentUser.CreateSubKey(CommandDir);
                registryMenuDir?.SetValue("", appPath + " \"%1\"");
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

        private void NotifyIcon1_MouseClick(object sender, MouseEventArgs e)
        {
            if(alert.IsDisposed)
                return;
            if (alert.Visible) {
                alert.BringToFront();
                return;
            }

            alert.Show();
        }
    }

}
