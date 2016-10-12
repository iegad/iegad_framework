#ifndef __IEGAD_FILESYSTEM__
#define __IEGAD_FILESYSTEM__



#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <string>



namespace iegad {



class filesystem {
public:
    using bf = boost::filesystem;


    static const std::string
    find_file(const bf::path & dir, const std::string & filename)
    {
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




private:
    filesystem() {}
}; // class filesystem;



} // namespace iegad;



#endif // __IEGAD_FILESYSTEM__
