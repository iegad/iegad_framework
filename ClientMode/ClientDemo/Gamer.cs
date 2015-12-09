using Onepc.AppService;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using Thrift.Protocol;
using Thrift.Transport;

namespace onepc
{
    class Gamer
    {
        public static Gamer Instance
        {
            get
            { 
                if(instance_ == null) 
                {
                    instance_ = new Gamer();
                }
                return instance_;
            }
        }

        private AppService.Client client_;

        private static Gamer instance_;
        private Gamer()
        {
            TTransport transport = new TSocket("127.0.0.1", 6688);
            TProtocol protocol = new TBinaryProtocol(transport);
            client_ = new AppService.Client(protocol); 
        }
        public void Close()
        {

        }
        public bool CheckCellNum(string cellNumString)
        {// 验证手机号
            if (cellNumString.Length != 11)
            {
                return false;
            }

            if (!cellNumString.StartsWith("1")) 
            {
                return false;
            }

            foreach (var i in cellNumString)
            {
                if (!(i >= '0' && i <= '9'))
                {
                    return false;
                }
            }
            return true;
        }

        public bool CheckEmail(string emailString)
        {// 验证邮箱
            Regex reg = new Regex(@"^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$");
            return reg.IsMatch(emailString);
        }

        public bool CheckPassword(string firstPassword, string secondPassword)
        {// 验证密码
            return firstPassword == secondPassword && 
                firstPassword.Length <= 16 && 
                firstPassword.Length >= 8;
        }      

        private string GetLocalDevice()
        {
            return "iphone6s";
        }

        private Location GetLocation()
        {
            Location loc = new Location();
            loc.Longitude = 0;
            loc.Latitude = 0;
            return loc;
        }

        private string PasswordEncoding(string inputPassword)
        {// MD5加密
            MD5 md5 = MD5.Create();
            StringBuilder sb = new StringBuilder();
            byte[] bytes = md5.ComputeHash(Encoding.UTF8.GetBytes(inputPassword));
            for (int i = 0; i < bytes.Length; i++)
            {
                sb.Append(bytes[i].ToString("X"));
            }
            return sb.ToString();
        }

        private string GetLocalPhoneNumber()
        {// ing
            return "";
        }

        public string SetUp(string email, string password)
        {
            return client_.SetUp(email, this.GetLocalPhoneNumber(), password, this.GetLocalDevice(), this.GetLocation());
        }

        public void BeginBindPhone(string uHandle, string phoneNumber)
        {
            client_.BeginBindPhoneNumber(uHandle, phoneNumber, this.GetLocalDevice(), this.GetLocation());
        }

        public int GetServerVersion()
        {
            try
            {
                return client_.GetServerVersion();
            }
            catch (Exception ex)
            {
                return -1;
            }
        }
    }
}
