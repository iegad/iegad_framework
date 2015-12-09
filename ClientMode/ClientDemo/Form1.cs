using onepc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ClientDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        string password_;
        string email_;
        private void txtEmail_Leave(object sender, EventArgs e)
        {
            if (!Gamer.Instance.CheckEmail(txtEmail.Text.Trim()))
            {
                txtEmail.BackColor = Color.Red;
            }
            else
            {
                txtEmail.BackColor = Color.White;
                email_ = txtEmail.Text.Trim();
            }
        }

        private void txtPwd_Leave(object sender, EventArgs e)
        {
            if (txtPwd.Text.Length < 8)
            {
                txtPwd.BackColor = Color.Red;
            }
            else
            {
                txtPwd.BackColor = Color.White;
            }
        }

        private void txtRepeatPwd_Leave(object sender, EventArgs e)
        {
            if (!Gamer.Instance.CheckPassword(txtPwd.Text, txtRepeatPwd.Text))
            {
                txtPwd.BackColor = Color.Red;
                txtRepeatPwd.BackColor = Color.Red;
            }
            else
            {
                txtPwd.BackColor = Color.White;
                txtRepeatPwd.BackColor = Color.White;
                password_ = txtPwd.Text;
            }
        }

        private void btnSetUp_Click(object sender, EventArgs e)
        {
            Gamer.Instance.SetUp(email_, password_);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int ver = Gamer.Instance.GetServerVersion();
            if (ver == -1)
            {
                MessageBox.Show("世界上最遥远的距离就是没有网络");
            }
        }
    }
}
