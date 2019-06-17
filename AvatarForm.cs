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

    public partial class AvatarForm : Form
    {
        private ProfileControl parent;

        public AvatarForm(ProfileControl profileControl)
        {
            InitializeComponent();
            CenterToParent();
            parent = profileControl;
        }

        private void Control_MouseHover ( object sender, EventArgs e )
        {
            //var control = ((Control) sender);
            //var g = ((Control)control).CreateGraphics();
            //ControlPaint.DrawBorder(g, control.ClientRectangle, Color.Red, 5, ButtonBorderStyle.Outset,
            //    Color.Red, 5, ButtonBorderStyle.Outset, Color.Red, 5, ButtonBorderStyle.Inset, Color.Red, 5,
            //    ButtonBorderStyle.Inset);
            var c = (Control)sender;
            var rect = c.Bounds;
            rect.Inflate(5,5);
            var g = CreateGraphics ();
            var borderColor = Color.FromArgb(25, 118, 210);
            var borderStyle = ButtonBorderStyle.Solid;
            var borderWidth = 3;

            ControlPaint.DrawBorder ( g, rect, borderColor,
                borderWidth,
                borderStyle,
                borderColor,
                borderWidth,
                borderStyle,
                borderColor,
                borderWidth,
                borderStyle,
                borderColor,
                borderWidth,
                borderStyle);
        }

        private void Control_MouseLeave ( object sender, EventArgs e ) {
            Invalidate ();
        }

        private void OvalPictureBox1_Click(object sender, EventArgs e)
        {
            parent.set_avatar("businessman");
        }

        private void OvalPictureBox2_Click(object sender, EventArgs e)
        {
            parent.set_avatar("businesswoman");
        }

        private void OvalPictureBox4_Click(object sender, EventArgs e)
        {
            parent.set_avatar("man");
        }

        private void OvalPictureBox3_Click(object sender, EventArgs e)
        {
            parent.set_avatar("man2");
        }

        private void OvalPictureBox8_Click(object sender, EventArgs e)
        {
            parent.set_avatar("man3");
        }

        private void OvalPictureBox7_Click(object sender, EventArgs e)
        {
            parent.set_avatar("woman");
        }

        private void OvalPictureBox6_Click(object sender, EventArgs e)
        {
            parent.set_avatar("woman2");
        }

        private void OvalPictureBox5_Click(object sender, EventArgs e)
        {
            parent.set_avatar("woman3");
        }
    }
}
