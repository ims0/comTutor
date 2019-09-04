using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;
namespace WindowsForms
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void en_to_ch(object sender, EventArgs e)
        {
            try
            {
                textBox1.Text = "";
                if (m_word.Text.Trim() == "")
                {
                    textBox1.Text = "     无结果";
                    return;
                }
                WebClient MyWebClient = new WebClient();

                MyWebClient.Credentials = CredentialCache.DefaultCredentials;//获取或设置用于向Internet资源的请求进行身份验证的网络凭据

                string url = "http://dict.youdao.com/search?q=" + m_word.Text.Trim();
                Byte[] pageData = MyWebClient.DownloadData(url); //从指定网站下载数据

                //string pageHtml = Encoding.Default.GetString(pageData);  //如果获取网站页面采用的是GB2312，则使用这句            
                //StreamReader sr = new System.IO.StreamReader(pageData, System.Text.Encoding.UTF8);
                string pageHtml = Encoding.UTF8.GetString(pageData); //如果获取网站页面采用的是UTF-8，则使用这句
                //using (StreamWriter sw = new StreamWriter("output1.html"))//将获取的内容写入文本
                //{
                //    sw.Write(pageHtml);
                //}

                //get pronounce
                string pronstr = pageHtml;
                int p1 = pronstr.IndexOf("\"phonetic\"");
                string phone = "";

                if (p1 > 0)
                {

                    pronstr = pronstr.Substring(p1 + 11);
                    int p2 = pronstr.IndexOf("</span>");
                    if (p2 > 0)
                    {
                        phone = "     英：";
                        phone += pronstr.Substring(0, p2) + "     ";

                    }
                   // MessageBox.Show(p1.ToString());
                }
               // else
                // m_word.Text = p1.ToString();--内容结束--

                p1 = pronstr.IndexOf("\"phonetic\"");

                if (p1 > 0)
                {
                    pronstr = pronstr.Substring(p1 + 11);
                    int p2 = pronstr.IndexOf("</span>");
                    if (p2 > 0)
                        phone += "美：" + pronstr.Substring(0, p2) + "\r\n";
                }
             
                int pos = pageHtml.IndexOf("class=\"trans-container\"");
                if (pos == -1)
                {
                    textBox1.Text = "无结果";
                    return;
                }
                int pauth = pageHtml.IndexOf("\"trans-container\" id=\"authDictTrans\"");
                if (pauth > 0)
                {
                    int pend = pageHtml.IndexOf("--内容结束--");
                    string sauth = pageHtml.Substring(pauth + 50, pend - pauth);
                    while (true)
                    {
                        int pl = sauth.IndexOf("<");
                        if (pl == -1)
                            break;
                        int pr = sauth.IndexOf(">");
                        if (pr == -1)
                            break;
                        if (pl < pr)
                            sauth = sauth.Remove(pl, pr - pl + 1);
                        else
                            break;
                    }

                    char[] sep = { '\r' };
                    string[] arr1;
                    arr1 = sauth.Split(sep);  //分解为数组
                    sauth = "";
                    foreach (string i in arr1)
                    {
                        sauth += "     " + i.Trim();
                    }
                    sauth = Regex.Replace(sauth, @"\s+", " ");
                    sauth = sauth.Replace("。", "。\r\n");
                    sauth = sauth.Replace(".", ".\r\n");
                    sauth = sauth.Replace(",", ", ");
                    sauth = sauth.Replace("，", "， ");
                    textBox1.Text =   sauth.Trim();
                    return;
                }

                string container = pageHtml.Substring(pos + 25);  //截取开始str
                int posdiv = container.IndexOf("</div>");

                container = container.Substring(0, posdiv - 3);// 获取结果,以第一个"</div>"结尾
             
                int p_addit = container.IndexOf("class=\"additional\"");

                //str=id="authDictTrans"《21世纪大英汉词典》
                if (p_addit == -1)  //无附加解释
                {


                   container = container.Replace("<li>", "");
                   container = container.Replace("</li>", "");
                   container = container.Replace("<ul>", "");
                   container = container.Replace("</ul>", "");      
                   container = container.Replace("\n", "\r\n");
   
                   textBox1.Text =phone+"\r\n     "  + container.Trim();
                   //MessageBox.Show("no add");
                   return; //end
                }
               int p_ul = container.IndexOf("</ul>");
               string part1 = container.Substring(0, p_ul );
               part1 = part1.Replace("<li>", "");
               part1 = part1.Replace("</li>", "");
               part1 = part1.Replace("<ul>", "");
               part1 = part1.Replace("</ul>", "");
               part1 = part1.Replace("\n", "\r\n");
               string part2 = container.Substring(p_addit + 20);  //附加解释
 
                part2 = part2.Replace("[", "");
                part2 = part2.Replace("]", "");
                // char[] sep = { '\r',' ' };
                part2 = part2.Trim( );
                char[] separator = { '\n' };
                string []arr;
                arr = part2.Split(separator);  //分解为数组
                part2 = "";
                int f = 0;
                foreach (string i in arr)
                {
                    f++;
                    part2 += "     " + i.Trim();
                    if (f % 2 == 0)
                        part2 = part2 + "\r\n";
                }
                part2 = part2.Substring(0, part2.Length - 4);
                textBox1.Text =phone+"\r\n     "+ part1.Trim()+ "\r\n\r\n"+part2;
            }
            catch (WebException webEx)
            {
               textBox1.Text=webEx.Message.ToString();
            }        
        }

        private void ch_to_en(object sender, EventArgs e)
        {
            try
            {
                textBox1.Text = "";
                if (m_word.Text.Trim() == "")
                {
                    textBox1.Text = "     无结果";
                    return;
                }
                WebClient MyWebClient = new WebClient();


                MyWebClient.Credentials = CredentialCache.DefaultCredentials;//获取或设置用于向Internet资源的请求进行身份验证的网络凭据

                string url = "http://dict.youdao.com/search?q=" + m_word.Text.Trim();
                Byte[] pageData = MyWebClient.DownloadData(url); //从指定网站下载数据

                //string pageHtml = Encoding.Default.GetString(pageData);  //如果获取网站页面采用的是GB2312，则使用这句            
                //StreamReader sr = new System.IO.StreamReader(pageData, System.Text.Encoding.UTF8);
                string pageHtml = Encoding.UTF8.GetString(pageData); //如果获取网站页面采用的是UTF-8，则使用这句

                int pos = pageHtml.IndexOf("class=\"trans-container\"");


                string sub = pageHtml.Substring(pos + 25);
                int pos1 = sub.IndexOf("div");

                string strout = sub.Substring(0, pos1);
                strout = strout.Replace("<li>", "");
                strout = strout.Replace("</li>", "");
                strout = strout.Replace("<ul>", "");
                strout = strout.Replace("</ul>", "");
                string sout = "";
                while (true)
                {
                    int site = strout.IndexOf("E2Ctranslation");
                    Console.WriteLine("site ");//在控制台输入获取的内容
                    Console.WriteLine(site);//在控制台输入获取的内容
                    if (site == -1)
                        break;
                    int site2 = strout.IndexOf("</a>");
                    Console.WriteLine("site2 ");//在控制台输入获取的内容
                    Console.WriteLine(site2);//在控制台输入获取的内容
                    sout += strout.Substring(site + 16, site2 - site - 16) + "\r\n";
                    strout = strout.Substring(site2 + 5);
                }
                textBox1.Text = sout;
            }

            catch (WebException webEx)
            {
                textBox1.Text = webEx.Message.ToString();
            }
        }
 
       
        //焦点集中到输入框
        private void Form1_Load(object sender, EventArgs e)
        {
            m_word.Focus();
            m_word.Select();
             
        }
        //需要在窗体上注册控件的键盘事件，把窗体的KeyPreview属性设置成 true；
        //然后才能响应Enter
        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((Keys)e.KeyChar == Keys.Enter)
            {
                en_to_ch(sender, e);
            }
        }

    }
}
