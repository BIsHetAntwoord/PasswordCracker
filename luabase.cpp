#include "luabase.hpp"
#include "syncedstream.hpp"
#include "pathutils.hpp"

#include <sstream>

void luabase_load(lua_State* L)
{
    lua_addpath(L, resolve_path("file:?.lua"));
    lua_addpath(L, resolve_path("std:?.lua"));

    lua_addcpath(L, resolve_path("file:?.dll"));
    lua_addcpath(L, resolve_path("std:native/?.dll"));

    lua_pushcfunction(L, lua_synced_print);
    lua_setglobal(L, "print");
}

int lua_pcall_trace(lua_State* L, int argno, int retno)
{
    int handler_pos = lua_gettop(L) - argno;

    lua_pushcfunction(L, lua_stacktrace);
    lua_insert(L, handler_pos);

    int ret = lua_pcall(L, argno, retno, handler_pos);
    lua_remove(L, handler_pos);

    return ret;
}

void lua_stackdump(lua_State* L)
{
    int top = lua_gettop(L);

    sync_cout << "Stack has " << top << " elements" << std::endl;
    for(int i = 1; i <= top; ++i)
    {
        int type = lua_type(L, i);
        switch(type)
        {
            case LUA_TSTRING:
                sync_cout << "string: " << lua_tostring(L, i) << std::endl;
                break;
            case LUA_TBOOLEAN:
                sync_cout << "boolean: " << (lua_toboolean(L, i) ? "true" : "false") << std::endl;
                break;
            case LUA_TNUMBER:
                sync_cout << "number: " << lua_tonumber(L, i) << std::endl;
                break;
            default:
                sync_cout << lua_typename(L, type) << std::endl;
                break;
        }
    }
}

void lua_addpath(lua_State* L, const std::string& path)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");

    std::string current_path = lua_tostring(L, -1);
    current_path.append(";");
    current_path.append(path);

    lua_pop(L, 1);
    lua_pushstring(L, current_path.c_str());
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);
}

void lua_addcpath(lua_State* L, const std::string& path)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "cpath");

    std::string current_path = lua_tostring(L, -1);
    current_path.append(";");
    current_path.append(path);

    lua_pop(L, 1);
    lua_pushstring(L, current_path.c_str());
    lua_setfield(L, -2, "cpath");
    lua_pop(L, 1);
}

int lua_synced_print(lua_State* L)
{
    int argno = lua_gettop(L);
    lua_getglobal(L, "tostring");

    std::stringstream result;
    for(int i = 1; i <= argno; ++i)
    {
        lua_pushvalue(L, -1);
        lua_pushvalue(L, i);

        lua_call(L, 1, 1);

        //Convert the string
        size_t length;
        const char* str = lua_tolstring(L, -1, &length);

        if(str == NULL)
            return luaL_error(L, "\'tostring\' must return a string to \'print\'");

        //Write to buffer
        if(i > 1)
            result << "\t";
        result.write(str, length);

        lua_pop(L, 1); //Remove the string
    }

    sync_cout << result.str() << std::endl;

    return 0;
}

int lua_stacktrace(lua_State* L)
{
    if(!lua_isstring(L, 1)) //Check if a message was sent
        return 1;
    lua_getglobal(L, "debug"); //Get the debug table
    if(!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return 1;
    }

    lua_getfield(L, -1, "traceback"); //Get the debug.traceback function
    if(!lua_isfunction(L, -1))
    {
        lua_pop(L, 2);
        return 1;
    }

    lua_pushvalue(L, 1); //Pass the error message
    lua_pushinteger(L, 2); //Skip this function
    lua_call(L, 2, 1); //Call debug.traceback
    return 1;
}
