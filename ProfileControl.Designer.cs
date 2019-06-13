using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace LanSharing
{
    partial class ProfileControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProfileControl));
            this.label1 = new System.Windows.Forms.Label();
            this.avatarButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.usernameBox = new System.Windows.Forms.TextBox();
            this.saveUsername = new System.Windows.Forms.Button();
            this.userAvatarBox = new LanSharing.OvalPictureBox();
            this.usernameLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.userAvatarBox)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 26.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(498, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(114, 42);
            this.label1.TabIndex = 0;
            this.label1.Text = "Profile";
            // 
            // avatarButton
            // 
            this.avatarButton.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.avatarButton.Location = new System.Drawing.Point(7, 344);
            this.avatarButton.Name = "avatarButton";
            this.avatarButton.Size = new System.Drawing.Size(152, 28);
            this.avatarButton.TabIndex = 3;
            this.avatarButton.Text = "Select a picture";
            this.avatarButton.UseVisualStyleBackColor = true;
            this.avatarButton.Click += new System.EventHandler(this.Button1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(469, 217);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(118, 25);
            this.label2.TabIndex = 4;
            this.label2.Text = "Username";
            // 
            // usernameBox
            // 
            this.usernameBox.Location = new System.Drawing.Point(442, 264);
            this.usernameBox.Name = "usernameBox";
            this.usernameBox.Size = new System.Drawing.Size(165, 20);
            this.usernameBox.TabIndex = 5;
            // 
            // saveUsername
            // 
            this.saveUsername.Location = new System.Drawing.Point(613, 264);
            this.saveUsername.Name = "saveUsername";
            this.saveUsername.Size = new System.Drawing.Size(75, 20);
            this.saveUsername.TabIndex = 6;
            this.saveUsername.Text = "Save";
            this.saveUsername.UseVisualStyleBackColor = true;
            this.saveUsername.Click += new System.EventHandler(this.SaveUsername_Click);
            // 
            // userAvatarBox
            // 
            this.userAvatarBox.BackColor = System.Drawing.Color.DarkGray;
            this.userAvatarBox.Image = ((System.Drawing.Image)(resources.GetObject("userAvatarBox.Image")));
            this.userAvatarBox.Location = new System.Drawing.Point(7, 186);
            this.userAvatarBox.Name = "userAvatarBox";
            this.userAvatarBox.Size = new System.Drawing.Size(152, 152);
            this.userAvatarBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.userAvatarBox.TabIndex = 2;
            this.userAvatarBox.TabStop = false;
            // 
            // usernameLabel
            // 
            this.usernameLabel.AutoSize = true;
            this.usernameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.usernameLabel.Location = new System.Drawing.Point(593, 217);
            this.usernameLabel.Name = "usernameLabel";
            this.usernameLabel.Size = new System.Drawing.Size(0, 25);
            this.usernameLabel.TabIndex = 7;
            // 
            // ProfileControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.usernameLabel);
            this.Controls.Add(this.saveUsername);
            this.Controls.Add(this.usernameBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.avatarButton);
            this.Controls.Add(this.userAvatarBox);
            this.Controls.Add(this.label1);
            this.Name = "ProfileControl";
            this.Size = new System.Drawing.Size(1130, 552);
            ((System.ComponentModel.ISupportInitialize)(this.userAvatarBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private OvalPictureBox userAvatarBox;
        private Button avatarButton;
        private Label label2;
        private TextBox usernameBox;
        private Button saveUsername;
        private Label usernameLabel;
    }
}
