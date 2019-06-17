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
    class UserPanel : FlowLayoutPanel
    {

        public string user_address;
        public string username;
        private bool selected = false;

        public void OnClick()
        {
            if (!selected) {
                selected = true;
                var rc = this.ClientRectangle;
                var borderColor = Color.FromArgb(25, 118, 210);
                var borderStyle = ButtonBorderStyle.Solid;
                var borderWidth = 3;

                rc.Inflate(0, 0);
                ControlPaint.DrawBorder(this.CreateGraphics(), rc, borderColor,
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
            } else {
                Invalidate();
                selected = false;
                var rc = this.ClientRectangle;
                rc.Inflate(0, 0);
                ControlPaint.DrawBorder(this.CreateGraphics(), rc, Color.FromArgb(0, 255, 255, 255),
                    ButtonBorderStyle.Inset);
            }
        }

        protected override void OnPaint(PaintEventArgs pe) {
            base.OnPaint(pe);

            if (!selected)
                return;
            var rc = this.ClientRectangle;
            var borderColor = Color.FromArgb(25, 118, 210);
            var borderStyle = ButtonBorderStyle.Solid;
            var borderWidth = 3;

            rc.Inflate(0, 0);
            ControlPaint.DrawBorder(this.CreateGraphics(), rc, borderColor,
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

        public bool isSelected() {
            return selected;
        }
    }
}
