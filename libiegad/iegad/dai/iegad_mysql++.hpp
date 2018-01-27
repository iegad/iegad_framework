#ifndef __IEGAD_MYSQLPP__
#define __IEGAD_MYSQLPP__


#include <mysql++/mysql++.h>
#include <json/json.h>



namespace iegad {



class MySqlDAI {
public:
    typedef std::shared_ptr<mysqlpp::Connection> conn_t;
    typedef std::shared_ptr<mysqlpp::Query> dai_t;


    static std::string
    makeConnectionString(const std::string &host,
                         const std::string &db,
                         const std::string &user,
                         const std::string &pass,
                         int port = 3306)
    {
        Json::StreamWriterBuilder wr;
        Json::Value root;

        root["DB"] = Json::Value(db);
        root["HOST"] = Json::Value(host);
        root["USER"] = Json::Value(user);
        root["PASS"] = Json::Value(pass);
        root["PORT"] = Json::Value(port);

        return Json::writeString(wr, root);
    }


    static conn_t
    makeConnection(const std::string &initstr, std::string *errstr)
    {
        Json::CharReaderBuilder rd;
        Json::CharReader* r = rd.newCharReader();
        Json::Value root;

        assert(r->parse(initstr.data(), initstr.data() + initstr.size(), &root, errstr));

        std::string host = root["HOST"].asString();
        std::string db = root["DB"].asString();
        std::string user = root["USER"].asString();
        std::string pass = root["PASS"].asString();
        int port = root["PORT"].asInt();

        try {
            conn_t conn(new mysqlpp::Connection);
            conn->disconnect();
            conn->connect(db.c_str(), host.c_str(), user.c_str(), pass.c_str(), port);

            mysqlpp::Query q(conn.get());

            if (q.exec("set names utf8")) {
                return conn;
            }

            return NULL;
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return NULL;
    }


    static int
    executeTransaction(std::list<std::string> &sqls, mysqlpp::Connection &conn, std::string *errstr)
    {
        mysqlpp::Transaction trans(conn);
        mysqlpp::Query q(&conn);
        bool noerr = false;

        try {
            for (auto itr = sqls.begin(); itr != sqls.end(); ++itr) {
                noerr = q.exec(*itr);
                if (!noerr) {
                    break;
                }
            }
            if (noerr) {
                trans.commit();
            }
            else {
                trans.rollback();
            }
        }
        catch (std::exception &ex) {
            trans.rollback();
            *errstr = ex.what();
        }

        return noerr ? 0 : -1;
    }


    explicit
    MySqlDAI(conn_t conn, const std::string &initstr) :
        conn_(conn)
    {
        _init(initstr);
    }


    bool
    check(std::string *errstr)
    {
        assert(errstr);

        try {
            return conn_->connected();
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


    bool
    get(dai_t &q, std::string *errstr)
    {
        assert(errstr);

        try {
            q = dai_t(new mysqlpp::Query(conn_.get()));
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
            if (conn_->connected()) {
                conn_->disconnect();
            }

            conn_->connect(db_.c_str(), host_.c_str(), user_.c_str(), pass_.c_str(), port_);
            mysqlpp::Query q(conn_.get());

            return q.exec("set names utf8");
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


private:
    void
    _init(const std::string &connstr)
    {
        Json::CharReaderBuilder rd;
        Json::CharReader* r = rd.newCharReader();
        Json::Value root;
        std::string errstr;

        assert(r->parse(connstr.data(), connstr.data() + connstr.size(), &root, &errstr));

        db_ = root["DB"].asString();
        host_ = root["HOST"].asString();
        user_ = root["USER"].asString();
        pass_ = root["PASS"].asString();
        port_ = root["PORT"].asInt();

        assert(this->reset(&errstr));
    }


    conn_t conn_;
    int port_;
    std::string db_;
    std::string host_;
    std::string user_;
    std::string pass_;
}; // class MySQLDAI;



} // namespace iegad;




#endif // __IEGAD_MYSQLPP__
