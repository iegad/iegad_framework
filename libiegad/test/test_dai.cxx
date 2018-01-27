#include <gtest/gtest.h>
#include <iegad/dai/iegad_mysql++.hpp>
#include <iegad/dai/iegad_mongo.hpp>
#include <iegad/tigen/iegad_tigen_base_handler.hpp>



class MyHandler : public virtual iegad::tigen::TigenBaseHandler<iegad::MySqlDAI> {
public:
    explicit MyHandler(const std::string &initstr) :
        iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>(initstr)
    {}


    virtual ~MyHandler() {}


    bool
    setData(iegad::MySqlDAI::dai_t q)
    {
        return q->exec("INSERT INTO T_TEST(T_VAL) VALUES(11)");
    }


    bool
    check(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::check(errstr);
    }


    bool
    reset(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::reset(errstr);
    }


    bool
    get(iegad::MySqlDAI::dai_t &q, std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::get(q, errstr);
    }
}; // class MyHandler;




TEST(daiTesting, mysqlHandler)
{
    iegad::MySqlDAI::dai_t q;
    std::string errstr;
    MyHandler handler(iegad::MySqlDAI::makeConnectionString("IEGAD_TEST", "127.0.0.1", "iegad", "1111"));

    EXPECT_TRUE(handler.check(&errstr));
    EXPECT_TRUE(handler.reset(&errstr));
    EXPECT_TRUE(handler.get(q, &errstr));
    EXPECT_TRUE(handler.setData(q));
}



class MonHandler : public virtual iegad::tigen::TigenBaseHandler<iegad::MongoDAI> {
public:
    explicit MonHandler(const std::string &initstr) :
        iegad::tigen::TigenBaseHandler<iegad::MongoDAI>(initstr)
    {}


    virtual ~MonHandler() {}


    bool
    setData(iegad::MongoDAI::dai_t q)
    {
        using bsoncxx::builder::basic::make_document;
        using bsoncxx::builder::basic::kvp;
        q.insert_one(make_document(kvp("tval", "111")));
        return true;
    }


    bool
    check(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::check(errstr);
    }


    bool
    reset(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::reset(errstr);
    }


    bool
    get(iegad::MongoDAI::dai_t &q, std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::get(q, errstr);
    }
}; // class MyHandler;



TEST(daiTesting, mongoHandler)
{
    iegad::MongoDAI::dai_t q;
    std::string errstr;
    MonHandler handler(iegad::MongoDAI::makeConnectionString("127.0.0.1", "test", "testcol", "", ""));

    EXPECT_TRUE(handler.check(&errstr));
    EXPECT_TRUE(handler.reset(&errstr));
    EXPECT_TRUE(handler.get(q, &errstr));
    EXPECT_TRUE(handler.setData(q));
}

