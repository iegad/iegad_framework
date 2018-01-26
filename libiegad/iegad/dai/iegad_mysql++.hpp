#ifndef __IEGAD_MYSQLPP__
#define __IEGAD_MYSQLPP__


#include <mysql++/mysql++.h>
#include <json/json.h>



namespace iegad {



class MySQLDAI {
public:
    static std::string
    makeConnectionString(const std::string &db,
                         const std::string &host,
                         const std::string &user,
                         const std::string &pass,
                         int port = 3306)
    {
        Json::FastWriter wr;
        Json::Value root;

        root["DB"] = Json::Value(db);
        root["HOST"] = Json::Value(host);
        root["USER"] = Json::Value(user);
        root["PASS"] = Json::Value(pass);
        root["PORT"] = Json::Value(port);

        return wr.write(root);
    }


    MySQLDAI(const std::string &connstr)
    {
        _init(connstr);
    }


    bool
    check(std::string *errstr)
    {
        assert(errstr);

        try {
            return conn_.connected();
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


    bool
    get(std::shared_ptr<mysqlpp::Query> &q, std::string *errstr)
    {
        assert(errstr);

        try {
            q = std::shared_ptr<mysqlpp::Query>(new mysqlpp::Query(&conn_));
            return true;
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


    bool
    reset(std::string *errstr)
    {
        try {
            if (!conn_.connected()) {
                conn_.disconnect();
                conn_.connect(db_.c_str(), host_.c_str(), user_.c_str(), pass_.c_str(), port_);
            }

            mysqlpp::Query q(&conn_);
            return q.exec("set names utf8");
        }
        catch(std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


private:
    void
    _init(const std::string &connstr)
    {
        Json::Reader rd;
        Json::Value root;
        std::string errstr;

        assert(rd.parse(connstr, root));

        db_ = root["DB"].asString();
        host_ = root["HOST"].asString();
        user_ = root["USER"].asString();
        pass_ = root["PASS"].asString();
        port_ = root["PORT"].asInt();

        assert(this->reset(&errstr));
    }


    int port_;
    std::string db_;
    std::string host_;
    std::string user_;
    std::string pass_;
    mysqlpp::Connection conn_;
}; // class MySQLDAI;



} // namespace iegad;




#endif // __IEGAD_MYSQLPP__
