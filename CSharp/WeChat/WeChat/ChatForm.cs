using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WeChat
{
    public partial class ChatForm : Form
    {
        // 当前用户
        private User user;
        // 报存下登录框
        private LoginForm loginForm;
        // 客户端
        private TcpClient client;
        // 输入流
        private StreamWriter sw;
        // 好友列表
        private List<Friend> friends = new List<Friend>();
        // 选择的好友
        private Friend choose;
        // 聊天记录
        private Dictionary<string, List<Message>> chatRecords = new Dictionary<string, List<Message>>();
        // 消息列表
        private Dictionary<string, int> messageDic = new Dictionary<string, int>();

        public ChatForm(LoginForm loginForm, User user)
        {
            InitializeComponent();
            this.loginForm = loginForm;
            this.user = user;
            loginForm.Hide();
            // 修改标题
            this.Text = "WeChat - 当前在线用户：" + user.Username;
            // 开启客户端
            new Thread(OpenClient).Start();
            // 渲染好友
            drawFriendList();
            // 渲染聊天记录
            handlerChat();
        }

        /**
         * 打开客户端
         */
        private void OpenClient()
        {
            // Parse将字符串转换为IP地址类型
            IPAddress localIp = IPAddress.Parse("127.0.0.1");
            // 构造一个TcpClient类对象,TCP客户端
            client = new TcpClient();
            // 与TCP服务器连接
            client.Connect(localIp, 9999);
            Console.WriteLine("服务器已经连接...请输入对话内容...");

            // 创建网络流,获取数据流
            NetworkStream stream = client.GetStream();
            //读数据流对象
            StreamReader sr = new StreamReader(stream);
            // 写数据流对象
            sw = new StreamWriter(stream);
            // 将当前用户发送给服务器
            sw.WriteLine(user.Username);
            sw.Flush();
            // 接收消息
            while (true)
            {
                string msg = sr.ReadLine();
                Message message = Message.convertFor(msg);
                addChat(message);
            }
        }

        /**
         * 切换标签
         */
        private void tabControl1_Click(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedTab == this.tabPage1)
            {
                drawFriendList();
            }
            else if (this.tabControl1.SelectedTab == this.tabPage2)
            {
                drawMessageList();
            }
            else if (this.tabControl1.SelectedTab == this.tabPage3)
            {
                drawSystemList();
            }
        }

        /**
         * 渲染好友列表
         */
        private void drawFriendList()
        {
            this.friends.Clear();
            string sql = String.Format("select * from friend where (username = '{0}' or friendname = '{0}') and status = 1", user.Username);
            DataTable dt = MySqlHelper.GetDataTable(sql);
            foreach(DataRow dr in dt.Rows)
            {
                // 添加至好友列表
                Friend friend = new Friend(dr["username"] + "", dr["friendname"] + "");
                string friendname = friend.Username.Equals(user.Username) ? friend.Friendname : friend.Username;
                friend.Username = user.Username;
                friend.Friendname = friendname;
                friends.Add(friend);
            }
            updateFriendList();
            if (choose == null)
                handlerFriend(0);
        }

        /**
         * 修改好友列表显示信息
         */
         private void updateFriendList()
        {
            this.friendList.Items.Clear();
            foreach (Friend friend in friends)
            {
                string friendname = friend.Friendname;
                ListViewItem item = new ListViewItem();
                string msg = "暂无消息...";
                if (!chatRecords.ContainsKey(friendname))
                {
                    chatRecords.Add(friendname, new List<Message>());
                }
                if (chatRecords[friendname].Count > 0)
                {
                    string content = chatRecords[friendname][chatRecords[friendname].Count - 1].Content;
                    int len = content.Length > 10 ? 10 : content.Length;
                    msg = content.Substring(0, len) + "...";
                }
                item.Text = friendname + ": " + msg;
                this.friendList.Items.Add(item);
            }
        }

        /**
         * 渲染未收消息
         */
        private void drawMessageList()
        {
            this.messageList.Items.Clear();
            foreach (string name in messageDic.Keys)
            {
                ListViewItem item = new ListViewItem();
                item.Text = name + " 的未收消息（" + messageDic[name] + "）";
                this.messageList.Items.Add(item);
            }
        }

        /**
         * 渲染系统信息列表
         */
        private void drawSystemList()
        {
            this.systemList.Items.Clear();
            string sql = string.Format("select * from friend where friendname = '{0}' and status = 0", user.Username);
            DataTable dt = MySqlHelper.GetDataTable(sql);
            foreach (DataRow dr in dt.Rows)
            {
                ListViewItem item = new ListViewItem();
                item.Text = dr["username"] + " 的好友请求";
                this.systemList.Items.Add(item);
            }
        }

        /**
         * 好友列表点击事件
         */
        private void friendList_Click(object sender, EventArgs e)
        {
            // 切换至该好友
            int count = this.friendList.FocusedItem.Index;
            handlerFriend(count);
            // 如果有该好友的未收消息删除了
            string text = this.friendList.FocusedItem.Text;
            string friendname = text.Substring(0, text.IndexOf(":"));
            messageDic.Remove(friendname);
        }

        /**
         * 好友请求点击事件
         */
        private void systemList_Click(object sender, EventArgs e)
        {
            string text = this.systemList.FocusedItem.Text;
            string friendname = text.Substring(0, text.LastIndexOf(" "));
            DialogResult AF = MessageBox.Show("确定添加 " + friendname + " 吗？", "WeChat", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (AF == DialogResult.OK)
            {
                MySqlHelper.DoUpdate("friend", new string[] { "status" }, new object[] { 1 },
                    new string[] { "username", "friendname" }, new object[] { friendname, user.Username });
            } else
            {
                MySqlHelper.DoDelete("friend", new string[] { "username", "friendname" }, new object[] { friendname, user.Username });
            }
            this.systemList.Items.Remove(this.systemList.FocusedItem);
        }


        /**
         * 未读消息点击事件
         */
        private void messageList_Click(object sender, EventArgs e)
        {
            string text = this.messageList.FocusedItem.Text;
            string friendname = text.Substring(0, text.LastIndexOf(" "));
            // 切换选中的好友
            foreach (Friend friend in friends) if (friend.Friendname.Equals(friendname))
            {
                choose = friend;
                break;
            }
            // 移除消息
            messageDic.Remove(friendname);
            // 渲染消息列表
            drawMessageList();
            // 渲染聊天记录
            handlerChat();
        }

        /**
         * 好友切换
         */
        private void handlerFriend(int count)
        {
            if (count >= friends.Count)
            {
                return;
            }
            // 切换好友并渲染聊天记录
            choose = friends[count];
            handlerChat();
        }

        /**
         * 发送聊天信息
         */
        private void sendBtn_Click(object sender, EventArgs e)
        {
            string content = this.inputBox.Text;
            if (content == null || "".Equals(content)) {
                return;
            }
            if(choose == null) {
                MessageBox.Show("请选择好友");
                return;
            }
            Message message = new Message(user.Username, choose.Friendname, content);
            // 添加至聊天记录
            chatRecords[choose.Friendname].Add(message);
            handlerChat();
            // 刷新下列表
            updateFriendList();

            // 发送值服务器
            this.inputBox.Text = "";
            sw.WriteLine(message.toJson());
            sw.Flush();
        }

        /**
         * 渲染聊天记录
         */
        private void handlerChat()
        {
            if(choose == null) {
                return;
            }
            this.label1.Text = "和 " + choose.Friendname + " 聊天中...";
            // 获取聊天记录并格式化渲染
            List<Message> messages = chatRecords[choose.Friendname];
            string chat = "";
            foreach (Message msg in messages) {
                chat += formatMsg(msg);
            }
            this.outputBox.Text = chat;
        }

        /**
         * 存储收到的消息
         */
        private void addChat(Message message)
        {
            chatRecords[message.From].Add(message);
            if (message.From.Equals(choose.Friendname))
            {
                handlerChat(); // 直接渲染至列表
            } else 
            {
                if (messageDic.ContainsKey(message.From))
                {
                    messageDic[message.From]++;
                } else
                {
                    messageDic.Add(message.From, 1);
                }
            }
            updateFriendList();
        }

        /**
         * 格式化聊天记录
         */
        private string formatMsg(Message message)
        {
            int width = this.inputBox.Width;
            int tabNum = (width / 5 - wordCount(message.Content)) / 10;
            // 系统消息
            if (message.Type == 0)
            {
                return "\t\t\t  系统提示：" + message.Content + "\r\n";
            }
            string msg;
            // 自己发的消息
            if (message.From.Equals(user.Username))
            {
                msg = String.Format("{0, 80}", message.From + " " + message.formatTime()) + "\r\n";
                for (int i = 0; i < tabNum; i++)
                {
                    msg += "\t";
                }
                msg += message.Content + "\r\n";
            } 
            // 别人发的消息
            else
            {
                msg = "     " + message.From + " " + message.formatTime() + "\r\n";
                msg += "\t" + message.Content + "\r\n";
            }
            return msg;
        }

        /**
         * 添加好友
         */
        private void addFriendBtn_Click(object sender, EventArgs e)
        {
            string str = Interaction.InputBox("添加好友", "WeChat", "", -1, -1);
            string sql1 = string.Format("SELECT * FROM user WHERE username = '{0}'", str);
            string sql2 = string.Format("SELECT * FROM friend WHERE username='{0}' AND friendname='{1}' OR username='{1}' AND friendname='{0}'", user.Username, str);
            if (str == null || "".Equals(str)) {
                return;
            }
            if (str.Equals(user.Username))
            {
                MessageBox.Show("不可添加自己为好友");
                return;
            }
            if (MySqlHelper.GetDataRow(sql1) == null) {
                MessageBox.Show("该用户不存在");
                return;
            }
            if (MySqlHelper.GetDataRow(sql2) != null) {
                MessageBox.Show("该用户已是您的好友");
                return;
            }
            MySqlHelper.DoInsert("friend", new string[] { "username","friendname","status" }, new object[] {user.Username, str, 0 });
        }

        
        /**
         * 删除好友
         */
        private void delFriendBtn_Click(object sender, EventArgs e)
        {
            DialogResult AF = MessageBox.Show("确定删除 " + choose.Friendname + " 吗？", "WeChat", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (AF == DialogResult.OK)
            {
                MySqlHelper.DoDelete("friend", new string[] { "username", "friendname" }, new object[] { user.Username, choose.Friendname });
                MySqlHelper.DoDelete("friend", new string[] { "username", "friendname" }, new object[] { choose.Friendname, user.Username });
                // 将选中的好友重置，并重新渲染好友列表
                choose = null;
                drawFriendList();
            }
        }

        /**
         * 获取字符串长度
         * 中文2 其他1个
         */
        private int wordCount(string str)
        {
            int count = 0;
            for (int i = 0; i < str.Length; i++)
            {
                if (Char.ConvertToUtf32(str, i) >= Convert.ToInt32("4e00", 16) && Char.ConvertToUtf32(str, i) <= Convert.ToInt32("9fff", 16))
                {
                    count += 2; //统计汉字
                } else
                {
                    count++;
                }
            }
            return count;
        }

        /**
         *  窗口关闭后，把登录窗口关了
         */
        private void ChatForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            loginForm.Close();
            client.Close();
        }

        private void inputBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.Enter)
            {
            }
            else if (e.KeyCode == Keys.Enter)
            {
                sendBtn_Click(sender, e);
                e.Handled = true;
            }
        }
    }
}
