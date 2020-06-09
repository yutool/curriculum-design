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
    public partial class RegisterForm : Form
    {
        private LoginForm loginForm;

        public RegisterForm(LoginForm loginForm)
        {
            InitializeComponent();
            this.loginForm = loginForm;
            loginForm.Hide();
        }

        /**
         * 关闭窗口
         */
        private void RegisterForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            loginForm.Show();
        }

        /**
         * 注册按钮
         */
        private void button1_Click(object sender, EventArgs e)
        {
            string account = this.account.Text;
            string password = this.password.Text;
            string password2 = this.textBox1.Text;
            if ("".Equals(account) || "".Equals(password))
            {
                MessageBox.Show("用户名或密码不能为空");
                return;
            }
            if (!password.Equals(password2))
            {
                MessageBox.Show("密码和确认密码不一致");
                return;
            }
            DataRow dr = MySqlHelper.GetDataRow(string.Format("select * from user where username = '{0}'", account));
            if (dr != null)
            {
                MessageBox.Show("该用户名已被占用");
                return;
            }
            MySqlHelper.DoInsert("user", new string[] { "username", "password" }, new object[] { account, password });
            MessageBox.Show("注册成功");
            this.Close();
            loginForm.Show();
        }
    }
}
