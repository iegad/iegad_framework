using rdm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Thrift.Protocol;
using Thrift.Transport;

namespace rdm_client.proxy
{
    class ClientProxy
    {
        public static void SetEvironemnt(string ipString, int port)
        {
            ipString_ = ipString;
            port_ = port;
        }

        public static ClientProxy Instance 
        {
            get
            {
                if (handler_ == null)
                {
                    if (ipString_ == null) { return null; }
                    else { handler_ = new ClientProxy(); }
                }
                return handler_;
            }
        }

        public bool Open(string errString = "")
        {
            try
            {
                trans_.Open();
                return true;
            }
            catch(Exception ex)
            {
                errString = ex.Message;
                return false;
            }
        }

        public void Close(string errString = "")
        {
            try
            {
                if (trans_.IsOpen)
                {
                    trans_.Close();
                }
            }
            catch(Exception ex)
            {
                errString = ex.Message;
            }
        }

        public bool ConnectToRedis(string ipString, int port, string errString = "", string usrId = "", string pwd = "")
        {
            try
            {
                if (!trans_.IsOpen) { trans_.Open(); }
                return client_.Connect(ipString, port, usrId, pwd);
            }
            catch (Exception ex)
            {
                errString = ex.Message;
                return false;
            }
        }

        public List<RedisData_t> GetKeys(string queryStr, string errString = "")
        {
            try
            {
                if (!trans_.IsOpen) { trans_.Open(); }
                return client_.GetKeys(queryStr);
            }
            catch (Exception ex)
            {
                errString = ex.Message;
                return null;
            }
        }

        private ClientProxy()
        {
            trans_ = new TSocket(ipString_, port_);
            protoc_ = new TBinaryProtocol(trans_);
            client_ = new rdmServer.Client(protoc_);
        }

        private static string ipString_;
        private static int port_;
        private static ClientProxy handler_;


        private TTransport trans_;
        private TProtocol protoc_;        
        private rdmServer.Client client_;
    }
}
