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

            //if (this.Focused) {
            //    var rc = this.ClientRectangle;
            //    rc.Inflate(5, 5);
            //    ControlPaint.DrawFocusRectangle(e.Graphics, rc);
            //    Console.Out.WriteLine("OJHDAIOHDUIOASIOUDHAPUIOH");
            //}
        }

        //protected override void OnMouseClick(MouseEventArgs e) {

        //    using (GraphicsPath gp = new GraphicsPath())
        //    {
        //        gp.AddEllipse(0, 0, this.Width - 1, this.Height - 1);
        //        Region = new Region(gp);

        //        var rc = this.ClientRectangle;
        //        rc.Inflate(-3, -3);
        //        ControlPaint.DrawBorder(this.CreateGraphics(), rc, Color.FromArgb(25, 118, 210),
        //            ButtonBorderStyle.Solid);
        //        Console.Out.WriteLine("OJHDAIOHDUIOASIOUDHAPUIOH");
        //    }

        //    base.OnMouseClick(e);
        //}
        //protected override void OnMouseLeave(EventArgs e) {
        //    this.Invalidate();
        //    base.OnEnter(e);
        //}
        //protected override void OnLeave(EventArgs e) {
        //    this.Invalidate();
        //    base.OnLeave(e);
        //}

    }
}
