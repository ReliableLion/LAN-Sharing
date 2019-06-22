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
        public ProgressForm()
        {
            InitializeComponent();
            BringToFront();
        }

        public void addProgressbar(ProgressBar progressBar) {
            progressBar.Width = progressPanel.Width;
            progressPanel.Controls.Add(progressBar);
        }
    }
}
