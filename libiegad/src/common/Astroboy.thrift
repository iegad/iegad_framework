namespace cpp iegad.thrift_ex
namespace php iegad.thrift_ex
namespace csharp iegad.thrift_ex
namespace java iegad.thrift_ex




// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 : 提供一个公共的远程过程调用接口.
//   	   目前为 v0.1.0版
// 
//		未防止开发过程中, 业务需求的变更, 
//		作出统一的接口调用, 提高可扩展性.
//		同时提高开发效率.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================




struct req_t {
// 请求信息
1 : required i32 r_type;				// 消息类型
2 : optional i32 r_size = 0;			// 消息长度
3 : optional string r_ver = "v0.1.0";	// 版本号
4 : optional string r_str;				// 消息数据
5 : optional string r_md5;				// MD5校验
6 : optional list<i32> r_sha1;			// SHA1校验
}


struct res_t {
// 应答信息
1 : required i32 r_type;				// 消息类型
2 : optional i32 r_size = 0;			// 消息长度
3 : optional i32 r_errcode = 0;			// 错误code
4 : optional string r_ver = "v0.1.0";	// 版本号
5 : optional string r_str;				// 消息数据
6 : optional string r_errstr;			// 错误信息
}



// ============================
// @用途 : Service 服务接口类
// @PS : 名字叫作 "阿童木"
// ============================
service Astroboy {
	// ============================
	// @用途 : 发送 string 类型消息, 继承自 AMQ_ISender
	// @req_msg : 请求的消息
	// @返回值 : 应答消息
	// @名字叫作 茶水博士(阿童木的创造者)
	// ============================
	res_t Elefun(1: req_t req_msg);
}