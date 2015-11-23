#ifndef __SAFE_MAP_TEST__
#define __SAFE_MAP_TEST__


#include "common/iegad_map.hpp"
#include <string>
#include "gtest/gtest.h"



TEST(SAFE_MAP_CASE, SAFE_MAP_TEST)
{
    typedef iegad::tools::safe_map<int, std::string> map_t;
    map_t mp;
    EXPECT_EQ(true, mp.empty());
    mp.insert(10, "ten");
    mp.set(11, "eleven");
    
    EXPECT_EQ(2, mp.size());
    EXPECT_EQ(true, mp.find(10) && mp.find(11));
    EXPECT_EQ(false, mp.find(50));
    EXPECT_EQ(false, mp.insert(10, "one"));
    mp.set(11, "shiyi");
    EXPECT_EQ("shiyi", mp[11]);
    mp.set(1, "one");
    mp.set(2, "two");

    EXPECT_EQ(4, mp.size());
    EXPECT_EQ(1, mp.erase(2));
    mp.clear();
    EXPECT_EQ(true, mp.empty());
}


    




#endif // __SAFE_MAP_TEST__