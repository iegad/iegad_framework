#include <gtest/gtest.h>
#include <iegad/dai/iegad_mysql++.hpp>
#include <iegad/dai/iegad_mongo.hpp>
#include <iegad/tigen/iegad_tigen_base_handler.hpp>



class MyHandler : public virtual iegad::tigen::TigenBaseHandler<iegad::MySqlDAI> {
public:
    explicit MyHandler(iegad::MySqlDAI::conn_t conn, const std::string &initstr) :
        iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>(conn, initstr)
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
    std::string initstr = iegad::MySqlDAI::makeConnectionString("127.0.0.1", "IEGAD_TEST", "iegad", "1111");

    iegad::MySqlDAI::conn_t conn = iegad::MySqlDAI::makeConnection(initstr, &errstr);

    EXPECT_TRUE(conn != NULL);

    MyHandler handler(conn, initstr);

    EXPECT_TRUE(handler.check(&errstr));
    EXPECT_TRUE(handler.reset(&errstr));
    EXPECT_TRUE(handler.get(q, &errstr));
    EXPECT_TRUE(handler.setData(q));
}



class MonHandler : public virtual iegad::tigen::TigenBaseHandler<iegad::MongoDAI> {
public:
    explicit MonHandler(iegad::MongoDAI::conn_t conn, const std::string &initstr) :
        iegad::tigen::TigenBaseHandler<iegad::MongoDAI>(conn, initstr)
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

    std::string errstr;
    std::string initstr = iegad::MongoDAI::makeConnectionString("127.0.0.1", "test", "testcol", "", "");

    iegad::MongoDAI::dai_t q;
    iegad::MongoDAI::conn_t conn = iegad::MongoDAI::makeConnection(initstr, &errstr);

    EXPECT_TRUE(conn != NULL);

    MonHandler handler(conn, initstr);

    EXPECT_TRUE(handler.check(&errstr));
    EXPECT_TRUE(handler.reset(&errstr));
    EXPECT_TRUE(handler.get(q, &errstr));
    EXPECT_TRUE(handler.setData(q));
}


class MMHandler : public virtual iegad::tigen::TigenBaseHandler<iegad::MongoDAI>,
                  public virtual iegad::tigen::TigenBaseHandler<iegad::MySqlDAI> {
public:
    explicit MMHandler(iegad::MongoDAI::conn_t conn1, const std::string &initstr1,
                       iegad::MySqlDAI::conn_t conn2, const std::string &initstr2) :
        iegad::tigen::TigenBaseHandler<iegad::MongoDAI>(conn1, initstr1),
        iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>(conn2, initstr2)
    {}


    virtual ~MMHandler() {}


    bool
    setData1(iegad::MongoDAI::dai_t q)
    {
        using bsoncxx::builder::basic::make_document;
        using bsoncxx::builder::basic::kvp;
        q.insert_one(make_document(kvp("tval", "111")));
        return true;
    }


    bool
    setData2(iegad::MySqlDAI::dai_t q)
    {
        return q->exec("INSERT INTO T_TEST(T_VAL) VALUES(11)");
    }


    bool
    check1(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::check(errstr);
    }


    bool
    reset1(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::reset(errstr);
    }


    bool
    get1(iegad::MongoDAI::dai_t &q, std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MongoDAI>::get(q, errstr);
    }


    bool
    check2(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::check(errstr);
    }


    bool
    reset2(std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::reset(errstr);
    }


    bool
    get2(iegad::MySqlDAI::dai_t &q, std::string *errstr)
    {
        return iegad::tigen::TigenBaseHandler<iegad::MySqlDAI>::get(q, errstr);
    }
}; // class MyHandler;



TEST(daiTesting, mongoAndMySqlHandler)
{
    std::string errstr;
    std::string initstr1 = iegad::MongoDAI::makeConnectionString("127.0.0.1", "test", "testcol", "", "");
    std::string initstr2 = iegad::MySqlDAI::makeConnectionString("127.0.0.1", "IEGAD_TEST", "iegad", "1111");

    iegad::MongoDAI::conn_t conn1 = iegad::MongoDAI::makeConnection(initstr1, &errstr);
    iegad::MySqlDAI::conn_t conn2 = iegad::MySqlDAI::makeConnection(initstr2, &errstr);

    EXPECT_TRUE(conn1 != NULL);
    EXPECT_TRUE(conn2 != NULL);

    iegad::MongoDAI::dai_t q1;
    iegad::MySqlDAI::dai_t q2;

    MMHandler handler(conn1, initstr1, conn2, initstr2);

    EXPECT_TRUE(handler.check1(&errstr));
    EXPECT_TRUE(handler.reset1(&errstr));
    EXPECT_TRUE(handler.get1(q1, &errstr));
    EXPECT_TRUE(handler.setData1(q1));

    EXPECT_TRUE(handler.check2(&errstr));
    EXPECT_TRUE(handler.reset2(&errstr));
    EXPECT_TRUE(handler.get2(q2, &errstr));
    EXPECT_TRUE(handler.setData2(q2));
}
