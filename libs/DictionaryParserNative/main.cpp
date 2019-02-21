#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <memory>
#include <iostream>

#include "parser.hpp"

const char* METATABLE_NAME = "PasswordCracker.Std.DictionaryParser";

struct lua_userdata
{
    Parser* parser;
};

struct lua_dictionary
{
    int dict_ref;
};

void lua_dict_name_callback(lua_State* L, void* data, const std::string& name)
{
    lua_dictionary* dict = (lua_dictionary*)data;
    int dict_ref = dict->dict_ref;

    lua_rawgeti(L, LUA_REGISTRYINDEX, dict_ref);

    lua_getfield(L, -1, "addName");
    lua_pushvalue(L, -2);
    lua_pushstring(L, name.c_str());
    lua_call(L, 2, 0);

    lua_pop(L, 1);
}

void lua_dict_date_callback(lua_State* L, void* data, size_t day, size_t month, size_t year)
{
    lua_dictionary* dict = (lua_dictionary*)data;
    int dict_ref = dict->dict_ref;

    lua_rawgeti(L, LUA_REGISTRYINDEX, dict_ref);

    lua_getfield(L, -1, "addDate");
    lua_pushvalue(L, -2);
    lua_pushinteger(L, day);
    lua_pushinteger(L, month);
    lua_pushinteger(L, year);
    lua_call(L, 4, 0);

    lua_pop(L, 1);
}

void lua_dict_word_callback(lua_State* L, void* data, const std::vector<std::string>& word)
{
    lua_dictionary* dict = (lua_dictionary*)data;
    int dict_ref = dict->dict_ref;

    lua_rawgeti(L, LUA_REGISTRYINDEX, dict_ref);

    lua_getfield(L, -1, "addWord");
    lua_pushvalue(L, -2);
    for(const std::string& str : word)
    {
        std::cout << "Found word: " << str << std::endl;
        lua_pushstring(L, str.c_str());
    }
    lua_call(L, word.size() + 1, 0);

    lua_pop(L, 1);
}

lua_userdata* lua_checkparser(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TUSERDATA);
    lua_userdata* result = (lua_userdata*)luaL_checkudata(L, index, METATABLE_NAME);
    if(result == NULL)
        luaL_error(L, "Could not convert argument %d to a DictionnaryParser", index);

    return result;
}

lua_dictionary* lua_checkdictionary(lua_State* L, int index)
{
    if(index < 0)
        index = lua_gettop(L) + index + 1;
    if(!lua_istable(L, index))
    {
        luaL_error(L, "Argument %d was not a table", index);
    }

    const char* method_names[] = {"addName", "addDate", "addWord"};
    for(size_t i = 0; i < 3; ++i)
    {
        lua_getfield(L, index, method_names[i]);
        if(!lua_isfunction(L, -1))
        {
            lua_pop(L, 1);
            luaL_error(L, "Argument %d did not have a method %s", index, method_names[i]);
        }
        lua_pop(L, 1);
    }

    lua_dictionary* result = new lua_dictionary;
    lua_pushvalue(L, index);
    result->dict_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return result;
}

lua_userdata* lua_newparser(lua_State* L)
{
    lua_userdata* result = (lua_userdata*)lua_newuserdata(L, sizeof(lua_userdata));
    luaL_getmetatable(L, METATABLE_NAME);
    lua_setmetatable(L, -2);
    return result;
}

int lua_dictionaryparser_new(lua_State* L)
{
    lua_dictionary* dictionary = lua_checkdictionary(L, 1);

    data_callbacks callbacks;
    callbacks.data = dictionary;
    callbacks.name_callback = lua_dict_name_callback;
    callbacks.date_callback = lua_dict_date_callback;
    callbacks.word_callback = lua_dict_word_callback;

    Parser* parser = new Parser(callbacks);

    try
    {
        lua_userdata* userdata = lua_newparser(L);
        userdata->parser = parser;
        return 1;
    }
    catch(...)
    {
        delete dictionary;
        delete parser;

        throw;
    }
}

int lua_dictionaryparser_parse(lua_State* L)
{
    lua_userdata* userdata = lua_checkparser(L, 1);
    const char* filename = luaL_checkstring(L, 2);

    FILE* stream = std::fopen(filename, "rb");
    if(!stream)
    {
        lua_pushboolean(L, false);
        lua_pushfstring(L, "Could not open file %s", filename);
        return 2;
    }

    std::string errors;
    bool success = userdata->parser->parse(L, stream, errors);

    if(!success)
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, errors.c_str());
        return 2;
    }
    else
    {
        lua_pushboolean(L, true);
        return 1;
    }
}

int lua_dictionaryparser_gc(lua_State* L)
{
    lua_userdata* userdata = lua_checkparser(L, 1);

    data_callbacks callbacks = userdata->parser->get_callbacks();
    lua_dictionary* dict = (lua_dictionary*)callbacks.data;
    luaL_unref(L, LUA_REGISTRYINDEX, dict->dict_ref);

    delete dict;
    delete userdata->parser;

    return 0;
}

const luaL_Reg DictionaryParser_funcs[] = {
    {"new", lua_dictionaryparser_new},
    {"parse", lua_dictionaryparser_parse},
    {NULL, NULL}
};

const luaL_Reg DictionaryParser_meta[] = {
    {"__gc", lua_dictionaryparser_gc},
    {NULL, NULL}
};

extern "C" int luaopen_std_dictionaryparser(lua_State* L)
{
    lua_newtable(L);
    luaL_setfuncs(L, DictionaryParser_funcs, 0);

    luaL_newmetatable(L, METATABLE_NAME);
    luaL_setfuncs(L, DictionaryParser_meta, 0);

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -3);
    lua_rawset(L, -3);

    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, -3);
    lua_rawset(L, -3);

    lua_pop(L, 1);

    return 1;
}
