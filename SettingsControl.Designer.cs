namespace LanSharing
{
    partial class SettingsControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.downloadPath = new System.Windows.Forms.Label();
            this.dwnPathButton = new System.Windows.Forms.Button();
            this.downloadCheck = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(498, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(134, 42);
            this.label1.TabIndex = 2;
            this.label1.Text = "Settings";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 169);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(149, 24);
            this.label2.TabIndex = 3;
            this.label2.Text = "Download path";
            // 
            // downloadPath
            // 
            this.downloadPath.AutoSize = true;
            this.downloadPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.downloadPath.Location = new System.Drawing.Point(158, 169);
            this.downloadPath.Name = "downloadPath";
            this.downloadPath.Size = new System.Drawing.Size(28, 24);
            this.downloadPath.TabIndex = 4;
            this.downloadPath.Text = "C:";
            // 
            // dwnPathButton
            // 
            this.dwnPathButton.Location = new System.Drawing.Point(153, 197);
            this.dwnPathButton.Name = "dwnPathButton";
            this.dwnPathButton.Size = new System.Drawing.Size(139, 23);
            this.dwnPathButton.TabIndex = 5;
            this.dwnPathButton.Text = "Select download path";
            this.dwnPathButton.UseVisualStyleBackColor = true;
            this.dwnPathButton.Click += new System.EventHandler(this.DwnPathButton_Click);
            // 
            // downloadCheck
            // 
            this.downloadCheck.AutoSize = true;
            this.downloadCheck.Checked = true;
            this.downloadCheck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.downloadCheck.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.downloadCheck.Location = new System.Drawing.Point(7, 226);
            this.downloadCheck.Name = "downloadCheck";
            this.downloadCheck.Size = new System.Drawing.Size(253, 28);
            this.downloadCheck.TabIndex = 6;
            this.downloadCheck.Text = "Automatically accept file";
            this.downloadCheck.UseVisualStyleBackColor = true;
            this.downloadCheck.CheckedChanged += new System.EventHandler(this.DownloadCheck_CheckedChanged);
            // 
            // SettingsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.downloadCheck);
            this.Controls.Add(this.dwnPathButton);
            this.Controls.Add(this.downloadPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "SettingsControl";
            this.Size = new System.Drawing.Size(1130, 552);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label downloadPath;
        private System.Windows.Forms.Button dwnPathButton;
        private System.Windows.Forms.CheckBox downloadCheck;
    }
}
