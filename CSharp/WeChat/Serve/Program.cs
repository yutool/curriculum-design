using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace Serve
{
    class Program
    {
        private static TcpListener server;

        private static Dictionary<string, StreamWriter> clients = new Dictionary<string, StreamWriter>();

        static void Main(string[] args)
        {
            // Parse将字符串转换为IP地址类型
            IPAddress localIp = IPAddress.Parse("127.0.0.1");
            // 构造一个TcpListener(IP地址,端口)对象,TCP服务端
            server = new TcpListener(localIp, 9999);

            // 开始监听
            server.Start();
            Console.WriteLine("服务端启动成功...");

            // 处理客户端
            Thread thread = new Thread(ReceiveClient);
            thread.Start();
            
        }

        /**
         *  接收客户端
         */
        private static void ReceiveClient()
        {
            while (true)
            {
                TcpClient client = server.AcceptTcpClient();
                if (client != null)
                {
                    // 构造NetworkStream类,该类用于获取和操作网络流
                    NetworkStream stream = client.GetStream();
                    // 读数据流对象
                    StreamReader sr = new StreamReader(stream);
                    // 写数据流对象
                    StreamWriter sw = new StreamWriter(stream);
                   
                    // 读取连线用户
                    string username = sr.ReadLine();
                    clients.Add(username, sw);

                    Console.WriteLine("用户 {0} 已连接，目前在线人数：{1}", username, clients.Count);

                    // 开启线程接收客户端消息
                    Task.Run(() => {
                        while (true)
                        {
                            string msg = sr.ReadLine();
                            
                            // 关闭则回到null
                            if (msg == null)
                            {
                                clients.Remove(username);
                                client.Close();
                                Console.WriteLine("用户 {0} 已退出，目前在线人数：{1}", username, clients.Count);
                                return;
                            }
                            Console.WriteLine("接收到客户端消息：" + msg);
                            
                            // 转发给对应的用户
                            Message message = Message.convertFor(msg);
                            if (!clients.ContainsKey(message.To))
                            {
                                Message back = new Message(message.To, message.From, "对方不在线，稍后联系...", 0);
                                sw.WriteLine(back.toJson());
                                sw.Flush();
                                continue;
                            }
                            clients[message.To].WriteLine(msg);
                            clients[message.To].Flush();
                        }
                    });
                }
            }
        }
    }
}
