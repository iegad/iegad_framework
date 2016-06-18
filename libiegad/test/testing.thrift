namespace cpp test


service EchoServer {
    string echo(1: string msg);
}

service TimeServer {
    string get();
}
