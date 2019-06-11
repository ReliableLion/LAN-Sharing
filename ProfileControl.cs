using System;
using System.Collections.Generic;
using System.ComponentModel;
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
        public ProfileControl()
        {
            InitializeComponent();

            /* System.Drawing.Drawing2D.GraphicsPath gp = new System.Drawing.Drawing2D.GraphicsPath();
            gp.AddEllipse(0, 0, pictureBox1.Width - 3, pictureBox1.Height - 3);
            Region rg = new Region(gp);
            pictureBox1.Region = rg;*/
        }

        public IEnumerable<String> net_adapters() 
        { 
            foreach (NetworkInterface nic in NetworkInterface.GetAllNetworkInterfaces())
            {
                Console.Out.WriteLine(nic.Name);
                foreach (var address in nic.GetIPProperties().UnicastAddresses) {
                    if(address.Address.AddressFamily == AddressFamily.InterNetwork)
                        Console.Out.WriteLine(address.Address.ToString());
                }

                yield return nic.GetIPProperties().UnicastAddresses[0].ToString();
            } 
            yield break;
        }

        [DllImport("C:\\Users\\Asus\\source\\repos\\Dll1\\Debug\\DLL1.dll")]
        public static extern bool start_lan_sharing(string username);

        [DllImport("C:\\Users\\Asus\\source\\repos\\Dll1\\Debug\\DLL1.dll")]
        public static extern int fnDll1();

        private void Button1_Click(object sender, EventArgs e)
        {
            //label1.Text = start_lan_sharing(Truncate("User", 32)) ? "AVVIATO" : "NON AVVIATO";
            Console.Out.WriteLine(fnDll1());
            IEnumerable<String> networks = net_adapters();
            foreach(String network in networks) {
                //Console.Out.WriteLine(network);
            }
        }

        public static string Truncate(string value, int maxLength)
        {
            if (string.IsNullOrEmpty(value)) return value;
            return value.Length <= maxLength ? value : value.Substring(0, maxLength); 
        }
    }
}
