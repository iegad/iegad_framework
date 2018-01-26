// ==========================================
// @title : worker 第一轮测试
// @by : iegad, 2016-06-18
// ==========================================


#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <thread>
#include "iegad/tools/job_worker.hpp"

using namespace iegad::tools;


struct employee_t {
public:
    std::string name;
    int age;
};
typedef std::shared_ptr<employee_t> employee_ptr;


int
callback1(int & val)
{
    EXPECT_EQ(10, val);
    return 0;
}


int
callback2(employee_ptr & val)
{
    EXPECT_EQ(10, val->age);
    EXPECT_EQ("10", val->name);
    return 0;
}


// 1: ================== run_int ====================

TEST(workerTesting, run_int)
{
    job_que_t<int> que;
    worker_t<int> wkr(que, callback1);
    wkr.run(1);
    for (int i = 0; i < 10; i++) {
        que.push(10);
    }
    wkr.stop();
    EXPECT_TRUE(que.empty());
}


// 1: ================== run_class ====================

TEST(workerTesting, run_class)
{
    job_que_t<employee_ptr> que;
    worker_t<employee_ptr> wkr(que, callback2);
    wkr.run(1);
    for (int i = 0; i < 10; i++) {
        employee_ptr e(new employee_t);
        e->name = "10";
        e->age = 10;
        que.push(e);
    }
    wkr.stop();
    EXPECT_TRUE(que.empty());
}
