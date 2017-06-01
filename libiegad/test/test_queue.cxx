// ==========================================
// @title : queue 第一轮测试
// @by : iegad, 2016-06-18
// ==========================================


#include <gtest/gtest.h>
#include <thread>
#include <future>
#include <string>
#include <memory>

#include "tools/job_queue.hpp"
#include <boost/ref.hpp>


using namespace iegad::tools;



struct person_t {
public:
    person_t(const std::string & name, int age)
        :
          age(age),
          name(name)
    {}

    int age;
    std::string name;
};
typedef boost::shared_ptr<person_t> person_ptr;


int
func1(job_que_t<int> & que)
{// 异步出队线程
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    int res;
    if (!que.pop(&res)) {
        return -1;
    }
    return res;
}


person_ptr
func2(job_que_t<person_ptr> & que)
{
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    person_ptr res;
    que.pop(&res);
    return res;
}


// 1: ============== int que ================

TEST(queueTesting, pushAndPop_int)
{
    job_que_t<int> que;
    EXPECT_TRUE(que.empty());

    que.push(10);
    que.push(20);
    EXPECT_EQ(que.size(), 2u);
    // 同步出队
    int va1;
    EXPECT_TRUE(que.pop(&va1));
    EXPECT_EQ(10, va1);

    //异步出队
    std::future<int> f1 = std::async(func1, boost::ref(que));
    EXPECT_EQ(20, f1.get());
    // stop队列, 将返回错误出队值
    std::future<int> f2 = std::async(func1, boost::ref(que));
    que.stop();
    EXPECT_EQ(-1, f2.get());

    EXPECT_TRUE(que.empty());
}


// 2: ============== class que ================

TEST(queueTesting, pushAndPop_class)
{
    job_que_t<person_ptr> que;
    EXPECT_TRUE(que.empty());
    person_ptr p1(new person_t("肖琪", 28));
    person_ptr p2(new person_t("Aizen", 22));
    que.push(p1);
    que.push(p2);
    EXPECT_EQ(que.size(), 2u);
    // 同步出队
    person_ptr va1;
    EXPECT_TRUE(que.pop(&va1));
    EXPECT_EQ(28, va1->age);
    EXPECT_EQ("肖琪", va1->name);

    //异步出队
    std::future<person_ptr> f1 = std::async(func2, std::ref(que));
    person_ptr va2 = f1.get();
    EXPECT_EQ(22, va2->age);
    EXPECT_EQ("Aizen", va2->name);
    // stop队列, 将返回错误出队值
    std::future<person_ptr> f2 = std::async(func2, std::ref(que));
    que.stop();
    EXPECT_EQ(nullptr, f2.get());

    EXPECT_TRUE(que.empty());
}



