namespace cpp iegad.rpc


struct request {
1:  optional i32 msg_type = -1;
2:  optional i32 msg_flag = 0;
3:  optional i64 msg_time = 0;
4:  optional string msg_data;
}


typedef request response;


service iegadService {
    response func(1: request req);
    oneway void action(1: request req);
}
