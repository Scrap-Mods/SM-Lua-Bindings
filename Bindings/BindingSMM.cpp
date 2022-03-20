#include "pch.h"
#include "BindingSMM.h"

int LuaSMM_version(lua_State* L)
{
    lua_settop(L, 0);
    lua_pushstring(L, "v1.0.0");
    return 1;
}
