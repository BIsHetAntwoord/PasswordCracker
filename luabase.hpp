#ifndef LUABASE_HPP_INCLUDED
#define LUABASE_HPP_INCLUDED

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdexcept>
#include <string>

class LuaException : public std::runtime_error
{
    public:
        LuaException(const std::string&);
        LuaException(const char*);
        virtual ~LuaException();
};

void luabase_load(lua_State*);

int lua_pcall_trace(lua_State*, int, int);
void lua_stackdump(lua_State*);
void lua_addpath(lua_State*, const std::string&);
void lua_addcpath(lua_State*, const std::string&);

int lua_synced_print(lua_State*);
int lua_stacktrace(lua_State*);

#endif // LUABASE_HPP_INCLUDED
