#ifndef __IEGAD_MONGO__
#define __IEGAD_MONGO__



#include <assert.h>
#include <string>
#include <memory>
#include <json/json.h>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>



namespace iegad {



class MongoDAI {
public:
    typedef std::shared_ptr<mongocxx::client> conn_t;
    typedef mongocxx::collection dai_t;


    static std::string
    makeConnectionString(const std::string &host,
                         const std::string &db,
                         const std::string &collection,
                         const std::string &user,
                         const std::string &pass,
                         int port = 27017)
    {
        assert(!host.empty() && !db.empty() && !collection.empty());

        static mongocxx::instance instance;

        Json::StreamWriterBuilder wr;
        Json::Value root;

        root["HOST"] = Json::Value(host);
        root["DB"] = Json::Value(db);
        root["COL"] = Json::Value(collection);
        root["USER"] = Json::Value(user);
        root["PASS"] = Json::Value(pass);
        root["PORT"] = Json::Value(port);

        return Json::writeString(wr, root);
    }


    static conn_t
    makeConnection(const std::string &initstr, std::string *errstr)
    {
        Json::CharReaderBuilder rd;
        Json::CharReader *r = rd.newCharReader();
        Json::Value root;

        assert(r->parse(initstr.data(), initstr.data() + initstr.size(), &root, errstr));

        std::string host = root["HOST"].asString();
        std::string user = root["USER"].asString();
        std::string pass = root["PASS"].asString();
        std::string uri;
        int port = root["PORT"].asInt();

        if (uri.empty()) {
            uri = "mongodb://";

            if (!user.empty() && !pass.empty()) {
                uri += user + ":" + pass + "@";
            }

            uri += host + ":" + std::to_string(port);
        }

        try {
            conn_t conn = conn_t(new mongocxx::client(mongocxx::uri(uri)));
            return conn;
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return NULL;
    }


    explicit
    MongoDAI(conn_t conn, const std::string &initstr) :
        conn_(conn)
    {
        _init(initstr);
    }


    bool
    check(std::string *)
    {
        return true;
    }


    bool
    get(dai_t &col, std::string *)
    {
        col = col_;
        return true;
    }


    bool
    reset(std::string *errstr)
    {
        assert(errstr);

        if (uri_.empty()) {
            uri_ = "mongodb://";

            if (!user_.empty() && !pass_.empty()) {
                uri_ += user_ + ":" + pass_ + "@";
            }

            uri_ += host_ + ":" + std::to_string(port_);
        }

        try {
            conn_ = conn_t(new mongocxx::client(mongocxx::uri(uri_)));
            col_ = (*conn_)[db_][collection_];
            return true;
        }
        catch (std::exception &ex) {
            *errstr = ex.what();
        }

        return false;
    }


private:
    void
    _init(const std::string &initstr)
    {
        Json::CharReaderBuilder rd;
        Json::CharReader *r = rd.newCharReader();
        Json::Value root;
        std::string errstr;

        assert(r->parse(initstr.data(), initstr.data() + initstr.size(), &root, &errstr));

        host_ = root["HOST"].asString();
        db_ = root["DB"].asString();
        collection_ = root["COL"].asString();
        user_ = root["USER"].asString();
        pass_ = root["PASS"].asString();
        port_ = root["PORT"].asInt();

        assert(this->reset(&errstr));
    }


    int port_;
    conn_t conn_;
    dai_t col_;
    std::string host_;
    std::string db_;
    std::string collection_;
    std::string user_;
    std::string pass_;
    std::string uri_;
}; // class MongoDAI;



} // namespace iegad;


#endif // __IEGAD_MONGO__
