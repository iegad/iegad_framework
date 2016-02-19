namespace cpp rdm
namespace csharp rdm


struct RedisData_t {
1:  required string type;
2:  required string key;
3:  optional string str_value;
4:  optional list<string> lzs_value;			//list zset set
5:  optional map<string, string> hash_value;
}


service rdmServer {
    list<RedisData_t> GetKeys(1: string qurey_str);
    RedisData_t GetValue(1: string _key__, 2: string _type__);
    bool Connect(1: string ipstr, 2: i32 port, 3: string usrid, 4: string password);
    bool Select(1: i16 db_index);
}
