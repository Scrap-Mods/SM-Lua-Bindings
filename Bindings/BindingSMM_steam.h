#pragma once

int LuaSMM_Steam_getName(lua_State* L);

int LuaSMM_Steam_getFriendPassphrase(lua_State* L);

int LuaSMM_Steam_clearRichPresence(lua_State* L);


const luaL_reg smm_steam[] =
{
    {"getName",             LuaSMM_Steam_getName},

    {"getFriendPassphrase", LuaSMM_Steam_getFriendPassphrase},

    {"clearRichPresence",   LuaSMM_Steam_getName},
    {NULL, NULL}
};
