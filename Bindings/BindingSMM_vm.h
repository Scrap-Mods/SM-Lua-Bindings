#pragma once

int LuaSMM_Vm_getLastEvent(lua_State* L);

int LuaSMM_Vm_getScriptId(lua_State* L);
int LuaSMM_Vm_getScriptRef(lua_State* L);
int LuaSMM_Vm_getScriptInstanceId(lua_State* L);
int LuaSMM_Vm_getScriptTypeId(lua_State* L);

int LuaSMM_Vm_setScriptId(lua_State* L);
int LuaSMM_Vm_setScriptRef(lua_State* L);
int LuaSMM_Vm_setScriptInstanceId(lua_State* L);
int LuaSMM_Vm_setScriptTypeId(lua_State* L);

int LuaSMM_Vm_getIsServer(lua_State* L);
int LuaSMM_Vm_setIsServer(lua_State* L);

const luaL_reg smm_vm[] =
{
    {"getLastEvent",        LuaSMM_Vm_getLastEvent},

    {"getScriptId",         LuaSMM_Vm_getScriptId},
    {"getScriptRef",        LuaSMM_Vm_getScriptRef},
    {"getScriptInstanceId", LuaSMM_Vm_getScriptInstanceId},
    {"getScriptTypeId",     LuaSMM_Vm_getScriptTypeId},

    {"setScriptId",         LuaSMM_Vm_setScriptId},
    {"setScriptRef",        LuaSMM_Vm_setScriptRef},
    {"setScriptInstanceId", LuaSMM_Vm_setScriptInstanceId},
    {"setScriptTypeId",     LuaSMM_Vm_setScriptTypeId},

    {"getIsServer",     LuaSMM_Vm_getIsServer},
    {"setIsServer",     LuaSMM_Vm_setIsServer},
    {NULL, NULL}
};
