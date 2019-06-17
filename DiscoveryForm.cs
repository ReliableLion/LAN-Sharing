using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

        public DiscoveryForm()
        {
            InitializeComponent();
            search_users();
        }

        private void search_users() {
            DiscoveryUser.Instance.search_user();
            Thread.Sleep(3000);
        }

        private void users_collected(Object myObject, EventArgs myEventArgs) {
            myTimer.Stop();
            Console.Out.WriteLine("USERS COLLECTED");
            DiscoveryUser.Instance.print();
        }

        private void DiscoveryForm_Shown(Object sender, EventArgs e) {

            myTimer.Tick += new EventHandler(users_collected);
            // Sets the timer interval to 5 seconds.
            myTimer.Interval = 3000;
            myTimer.Start();
        }
    }
}
