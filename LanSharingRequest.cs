using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    class LanSharingRequest : FlowLayoutPanel
    {
        private CustomProgressBar progressBar;
        private string id;
        private string username;
        private string file_path;

        public bool completed { get; set; }

        public LanSharingRequest(CustomProgressBar progressBar, string id, string username, string file_path, int width) {

            completed = false;

            var label = new Label();
            var title = "Sending file " + file_path + " to " + username;
            label.Text = title;
            label.Width = width;
            label.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            this.Controls.Add(label);
            this.Controls.Add(progressBar);
            this.FlowDirection = FlowDirection.TopDown;
            this.Width = width;
            this.Height = 70;
            //flowLayoutPanel.BackColor = Color.Aqua;

            progressBar.Width = width;
        }

        public LanSharingRequest(CustomProgressBar progressBar, string id, string file_path, int width) {

            completed = false;

            var label = new Label();
            var title = "Download file " + file_path;
            label.Text = title;
            label.Width = width;
            label.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            this.Controls.Add(label);
            this.Controls.Add(progressBar);
            this.FlowDirection = FlowDirection.TopDown;
            this.Width = width;
            this.Height = 70;
            //flowLayoutPanel.BackColor = Color.Aqua;

            progressBar.Width = width;

        }
    }
}
