using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    public partial class DiscoveryForm : Form
    {

        System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        private SortedDictionary<string, string>  destinationUsers = new SortedDictionary<string, string> ();
        private string filePath;

        public DiscoveryForm(string fileName) {
            InitializeComponent();
            this.CenterToScreen();
            BringToFront();
            TopMost = true;
            filePath = fileName;
            Text = @"Sending " + Path.GetFileName(fileName);
            
            notFoundLabel.Hide();
            flowLayoutPanel1.Hide();
            sendButton.Enabled = false;

            search_users();
        }

        private void search_users() {
            DiscoveryUser.Instance.search_user();
        }

        private void users_collected(Object myObject, EventArgs myEventArgs) {
            myTimer.Stop();
            Console.Out.WriteLine("USERS COLLECTED");
            
            pictureBox1.Hide();

            var users = DiscoveryUser.Instance.get_users();

            //OvalPictureBox[] pics = new OvalPictureBox[users.Count];
            //Label[] txts = new Label[users.Count];
            //FlowLayoutPanel[] flws = new FlowLayoutPanel[users.Count];
            OvalPictureBox[] pics = new OvalPictureBox[10];
            Label[] txts = new Label[10];
            UserPanel[] flws = new UserPanel[10];
            int brh =0, i = 0;

            if (users.Count == 0) {
                notFoundLabel.Show();
                return;
            }

            flowLayoutPanel1.Show();
            flowLayoutPanel1.BackColor = Color.Transparent;

            foreach (var user in users) {

                flws[i] = new UserPanel();
                flws[i].Name = "flw" + i;
                flws[i].Location = new Point(3, brh);
                flws[i].Size = new Size(110, 180);
                //flws[i].BackColor = Color.DarkCyan;
                flws[i].user_address = user.Key;
                flws[i].username = user.Value.get_username();

                pics[i] = new OvalPictureBox();
                pics[i].Location = new Point(953, 0 + brh);
                pics[i].Name = "pic" + i;
                pics[i].Size = new Size(100, 100);
                pics[i].Image = (System.Drawing.Image)LanSharing.Properties.Resources.ResourceManager.GetObject(user.Value.get_avatar());
                pics[i].SizeMode = PictureBoxSizeMode.StretchImage;
                pics[i].Click += Control_Mouse_Click;
                flws[i].Controls.Add(pics[i]);

                txts[i] = new Label();
                txts[i].AutoSize = true;
                txts[i].Name = "txt" + i;
                txts[i].Text = user.Value.get_username() + @" (" + user.Key + @")";
                txts[i].Font = new Font(new FontFamily("Microsoft Sans Serif"), 9, FontStyle.Bold);
                txts[i].Location = new Point(953, 130 + brh);
                txts[i].Click += Control_Mouse_Click;
                flws[i].Controls.Add(txts[i]);

                //butns[i] = new Button();
                //butns[i].Text = "submit";
                //butns[i].Name = "but" + i;
                //butns[i].Location = new Point(1100, 186 + brh);

                //flws[i].Controls.Add(butns[i]);
                //this.Controls.Add(flws[i]);
                flowLayoutPanel1.Controls.Add(flws[i]);
                brh += 130;
                i += 1;
            }

        }

        private void DiscoveryForm_Shown(Object sender, EventArgs e) {

            myTimer.Tick += new EventHandler(users_collected);
            // Sets the timer interval to 5 seconds.
            myTimer.Interval = 3000;
            myTimer.Start();
        }

        private void Control_Mouse_Click ( object sender, EventArgs e) {

            var c = (UserPanel)((Control)sender).Parent;
            c.OnClick();

            if (c.isSelected()) {

                if (!destinationUsers.ContainsKey(c.user_address))
                    destinationUsers.Add(c.user_address, c.username);
                else
                    destinationUsers[c.user_address] = c.username;

            } else {
                if (destinationUsers.ContainsKey(c.user_address))
                    destinationUsers.Remove(c.user_address);
            }

            if (this.destinationUsers.Count == 0)
                sendButton.Enabled = false;
            else
                sendButton.Enabled = true;
        }

        private void Control_MouseLeave ( object sender, EventArgs e ) {
            Invalidate ();
        }

        private void SendButton_Click(object sender, EventArgs e) {
            foreach (var u in this.destinationUsers) {
                Console.Out.WriteLine(u);
            }
            FileSender fileSender = new FileSender(destinationUsers, filePath);
            fileSender.sendFile();
        }
    }
}
