using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    public partial class ProgressForm : Form
    {
        private SortedDictionary<string, bool> progressList = new SortedDictionary<string, bool>();

        public ProgressForm()
        {
            InitializeComponent();
            this.CenterToScreen();
            BringToFront();
            this.ControlBox = false;
            this.closeButton.Enabled = false;
        }

        public void addProgressbar(CustomProgressBar progressBar, string id, string username, string file_path) {

            this.closeButton.Enabled = false;
            progressList.Add(id, false);
            
            var flowLayoutPanel = new FlowLayoutPanel();
            var label = new Label();
            var title = "Sending file " + file_path + " to " + username;
            label.Text = title;
            label.Width = progressPanel.Width;
            label.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            flowLayoutPanel.Controls.Add(label);
            flowLayoutPanel.Controls.Add(progressBar);
            flowLayoutPanel.FlowDirection = FlowDirection.TopDown;
            flowLayoutPanel.Width = progressPanel.Width;
            flowLayoutPanel.Height = 70;
            //flowLayoutPanel.BackColor = Color.Aqua;

            progressBar.Width = progressPanel.Width;
            progressPanel.Controls.Add(flowLayoutPanel);

            this.closeButton.Location = new Point(this.closeButton.Location.X, this.closeButton.Location.Y + progressPanel.GetRowHeights()[0]);
        }

        public void progressTerminated(string id) {
            if (progressList.ContainsKey(id)) {
                progressList[id] = true;
            }

            foreach (var progress in progressList) {
                if (!progress.Value)
                    return;
            }

            this.closeButton.Enabled = true;
        }

        private void CloseButton_Click(object sender, EventArgs e) {
            this.Close();
        }
    }
}
