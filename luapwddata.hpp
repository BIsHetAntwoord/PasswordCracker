#ifndef LUADATA_HPP_INCLUDED
#define LUADATA_HPP_INCLUDED

#include <string>
#include <stdexcept>

#include "luabase.hpp"

struct thread_local_lua_pwd_info
{
    lua_State* state;
    int callback_ref;
};

class LuaPasswordData
{
    private:
        std::string init_file;
        thread_local_lua_pwd_info* get_thread_state();
        void init_thread_state(thread_local_lua_pwd_info*);
    public:
        LuaPasswordData(const std::string&);
        ~LuaPasswordData();

        int run_callback(const std::string&);
};

#endif // LUADATA_HPP_INCLUDED
