using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WeChat
{
    public class Friend
    {
        private string username;
        private string friendname;

        public Friend(string username, string friendname)
        {
            this.Username = username;
            this.Friendname = friendname;
        }

        public string Username { get => username; set => username = value; }
        public string Friendname { get => friendname; set => friendname = value; }
    }
}
