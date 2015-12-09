namespace cpp iegad.onepc.data
namespace csharp iegad.onepc.data



struct DataDictionary {
# 数据字典
1: byte 	Flag 			= 0;
2: string 	Key;
3: string   Value;
4: string   Type;
5: string   Descript;
}


struct UsrSetIn {
# 用户登录信息
1: byte		Status 			= 0;
2: i32	 	UsrID			= 0;
3: string 	Password;
4: string 	Handler;
}



struct UsrIDEmailMap {
1: i32		UsrID			= 0;
2: string 	Email;
}



struct UsrIDPhoneNumMap {
1: i32		UsrID			= 0;
2: i64		PhoneNumber		= 0;
}



struct UsrIDDeviceMap {
1: i32		UsrID			= 0;
2: string 	Device;
}



struct UsrPersonal {
# 用户个人信息
1:	byte	Sex 			= 0;
2:	byte	Nation 			= 0;
3:	byte	Education 		= 0;
4:	byte	Constellation 	= 0;
5:  byte	Blood 			= 0;
6:  byte	ShengXiao 		= 0;
7:	byte	Flag 			= 0;
8:	i16		Job 			= 0;
9:	i32		Hometown 		= 0;
10:	i32		LocAddress 		= 0;
11: i32 	UsrID			= 0;
12:	i64 	BirthDate		= 0;
13:	i64  	RegistDate		= 0;
14:	string 	Nick;
15:	string 	Name;
16:	string 	College;
}



struct GamerInfo {
# 游戏角色信息
1:	byte	Vocation 		= 0;
2:	byte	Camp 			= 0;
3:	byte	Model 			= 0;
4:	byte	Head 			= 0;
5:	byte	HeadColor 		= 0;
6:	byte	Skin 			= 0;
7:	byte	Honor 			= 0;
8: 	byte	Flag 			= 0;
9:	i32		Exp 			= 0;
10: i32	 	UsrID			= 0;
11: string  GamerID;
12:	string  Name;
}


