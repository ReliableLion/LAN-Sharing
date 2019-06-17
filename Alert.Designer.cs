namespace LanSharing
{
    partial class Alert
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.statusBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.statusLabel = new System.Windows.Forms.Label();
            this.closeStatus = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.closeStatus)).BeginInit();
            this.SuspendLayout();
            // 
            // statusBtn
            // 
            this.statusBtn.BackColor = System.Drawing.Color.Black;
            this.statusBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.statusBtn.ForeColor = System.Drawing.Color.Transparent;
            this.statusBtn.Location = new System.Drawing.Point(12, 6);
            this.statusBtn.Name = "statusBtn";
            this.statusBtn.Size = new System.Drawing.Size(77, 57);
            this.statusBtn.TabIndex = 0;
            this.statusBtn.UseVisualStyleBackColor = false;
            this.statusBtn.Click += new System.EventHandler(this.StatusBtn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(95, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(144, 24);
            this.label1.TabIndex = 1;
            this.label1.Text = "Current status:";
            // 
            // statusLabel
            // 
            this.statusLabel.AutoSize = true;
            this.statusLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.statusLabel.ForeColor = System.Drawing.Color.White;
            this.statusLabel.Location = new System.Drawing.Point(134, 30);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(66, 24);
            this.statusLabel.TabIndex = 2;
            this.statusLabel.Text = "Online";
            // 
            // closeStatus
            // 
            this.closeStatus.Image = global::LanSharing.Properties.Resources.close;
            this.closeStatus.Location = new System.Drawing.Point(262, 12);
            this.closeStatus.Name = "closeStatus";
            this.closeStatus.Size = new System.Drawing.Size(50, 50);
            this.closeStatus.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.closeStatus.TabIndex = 3;
            this.closeStatus.TabStop = false;
            this.closeStatus.Click += new System.EventHandler(this.CloseStatus_Click);
            // 
            // Alert
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(324, 70);
            this.Controls.Add(this.closeStatus);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.statusBtn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Alert";
            this.Opacity = 0.9D;
            this.Text = "Alert";
            this.Load += new System.EventHandler(this.Alert_Load);
            ((System.ComponentModel.ISupportInitialize)(this.closeStatus)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button statusBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label statusLabel;
        private System.Windows.Forms.PictureBox closeStatus;
    }
}