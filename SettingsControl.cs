using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    public partial class SettingsControl : UserControl
    {
        public SettingsControl()
        {
            InitializeComponent();
            string path;
            bool autoDownload;

            try  
            {  
                var appSettings = ConfigurationManager.AppSettings;
                path = appSettings[Constants.PATH] ?? "C:";
                autoDownload = Convert.ToBoolean((appSettings[Constants.AUTO_DOWNLOAD] ?? "true"));
            }  
            catch (ConfigurationErrorsException)
            {
                path = "C:";
                autoDownload = true;
            }

            downloadCheck.Checked = autoDownload;
            downloadPath.Text = path;
        }

        private void DownloadCheck_CheckedChanged(object sender, EventArgs e) {

            try  
            {  
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings[Constants.AUTO_DOWNLOAD] == null)  
                {  
                    settings.Add(Constants.AUTO_DOWNLOAD, Convert.ToString(downloadCheck.Checked));
                }  
                else  
                {  
                    settings[Constants.AUTO_DOWNLOAD].Value = Convert.ToString(downloadCheck.Checked);  
                }  
                configFile.Save(ConfigurationSaveMode.Modified);  
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

            }  
            catch (ConfigurationErrorsException) {
            }
        }

        private void DwnPathButton_Click(object sender, EventArgs e) {

            using (var fbd = new FolderBrowserDialog()) {
                
                DialogResult result = fbd.ShowDialog();

                if (result != DialogResult.OK || string.IsNullOrWhiteSpace(fbd.SelectedPath)) return;
                try  
                {  
                    var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    var settings = configFile.AppSettings.Settings;
                    if (settings[Constants.DLL_PATH] == null)  
                    {  
                        settings.Add(Constants.DLL_PATH, fbd.SelectedPath);
                    }  
                    else  
                    {  
                        settings[Constants.DLL_PATH].Value = fbd.SelectedPath;  
                    }  
                    configFile.Save(ConfigurationSaveMode.Modified);  
                    ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

                }  
                catch (ConfigurationErrorsException) {
                }

                downloadPath.Text = fbd.SelectedPath;
            }

        }
    }
}
