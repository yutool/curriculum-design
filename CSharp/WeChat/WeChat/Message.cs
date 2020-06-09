using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WeChat
{
    public class Message
    {
        private string from;
        private string to;
        private DateTime createTime;
        private string content;
        private int type; // 0 - 系统， 1 - 用户

        public Message() { }

        public Message(string from, string to, string content)
            : this(from, to, content, 1) { }

        public Message(string from, string to, string content, int type)
        {
            this.From = from;
            this.To = to;
            this.CreateTime = DateTime.Now;
            this.Content = content;
            this.Type = type;
        }

        public string toJson()
        {
            return JsonConvert.SerializeObject(this);
        }

        public static Message convertFor(string message)
        {
            return JsonConvert.DeserializeObject<Message>(message);
        }

        public string formatTime()
        {
            return string.Format("{0:dddd tt hh:mm:ss}", createTime);
        }


        public string From { get => from; set => from = value; }
        public string To { get => to; set => to = value; }
        public DateTime CreateTime { get => createTime; set => createTime = value; }
        public string Content { get => content; set => content = value; }
        public int Type { get => type; set => type = value; }
    }
}
