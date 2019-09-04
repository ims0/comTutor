namespace WindowsForms
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.search = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.m_word = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // search
            // 
            this.search.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.search.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.search.Location = new System.Drawing.Point(282, 25);
            this.search.Name = "search";
            this.search.Size = new System.Drawing.Size(82, 30);
            this.search.TabIndex = 0;
            this.search.Text = "英译汉";
            this.search.UseCompatibleTextRendering = true;
            this.search.UseVisualStyleBackColor = false;
            this.search.Click += new System.EventHandler(this.en_to_ch);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.Location = new System.Drawing.Point(27, 79);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(471, 210);
            this.textBox1.TabIndex = 1;
            // 
            // m_word
            // 
            this.m_word.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.m_word.Location = new System.Drawing.Point(77, 29);
            this.m_word.Name = "m_word";
            this.m_word.Size = new System.Drawing.Size(161, 26);
            this.m_word.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.button1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(388, 25);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 9, 3, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(82, 30);
            this.button1.TabIndex = 3;
            this.button1.Text = "汉译英";
            this.button1.UseCompatibleTextRendering = true;
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.ch_to_en);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.ClientSize = new System.Drawing.Size(526, 311);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.m_word);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.search);
            this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Name = "Form1";
            this.Text = "Dict";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Click += new System.EventHandler(this.en_to_ch);
            this.Enter += new System.EventHandler(this.en_to_ch);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form1_KeyPress);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button search;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox m_word;
        private System.Windows.Forms.Button button1;
    }
}

