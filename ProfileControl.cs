using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Drawing;
using System.Data;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    public partial class ProfileControl : UserControl
    {
        private AvatarForm _avatarForm;

        public ProfileControl()
        {
            InitializeComponent();
            string username;
            string avatarName;

            try  
            {  
                var appSettings = ConfigurationManager.AppSettings;
                username = appSettings["username"] ?? "DefaultUser";
                avatarName = appSettings["avatar"] ?? "man";
            }  
            catch (ConfigurationErrorsException)
            {
                username = "DefaultUser";
                avatarName = "man";
            }

            usernameLabel.Text = username;
            usernameBox.Text = username;
            userAvatarBox.Image = (System.Drawing.Image)LanSharing.Properties.Resources.ResourceManager.GetObject(avatarName);
        }

        private void Button1_Click(object sender, EventArgs e) {
            _avatarForm = new AvatarForm(this);
            _avatarForm.Show();
        }

        public static string Truncate(string value, int maxLength)
        {
            if (string.IsNullOrEmpty(value)) return value;
            return value.Length <= maxLength ? value : value.Substring(0, maxLength); 
        }

        public void set_avatar(string avatar_name) {

            DiscoveryUser.Instance.SetAvatar(avatar_name);
            try  
            {  
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);  
                var settings = configFile.AppSettings.Settings;  
                if (settings["avatar"] == null)  
                {  
                    settings.Add("avatar", avatar_name);  
                }  
                else  
                {  
                    settings["avatar"].Value = avatar_name;  
                }  
                configFile.Save(ConfigurationSaveMode.Modified);  
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

                userAvatarBox.Image = (System.Drawing.Image)LanSharing.Properties.Resources.ResourceManager.GetObject(avatar_name);
            }  
            catch (ConfigurationErrorsException) {
            }

            _avatarForm.Close();
        }

        private void SaveUsername_Click(object sender, EventArgs e)
        {

            var username = Truncate(usernameBox.Text, 254);
            DiscoveryUser.Instance.SetUsername(username);
            try  
            {  
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings["username"] == null)  
                {  
                    settings.Add("username", username);  
                }  
                else  
                {  
                    settings["username"].Value = username;  
                }  
                configFile.Save(ConfigurationSaveMode.Modified);  
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);

                usernameLabel.Text = username;
            }  
            catch (ConfigurationErrorsException) {
            }
        }
    }
}
