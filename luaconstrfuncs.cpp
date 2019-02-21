#include "luaconstrfuncs.hpp"
#include "constraints.hpp"

int luafuncs_constraints_setminlength(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.PasswordConstraints.setMinLength was not a light userdata");
    PasswordConstraints* userdata = (PasswordConstraints*)lua_touserdata(L, 1);
    size_t length = luaL_checkinteger(L, 2);

    userdata->min_length(length);

    return 0;
}

int luafuncs_constraints_setmaxlength(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.PasswordConstraints.setMaxLength was not a light userdata");
    PasswordConstraints* userdata = (PasswordConstraints*)lua_touserdata(L, 1);
    size_t length = luaL_checkinteger(L, 2);

    userdata->max_length(length);

    return 0;
}

int luafuncs_constraints_requireoneof(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.PasswordConstraints.requireOneOf was not a light userdata");
    PasswordConstraints* userdata = (PasswordConstraints*)lua_touserdata(L, 1);
    std::string characters = luaL_checkstring(L, 2);

    userdata->require_one_of(characters);

    return 0;
}
