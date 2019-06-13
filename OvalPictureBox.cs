using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{

    class OvalPictureBox : PictureBox {
        public OvalPictureBox() {
            this.BackColor = Color.DarkGray;
        }
        protected override void OnResize(EventArgs e) {
            base.OnResize(e);
            /*using (var gp = new GraphicsPath()) {
                gp.AddEllipse(new Rectangle(0, 0, this.Width-1, this.Height-1));
                this.Region = new Region(gp);
            } */
        }

        private void Control_MouseHover ( object sender, EventArgs e ) {
                var c = (Control)sender;
                var rect = c.Bounds;
                rect.Inflate(1,1);
                var g = CreateGraphics ();
                ControlPaint.DrawBorder ( g, rect, Color.Blue, ButtonBorderStyle.Solid );
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            using (GraphicsPath gp = new GraphicsPath())
            {
                gp.AddEllipse(0, 0, this.Width - 1, this.Height - 1);
                Region = new Region(gp);
                e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
                e.Graphics.DrawEllipse(new Pen(new SolidBrush(this.BackColor), 1), 0, 0, this.Width - 1, this.Height - 1);
            }
        }
    }
}
