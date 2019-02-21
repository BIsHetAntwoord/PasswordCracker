#ifndef LUACONSTRFUNCS_HPP_INCLUDED
#define LUACONSTRFUNCS_HPP_INCLUDED

#include "luabase.hpp"

int luafuncs_constraints_setminlength(lua_State*);
int luafuncs_constraints_setmaxlength(lua_State*);
int luafuncs_constraints_requireoneof(lua_State*);

#endif // LUACONSTRFUNCS_HPP_INCLUDED
