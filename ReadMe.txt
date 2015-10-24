iegad_framework <架构库>
依赖库 :
	1. Boost::asio 			用户网络服务组件
	2. google glog 			用于日志系统
	3. google protocl buffer 	用户跨平台数据传输

iegad_mysql <MySQL API 封装库>
依赖库 :
	1. libmysql			用于mysql数据库访问
	2. google glog 			用于日志系统
	3. iegad_framework		iegad架构库


test_server <测试服务端程序>
依赖库 :
	1. libmysql			用于mysql数据库访问
	2. iegad_mysql 			用于数据库操作
	3. iegad_framework		iegad架构库
	4. google glog 			用于日志系统
	5. google protocl buffer 	用户跨平台数据传输
	6. Boost::asio 			用户网络服务组件

test_client <测试客户端程序>
依赖库 :
	***




<2015-10-11>

	总结: 当今天为止, 一切还都顺利, 很多功能都进行了封装, 多线程的TCP服务端也进行了多次验证(这里使用验证的说法, 是因为, 还没有达到测试的级别), 每个模块都QC了多次, 虽然不是很详细的QC, 后期, 在作应用服务端程序前, 要进行仔细的QC.
	mysql库, 验证的不多, 对于mysql的运维, 我的学习还不够, 后期会更多的专注于mysql库的验证和测试.
	
	值得一说的是udp, 准备使用自己所想的那种方式来进行推送服务器的开发, 虽然udp的接收端和推送端都已开发完成, 但里面的 可靠通信的算法部分还有待进一步的改进(就算没有问题, 也是需要多次验证和测试).
	并且, C#的pb封装也完成了, 按照书上所说的方式, 进行了可移植性的改装, 但能否应用到IO
S中, 现在还不得而知.

	总之, 一切还算顺利, 下一步的工作, 主要还是完成一个 即时聊天系统, 用来验证和测试现
有的框架的稳定性.

	稳定性最为重要, 其次是代码的维护性, 最后才是性能.
	

<2015-10-23>

	将整个库的结构进行整改, 并且开始单元测试.

<2015-10-24>
	对github 的使用还是太差了!
