#include "luaconstrdata.hpp"
#include "luafuncs.hpp"
#include "syncedstream.hpp"

LuaConstraintsData::LuaConstraintsData(const std::string& script)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luafuncs_load(L);

    if(luaL_loadfile(L, script.c_str()))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_close(L);
        throw LuaException(error_msg);
    }
    if(lua_pcall_trace(L, 0, 1))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_close(L);
        throw LuaException(error_msg);
    }
    if(!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
        lua_close(L);
        throw LuaException("Returned constraints callback was not a function");
    }
    int callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    this->state = L;
    this->constraints_callback = callback_ref;
}

LuaConstraintsData::~LuaConstraintsData()
{
    luaL_unref(this->state, LUA_REGISTRYINDEX, this->constraints_callback);
    lua_close(this->state);
}

void LuaConstraintsData::make_constraints_lua_ref(PasswordConstraints& constraints)
{
    lua_State* L = this->state;

    lua_getglobal(L, "PasswordConstraints");
    lua_getfield(L, -1, "new");
    lua_insert(L, -2); //Swap the two elements

    lua_pushlightuserdata(L, &constraints);

    if(lua_pcall_trace(L, 2, 1))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw LuaException(error_msg);
    }
}

PasswordConstraints LuaConstraintsData::get_constraints()
{
    PasswordConstraints constraints;

    lua_rawgeti(this->state, LUA_REGISTRYINDEX, this->constraints_callback);
    this->make_constraints_lua_ref(constraints);

    if(lua_pcall_trace(this->state, 1, 0))
    {
        std::string error_msg = lua_tostring(this->state, -1);
        lua_pop(this->state, 1);
        throw LuaException(error_msg);
    }

    return constraints;
}
