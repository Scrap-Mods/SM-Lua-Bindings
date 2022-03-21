#pragma once

int LuaSMM_Misc_test(lua_State* L);
int LuaSMM_Misc_isDeveloperMode(lua_State* L);

const luaL_reg smm_misc[] =
{
    {"isDeveloperMode",   LuaSMM_Misc_isDeveloperMode},
    {NULL, NULL}
};
