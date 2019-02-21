#ifndef LUADICTDATA_HPP_INCLUDED
#define LUADICTDATA_HPP_INCLUDED

#include "luabase.hpp"
#include "dictionary.hpp"

class LuaDictionaryData
{
    private:
        lua_State* state;
        int dictionary_callback;

        void make_dict_lua_ref(Dictionary&);
    public:
        LuaDictionaryData(const std::string&);
        ~LuaDictionaryData();

        Dictionary get_dictionary();
};

#endif // LUADICTDATA_HPP_INCLUDED
