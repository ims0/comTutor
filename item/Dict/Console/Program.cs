using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
 

using System.Net;

using System.IO;
 
namespace Consoles
{
    class Program
    {
        static void Main(string[] args)
        {

             try {

                WebClient MyWebClient = new WebClient();

        
                MyWebClient.Credentials = CredentialCache.DefaultCredentials;//获取或设置用于向Internet资源的请求进行身份验证的网络凭据

                Byte[] pageData = MyWebClient.DownloadData("http://dict.youdao.com/search?q=大"); //从指定网站下载数据

                //string pageHtml = Encoding.Default.GetString(pageData);  //如果获取网站页面采用的是GB2312，则使用这句            
                //StreamReader sr = new System.IO.StreamReader(pageData, System.Text.Encoding.UTF8);
               string pageHtml = Encoding.UTF8.GetString(pageData); //如果获取网站页面采用的是UTF-8，则使用这句

                //Console.WriteLine(pageHtml);//在控制台输入获取的内容
                int pos= pageHtml.IndexOf("class=\"trans-container\"");

 
                 string sub=pageHtml.Substring(pos+25);
                 int pos1 = sub.IndexOf("div");
 
                 string strout=sub.Substring(0,pos1);
                 strout = strout.Replace("<li>", "");
                 strout = strout.Replace("</li>", "");
                 strout = strout.Replace("<ul>", "");
                 strout = strout.Replace("</ul>", "");
                 string sout="";
                 while(true)
                 {
                     int site  =strout.IndexOf("E2Ctranslation");
                     Console.WriteLine("site ");//在控制台输入获取的内容
                     Console.WriteLine(site);//在控制台输入获取的内容
                     if (site == -1)
                         break;
                    int  site2 = strout.IndexOf("</a>");
                    Console.WriteLine("site2 ");//在控制台输入获取的内容
                    Console.WriteLine(site2);//在控制台输入获取的内容
                    sout += strout.Substring(site+16, site2-site-16)+"\r\n";
                    strout = strout.Substring(site2 + 5);
                 }
      

                 Console.WriteLine(sout);//在控制台输入获取的内容
               // using (StreamWriter sw = new StreamWriter("g:\\ouput.html"))//将获取的内容写入文本

                //{

                //    sw.Write(pageHtml);

                //}

                Console.ReadLine(); //让控制台暂停,否则一闪而过了             

            }

            catch(WebException webEx) {

                Console.WriteLine(webEx.Message.ToString());

            }


        }
    }
}
