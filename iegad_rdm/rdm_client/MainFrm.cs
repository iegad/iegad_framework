using rdm_client.proxy;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace rdm_client
{
    public partial class MainFrm : Form
    {
        public MainFrm()
        {
            InitializeComponent();
            ClientProxy.SetEvironemnt("127.0.0.1", 6688);
            ClientProxy.Instance.Open();
        }

        private void mnItem_Connection_Click(object sender, EventArgs e)
        {
            ConnectionFrm fm = new ConnectionFrm();
            fm.ShowDialog();
        }

        private void MainFrm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ClientProxy.Instance.Close();
        }
    }
}
