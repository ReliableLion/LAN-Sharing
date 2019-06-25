using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    public partial class ProgressForm : Form
    {
        private SortedDictionary<string, LanSharingRequest> progressList = new SortedDictionary<string, LanSharingRequest>();

        public ProgressForm(string title)
        {
            InitializeComponent();
            this.CenterToScreen();
            BringToFront();

            progressPanel.MaximumSize = new Size(progressPanel.Width, progressPanel.GetRowHeights()[0] * 5);

            //Panel wrapPanel = new Panel();
            //wrapPanel.Location = progressPanel.Location;
            //wrapPanel.Size = progressPanel.Size;
            //wrapPanel.Controls.Add(progressPanel);
            //progressPanel.Dock = DockStyle.Top;
            //progressPanel.AutoSize = true;
            //progressPanel.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            //progressPanel.AutoScroll = false;

            //wrapPanel.Dock = DockStyle.Fill;
            //wrapPanel.AutoSize = false;
            //wrapPanel.AutoScroll = true;

            this.closeButton.Enabled = false;
            this.cleanButton.Enabled = false;
            this.Text = title;
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams parms = base.CreateParams;
                parms.ClassStyle |= 0x200;  // CS_NOCLOSE
                return parms;
            }
        }

        public void addUploadProgressbar(CustomProgressBar progressBar, string id, string username, string file_path) {

            this.closeButton.Enabled = false;
            this.cleanButton.Enabled = false;

            var request = new LanSharingRequest(progressBar, id, username, file_path, progressPanel.Width);
            progressList.Add(id, request);

            progressPanel.Controls.Add(request);
            progressPanel.MaximumSize = new Size(progressPanel.Width, progressPanel.GetRowHeights()[0] * 5);

            this.closeButton.Location = new Point(this.closeButton.Location.X, this.closeButton.Location.Y + progressPanel.GetRowHeights()[0]);
            this.cleanButton.Location = new Point(this.cleanButton.Location.X, this.cleanButton.Location.Y + progressPanel.GetRowHeights()[0]);
        }

        public void addDownloadProgressbar(CustomProgressBar progressBar, string id, string file_path)
        {

            this.closeButton.Enabled = false;
            this.cleanButton.Enabled = false;

            var request = new LanSharingRequest(progressBar, id, file_path, progressPanel.Width);
            progressList.Add(id, request);

            progressPanel.Controls.Add(request);
            progressPanel.MaximumSize = new Size(progressPanel.Width, progressPanel.GetRowHeights()[0] * 5);

            this.closeButton.Location = new Point(this.closeButton.Location.X, this.closeButton.Location.Y + progressPanel.GetRowHeights()[0]);
            this.cleanButton.Location = new Point(this.cleanButton.Location.X, this.cleanButton.Location.Y + progressPanel.GetRowHeights()[0]);

        }

        public void progressTerminated(string id) {
            if (progressList.ContainsKey(id)) {
                progressList[id].terminated();
                progressList[id].completed = true;
            }

            var closable = true;
            var clean = false;
            foreach (var progress in progressList)
            {
                if (!progress.Value.completed)
                    closable = false;
                else
                    clean = true;
            }

            if (clean)
                this.cleanButton.Enabled = true;

            if(closable)
                this.closeButton.Enabled = true;
        }

        public int getUploadingRequest() {

            var counter = 0;

            foreach (var request in progressList)
            {
                if (!request.Value.completed)
                    counter++;
            }

            return counter;
        }

        private void CloseButton_Click(object sender, EventArgs e) {
            this.Close();
        }

        public void cleanUploadingRequest() {
            this.cleanButton.PerformClick();
        }

        private void CleanButton_Click(object sender, EventArgs e) {

            var keys = new List<string>();
            foreach (var request in progressList) {
                
                if (!request.Value.completed) continue;

                keys.Add(request.Key);
                progressPanel.Controls.Remove(request.Value);
                this.closeButton.Location = new Point(this.closeButton.Location.X, this.closeButton.Location.Y - progressPanel.GetRowHeights()[0]);
                this.cleanButton.Location = new Point(this.cleanButton.Location.X, this.cleanButton.Location.Y - progressPanel.GetRowHeights()[0]);
            }

            foreach (var key in keys) {
                if (!progressList[key].completed) continue;
                progressList.Remove(key);
            }

            if(progressList.Count < 1)
                this.closeButton.Enabled = true;

            this.cleanButton.Enabled = false;
        }
    }
}
