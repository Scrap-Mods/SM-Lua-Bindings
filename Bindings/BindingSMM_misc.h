#pragma once

int LuaSMM_Misc_test(lua_State* L);

const luaL_reg smm_misc[] =
{
    {"test",   LuaSMM_Misc_test},
    {NULL, NULL}
};
