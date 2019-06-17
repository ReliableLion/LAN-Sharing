using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using LanSharing.Properties;

namespace LanSharing
{
    public partial class SettingsControl : UserControl
    {
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void change_server_dw_path(string path);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void change_auto_accept(bool accept);

        public SettingsControl()
        {
            InitializeComponent();
            string path;
            bool autoDownload;

            try  
            {  
                path = Settings.Default[Constants.PATH].ToString();
                autoDownload = Convert.ToBoolean(Settings.Default[Constants.AUTO_DOWNLOAD]);
            }  
            catch (ConfigurationErrorsException)
            {
                path = UsersFolder.GetPath(KnownFolder.Downloads);
                autoDownload = true;
            }

            downloadCheck.Checked = autoDownload;
            downloadPath.Text = path;
        }

        private void DownloadCheck_CheckedChanged(object sender, EventArgs e) {

            try  
            {  
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

                if (Settings.Default[Constants.AUTO_DOWNLOAD] == null)
                {  
                    Settings.Default[Constants.AUTO_DOWNLOAD] = downloadCheck.Checked;
                }  
                else  
                {  
                    Settings.Default[Constants.AUTO_DOWNLOAD] = downloadCheck.Checked;  
                }  
                Settings.Default.Save();

                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

            }  
            catch (ConfigurationErrorsException) {
            }

            change_auto_accept(downloadCheck.Checked);

        }

        private void DwnPathButton_Click(object sender, EventArgs e) {

            using (var fbd = new FolderBrowserDialog()) {
                
                DialogResult result = fbd.ShowDialog();

                if (result != DialogResult.OK || string.IsNullOrWhiteSpace(fbd.SelectedPath)) return;
                try  
                {  
                    var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

                    if (Settings.Default[Constants.PATH] == null)  
                    {  
                        Settings.Default[Constants.PATH] = fbd.SelectedPath;
                    }  
                    else  
                    {  
                        Settings.Default[Constants.PATH] = fbd.SelectedPath;  
                    }  
                    Settings.Default.Save();
                    ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

                }  
                catch (ConfigurationErrorsException) {
                }

                downloadPath.Text = fbd.SelectedPath;
                change_server_dw_path(fbd.SelectedPath);
            }

        }
    }
}
