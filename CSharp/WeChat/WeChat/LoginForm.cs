using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WeChat
{
    public partial class LoginForm : Form
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        /**
         * 登录 
         */
        private void login_Click(object sender, EventArgs e)
        {
            string username = this.account.Text;
            string password = this.password.Text;
            string sql = String.Format("select * from user where username = '{0}' and password = '{1}'", username, password);
            DataRow dr =  MySqlHelper.GetDataRow(sql);
            if (dr == null )
            {
                MessageBox.Show("用户名或者密码错误");
                return;
            }
            User user = new User(dr["username"]+"", dr["password"] + "");
            ChatForm chatForm = new ChatForm(this, user);
            chatForm.Show();
        }

        /**
         * 去注册账号
         */
        private void register_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            RegisterForm registerForm = new RegisterForm(this);
            registerForm.Show();
        }

        private void LoginForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                login_Click(sender, e);
                e.Handled = true;
            }
        }
    }
}
