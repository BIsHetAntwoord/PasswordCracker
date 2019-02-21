#include "luadictfuncs.hpp"
#include "dictionary.hpp"

#include <iostream>

int luafuncs_dict_addname(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.Dictionary.addName was not a light userdata");
    Dictionary* userdata = (Dictionary*)lua_touserdata(L, 1);
    std::string name = lua_tostring(L, 2);

    userdata->add_name(name);

    return 0;
}

int luafuncs_dict_adddate(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.Dictionary.addDate was not a light userdata");
    Dictionary* userdata = (Dictionary*)lua_touserdata(L, 1);
    size_t day = luaL_checkinteger(L, 2);
    size_t month = luaL_checkinteger(L, 3);
    size_t year = luaL_checkinteger(L, 4);
    if(day < 1 || day > 31)
        return luaL_error(L, "Day argument to function Program.Dictionary.addDate was not a valid day");
    if(month < 1 || month > 12)
        return luaL_error(L, "Month argument to function Program.Dictionary.addDate was not a valid month");

    userdata->add_date(day, month, year);

    return 0;
}

int luafuncs_dict_addword(lua_State* L)
{
    if(!lua_islightuserdata(L, 1))
        return luaL_error(L, "Argument 1 to function Program.Dictionary.addWord was not a light userdata");
    Dictionary* userdata = (Dictionary*)lua_touserdata(L, 1);

    int num_args = lua_gettop(L);
    if(num_args < 2)
        return luaL_error(L, "No strings given to Program.Dictionary.addWord");

    std::vector<std::string> words;
    for(int i = 2; i <= num_args; ++i)
        words.push_back(lua_tostring(L, i));

    userdata->add_word(words);

    return 0;
}
