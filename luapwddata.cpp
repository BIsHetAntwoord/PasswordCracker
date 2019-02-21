#include "luapwddata.hpp"
#include "luafuncs.hpp"
#include "pwdgenerator.hpp"
#include "syncedstream.hpp"

#include <boost/thread/tss.hpp>

LuaException::LuaException(const char* msg) : std::runtime_error(msg) {}
LuaException::LuaException(const std::string& msg) : std::runtime_error(msg) {}
LuaException::~LuaException() {}

void lua_pwd_thread_state_cleanup(thread_local_lua_pwd_info* info)
{
    luaL_unref(info->state, LUA_REGISTRYINDEX, info->callback_ref);
    lua_close(info->state);
}

LuaPasswordData::LuaPasswordData(const std::string& init_file) : init_file(init_file) {}
LuaPasswordData::~LuaPasswordData() {}

thread_local_lua_pwd_info* LuaPasswordData::get_thread_state()
{
    static boost::thread_specific_ptr<thread_local_lua_pwd_info> lua_state(lua_pwd_thread_state_cleanup);
    if(!lua_state.get())
    {
        lua_state.reset(new thread_local_lua_pwd_info);
        thread_local_lua_pwd_info* info = lua_state.get();

        info->callback_ref = LUA_REFNIL;
        info->state = luaL_newstate();
        luaL_openlibs(info->state);

        luafuncs_load(info->state);

        this->init_thread_state(info);

    }
    return lua_state.get();
}

void LuaPasswordData::init_thread_state(thread_local_lua_pwd_info* info)
{
    lua_State* L = info->state;
    if(luaL_loadfile(L, this->init_file.c_str()))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1); //Remove error message
        throw LuaException(error_msg);
    }

    if(lua_pcall_trace(L, 0, 1))
    {
        std::string error_msg = lua_tostring(L, -1);
        lua_pop(L, 1); //Remove error message
        throw LuaException(error_msg);
    }

    if(!lua_isfunction(L, -1))
    {
        lua_pop(L, 1); //Remove the value
        throw LuaException("Returned password callback was not a function");
    }
    int callback_ref = luaL_ref(L, LUA_REGISTRYINDEX); //Store the returned function in the registry
    info->callback_ref = callback_ref;
}

int LuaPasswordData::run_callback(const std::string& str)
{
    thread_local_lua_pwd_info* lua_info = this->get_thread_state();

    lua_rawgeti(lua_info->state, LUA_REGISTRYINDEX, lua_info->callback_ref); //Fetch the callback
    lua_pushstring(lua_info->state, str.c_str()); //Push the argument
    if(lua_pcall_trace(lua_info->state, 1, 1))
    {
        std::string error_msg = lua_tostring(lua_info->state, -1);
        lua_pop(lua_info->state, 1); //Remove error message
        throw LuaException(error_msg);
    }

    if(!lua_isinteger(lua_info->state, -1))
    {
        lua_pop(lua_info->state, 1); //Remove the return value
        throw LuaException("Returned status code from callback was not an integer");
    }
    int result = lua_tointeger(lua_info->state, -1);
    if(result < 0 || result > 2)
    {
        lua_pop(lua_info->state, 1); //Remove the return value
        throw LuaException("Returned status code was not a valid status code");
    }

    lua_pop(lua_info->state, 1);

    return result;
}
