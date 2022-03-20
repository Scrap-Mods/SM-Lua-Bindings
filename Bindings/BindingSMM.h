#pragma once

int LuaSMM_version(lua_State* L);

const luaL_reg smm[] =
{
    {"version",   LuaSMM_version},
    {NULL, NULL}
};