#include "pathutils.hpp"
#include "syncedstream.hpp"

#include <windows.h>

#include <boost/filesystem/path.hpp>

std::string application_script_dir()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    boost::filesystem::path path(buffer);
    return path.parent_path().append("scripts").string();
}

std::string resolve_path(const std::string& path_link)
{
    std::string::size_type index = path_link.find(":");
    if(index == std::string::npos)
        return path_link;

    std::string protocol = path_link.substr(0, index);
    std::string file = path_link.substr(index + 1);

    if(protocol == "std")
    {
        std::string standard_dir = application_script_dir();
        if(file.length() > 0)
        {
            if(file[0] != '\\')
                file = "\\" + file;
        }
        file = standard_dir + file;
    }

    return file;
}
