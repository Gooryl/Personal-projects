namespace CNV2
{
    partial class Form1
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
            this.Refresh_COM_list = new System.Windows.Forms.Button();
            this.COM_list = new System.Windows.Forms.ComboBox();
            this.Connect = new System.Windows.Forms.Button();
            this.messageLabel = new System.Windows.Forms.Label();
            this.Send = new System.Windows.Forms.Button();
            this.readData = new System.Windows.Forms.Label();
            this.registerChecks = new System.Windows.Forms.CheckedListBox();
            this.messageText = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SynthFreq = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.SynthFreq)).BeginInit();
            this.SuspendLayout();
            // 
            // Refresh_COM_list
            // 
            this.Refresh_COM_list.Location = new System.Drawing.Point(211, 106);
            this.Refresh_COM_list.Name = "Refresh_COM_list";
            this.Refresh_COM_list.Size = new System.Drawing.Size(98, 21);
            this.Refresh_COM_list.TabIndex = 0;
            this.Refresh_COM_list.Text = "Обновить";
            this.Refresh_COM_list.UseVisualStyleBackColor = true;
            this.Refresh_COM_list.Click += new System.EventHandler(this.button1_Click);
            // 
            // COM_list
            // 
            this.COM_list.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.COM_list.FormattingEnabled = true;
            this.COM_list.Location = new System.Drawing.Point(29, 72);
            this.COM_list.Name = "COM_list";
            this.COM_list.Size = new System.Drawing.Size(154, 21);
            this.COM_list.TabIndex = 1;
            // 
            // Connect
            // 
            this.Connect.Location = new System.Drawing.Point(211, 72);
            this.Connect.Name = "Connect";
            this.Connect.Size = new System.Drawing.Size(98, 23);
            this.Connect.TabIndex = 2;
            this.Connect.Text = "Подключиться";
            this.Connect.UseVisualStyleBackColor = true;
            this.Connect.Click += new System.EventHandler(this.Connect_Click);
            // 
            // messageLabel
            // 
            this.messageLabel.AutoSize = true;
            this.messageLabel.Location = new System.Drawing.Point(26, 114);
            this.messageLabel.Name = "messageLabel";
            this.messageLabel.Size = new System.Drawing.Size(0, 13);
            this.messageLabel.TabIndex = 4;
            // 
            // Send
            // 
            this.Send.Location = new System.Drawing.Point(46, 252);
            this.Send.Name = "Send";
            this.Send.Size = new System.Drawing.Size(115, 23);
            this.Send.TabIndex = 6;
            this.Send.Text = "Сконфигурировать";
            this.Send.UseVisualStyleBackColor = true;
            this.Send.Click += new System.EventHandler(this.Send_Click);
            // 
            // readData
            // 
            this.readData.AutoSize = true;
            this.readData.Location = new System.Drawing.Point(26, 205);
            this.readData.MaximumSize = new System.Drawing.Size(170, 0);
            this.readData.Name = "readData";
            this.readData.Size = new System.Drawing.Size(0, 13);
            this.readData.TabIndex = 7;
            // 
            // registerChecks
            // 
            this.registerChecks.Enabled = false;
            this.registerChecks.FormattingEnabled = true;
            this.registerChecks.Items.AddRange(new object[] {
            "R0",
            "R1",
            "R2",
            "R3",
            "R4",
            "R5"});
            this.registerChecks.Location = new System.Drawing.Point(234, 171);
            this.registerChecks.Name = "registerChecks";
            this.registerChecks.Size = new System.Drawing.Size(75, 94);
            this.registerChecks.TabIndex = 9;
            // 
            // messageText
            // 
            this.messageText.AutoSize = true;
            this.messageText.Location = new System.Drawing.Point(26, 301);
            this.messageText.Name = "messageText";
            this.messageText.Size = new System.Drawing.Size(0, 13);
            this.messageText.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(102, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Выбор COM-порта:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(29, 145);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Выбор частоты:";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // SynthFreq
            // 
            this.SynthFreq.DecimalPlaces = 1;
            this.SynthFreq.Increment = new decimal(new int[] {
            25,
            0,
            0,
            65536});
            this.SynthFreq.Location = new System.Drawing.Point(32, 171);
            this.SynthFreq.Maximum = new decimal(new int[] {
            4400,
            0,
            0,
            0});
            this.SynthFreq.Minimum = new decimal(new int[] {
            35,
            0,
            0,
            0});
            this.SynthFreq.Name = "SynthFreq";
            this.SynthFreq.Size = new System.Drawing.Size(120, 20);
            this.SynthFreq.TabIndex = 13;
            this.SynthFreq.Value = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(158, 173);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "МГц";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(231, 145);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(84, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Логи загрузки:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(324, 332);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.SynthFreq);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.messageText);
            this.Controls.Add(this.registerChecks);
            this.Controls.Add(this.readData);
            this.Controls.Add(this.Send);
            this.Controls.Add(this.messageLabel);
            this.Controls.Add(this.Connect);
            this.Controls.Add(this.COM_list);
            this.Controls.Add(this.Refresh_COM_list);
            this.Name = "Form1";
            this.Text = "Конфигуратор частоты";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.SynthFreq)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Refresh_COM_list;
        private System.Windows.Forms.ComboBox COM_list;
        private System.Windows.Forms.Button Connect;
        private System.Windows.Forms.Label messageLabel;
        private System.Windows.Forms.Button Send;
        private System.Windows.Forms.Label readData;
        private System.Windows.Forms.CheckedListBox registerChecks;
        private System.Windows.Forms.Label messageText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown SynthFreq;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}

