#include "luafuncs.hpp"
#include "pwdgenerator.hpp"
#include "luadictfuncs.hpp"
#include "luaconstrfuncs.hpp"
#include "pathutils.hpp"

void luafuncs_loadconstraints(lua_State* L)
{
    lua_pushstring(L, "PasswordConstraints");
    lua_newtable(L);

    lua_pushstring(L, "setMinLength");
    lua_pushcfunction(L, luafuncs_constraints_setminlength);
    lua_settable(L, -3);

    lua_pushstring(L, "setMaxLength");
    lua_pushcfunction(L, luafuncs_constraints_setmaxlength);
    lua_settable(L, -3);

    lua_pushstring(L, "requireOneOf");
    lua_pushcfunction(L, luafuncs_constraints_requireoneof);
    lua_settable(L, -3);

    lua_settable(L, -3);
}

void luafuncs_loaddictionary(lua_State* L)
{
    lua_pushstring(L, "Dictionary");
    lua_newtable(L);

    lua_pushstring(L, "addName");
    lua_pushcfunction(L, luafuncs_dict_addname);
    lua_settable(L, -3);

    lua_pushstring(L, "addDate");
    lua_pushcfunction(L, luafuncs_dict_adddate);
    lua_settable(L, -3);

    lua_pushstring(L, "addWord");
    lua_pushcfunction(L, luafuncs_dict_addword);
    lua_settable(L, -3);

    lua_settable(L, -3);
}

void luafuncs_loadpassword(lua_State* L)
{
    lua_pushstring(L, "Password");
    lua_newtable(L);

    lua_pushstring(L, "FOUND");
    lua_pushinteger(L, PWD_CALLBACK_FOUND);
    lua_settable(L, -3);

    lua_pushstring(L, "NOT_FOUND");
    lua_pushinteger(L, PWD_CALLBACK_NOT_FOUND);
    lua_settable(L, -3);

    lua_pushstring(L, "ERROR");
    lua_pushinteger(L, PWD_CALLBACK_ERROR);
    lua_settable(L, -3);

    lua_settable(L, -3);
}

void luafuncs_load(lua_State* L)
{
    //Load the base functions
    luabase_load(L);

    //Create the table
    lua_newtable(L);
    luafuncs_loadconstraints(L);
    luafuncs_loaddictionary(L);
    luafuncs_loadpassword(L);

    lua_pushstring(L, "resolvePath");
    lua_pushcfunction(L, luafuncs_resolvepath);
    lua_settable(L, -3);

    lua_setglobal(L, "Program");

    //Call the initialization script to define lua objects
    if(luaL_loadfile(L, resolve_path("std:std\\init.lua").c_str()))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw LuaException(error_msg);
    }
    if(lua_pcall_trace(L, 0, 0))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw LuaException(error_msg);
    }
}

int luafuncs_resolvepath(lua_State* L)
{
    std::string link = luaL_checkstring(L, 1);

    std::string path = resolve_path(link);

    lua_pushstring(L, path.c_str());
    return 1;
}
