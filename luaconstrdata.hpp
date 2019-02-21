#ifndef LUACONSTRDATA_HPP_INCLUDED
#define LUACONSTRDATA_HPP_INCLUDED

#include <string>

#include "luabase.hpp"
#include "constraints.hpp"

class LuaConstraintsData
{
    private:
        lua_State* state;
        int constraints_callback;

        void make_constraints_lua_ref(PasswordConstraints&);
    public:
        LuaConstraintsData(const std::string&);
        ~LuaConstraintsData();

        PasswordConstraints get_constraints();
};

#endif // LUACONSTRDATA_HPP_INCLUDED
