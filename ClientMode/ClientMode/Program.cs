using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace onepc
{
    class Program
    {
        static void Main(string[] args)
        {
            BizLocalLogic bizControl = new BizLocalLogic();
            Console.WriteLine(bizControl.CheckCellNum("12345678912"));
            Console.WriteLine(bizControl.CheckCellNum("002345678912"));
            Console.WriteLine(bizControl.CheckEmail("iegad@vi"));
            Console.WriteLine(bizControl.CheckEmail("iegad@vip.qq.com"));
            Console.WriteLine(bizControl.CheckPassword("abcd1111", "abcD1111"));
            Console.WriteLine(bizControl.CheckPassword("86343961xq", "86343961xq"));
            Console.WriteLine(bizControl.PasswordEncoding("86343961xq"));
            Console.ReadKey();
        }
    }
}
