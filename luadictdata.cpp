#include "luadictdata.hpp"
#include "luafuncs.hpp"
#include "syncedstream.hpp"

LuaDictionaryData::LuaDictionaryData(const std::string& script) : dictionary_callback(LUA_REFNIL)
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
        throw LuaException("Returned dictionary callback was not a function");
    }
    int callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    this->state = L;
    this->dictionary_callback = callback_ref;
}

LuaDictionaryData::~LuaDictionaryData()
{
    luaL_unref(this->state, LUA_REGISTRYINDEX, this->dictionary_callback);
    lua_close(this->state);
}

void LuaDictionaryData::make_dict_lua_ref(Dictionary& dict)
{
    lua_State* L = this->state;

    lua_getglobal(L, "Dictionary"); //Get the global dictionary table
    lua_getfield(L, -1, "new"); //Get the constructor
    lua_insert(L, -2); //Swap the two elements

    lua_pushlightuserdata(L, &dict); //Push the argument to the constructor

    if(lua_pcall_trace(L, 2, 1))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw LuaException(error_msg);
    }
}

Dictionary LuaDictionaryData::get_dictionary()
{
    Dictionary dict;

    lua_rawgeti(this->state, LUA_REGISTRYINDEX, this->dictionary_callback); //Fetch the callback
    this->make_dict_lua_ref(dict); //Push the argument

    if(lua_pcall_trace(this->state, 1, 0))
    {
        std::string error_msg = lua_tostring(this->state, -1);
        lua_pop(this->state, 1);
        throw LuaException(error_msg);
    }

    return dict;
}
