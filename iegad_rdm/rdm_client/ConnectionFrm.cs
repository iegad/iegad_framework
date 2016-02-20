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
    public partial class ConnectionFrm : Form
    {
        public ConnectionFrm()
        {
            InitializeComponent();            
        }

        private bool ConnectRedis(string ipString, int port)
        {
            return ClientProxy.Instance.ConnectToRedis(ipString, port);
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (txtIPString.Text.Trim() == string.Empty || txtPort.Text.Trim() == string.Empty)
            {
                return;
            }
            if (!ConnectRedis(txtIPString.Text.Trim(), Convert.ToInt32(txtPort.Text.Trim())))
            { 
                MessageBox.Show("Connected failed");
                return;
            }
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
