iegad_framework 依赖库 ：
	1. Boost::asio 			用户网络服务组件
	2. google glog 			用于日志系统
	3. google protocl buffer 	用户跨平台数据传输
	4. libmysql			用于mysql数据库访问


服务端有一个致命的BUG, 那就是无法防止, 注入式攻击
所以在阻塞在客户消息接收时, 应当设定超时值.