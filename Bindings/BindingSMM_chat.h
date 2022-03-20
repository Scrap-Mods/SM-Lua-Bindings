#pragma once

int LuaSMM_Chat_getName(lua_State* L);
int LuaSMM_Chat_clear(lua_State* L);
int LuaSMM_Chat_setName(lua_State* L);
int LuaSMM_Chat_sendChatMessage(lua_State* L);

const luaL_reg smm_chat[] =
{
    {"sendChatMessage", LuaSMM_Chat_sendChatMessage},
    {"setName",         LuaSMM_Chat_setName},
    {"getName",         LuaSMM_Chat_getName},
    {"clear",           LuaSMM_Chat_clear},
    {"listen",          LuaSMM_version}, // TODO: Implement
    {NULL, NULL}
};