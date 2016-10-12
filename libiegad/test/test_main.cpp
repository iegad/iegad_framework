#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "sercurity/iegad_aes.hpp"


/*
int
main(int argc, char * argv[])
{    
    std::setlocale(LC_ALL, "zh_CN.UTF-8");                        
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/



#include <boost/filesystem.hpp>
#include <boost/optional.hpp>



const std::string
find_file(const boost::filesystem::path & dir, const std::string & filename) {
    boost::optional<boost::filesystem::path> res;

    if (!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir)) {
        return "path is invalied";
    }

    boost::filesystem::recursive_directory_iterator end;
    for (boost::filesystem::recursive_directory_iterator pos(dir); pos != end; pos++) {
        if (!boost::filesystem::is_directory(*pos)
                && pos->path().filename() == filename) {
            res = pos->path();
        }
    }
    return res ? res->string() : "File not found";
}




int
main()
{
    std::cout<<find_file("/usr/local/bin/", "ab")<<std::endl;
    exit(0);
}
