#include "pch.h"
#include "BindingSMM_vm.h"

int LuaSMM_Vm_getLastEvent(lua_State* L)
{
    const auto event_name = SM::LuaManager::getInstancePtr()->m_pLuaVM->m_lastEvent;
    lua_pushlstring(L, event_name.c_str(), event_name.size());
    return 1;
}

int LuaSMM_Vm_getScriptId(lua_State* L)
{
    lua_pushinteger(L, SM::LuaManager::getInstancePtr()->m_scriptId);
    return 1;
}

int LuaSMM_Vm_getScriptRef(lua_State* L)
{
    lua_pushinteger(L, SM::LuaManager::getInstancePtr()->m_scriptRef);
    return 1;
}

int LuaSMM_Vm_getScriptInstanceId(lua_State* L)
{
    lua_pushinteger(L, SM::LuaManager::getInstancePtr()->m_scriptInstanceId);
    return 1;
}

int LuaSMM_Vm_getScriptTypeId(lua_State* L)
{
    lua_pushinteger(L, SM::LuaManager::getInstancePtr()->m_scriptTypeId);
    return 1;
}

int LuaSMM_Vm_setScriptId(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint32_t scriptId = luaL_checkint(L, 1);
    SM::LuaManager::getInstancePtr()->m_scriptId = scriptId;

    lua_settop(L, 0);
    return 0;
}

int LuaSMM_Vm_setScriptRef(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint32_t scriptRef = luaL_checkint(L, 1);
    SM::LuaManager::getInstancePtr()->m_scriptRef = scriptRef;

    lua_settop(L, 0);
    return 0;
}

int LuaSMM_Vm_setScriptInstanceId(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint32_t scriptInstanceId = luaL_checkint(L, 1);
    SM::LuaManager::getInstancePtr()->m_scriptInstanceId = scriptInstanceId;

    lua_settop(L, 0);
    return 0;
}

int LuaSMM_Vm_setScriptTypeId(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint8_t scriptTypeId = luaL_checkint(L, 1);
    SM::LuaManager::getInstancePtr()->m_scriptTypeId = scriptTypeId;

    lua_settop(L, 0);
    return 0;
}

int  LuaSMM_Vm_getIsServer(lua_State* L)
{
    lua_settop(L, 0);
    lua_pushinteger(L, SM::LuaManager::getInstancePtr()->m_bIsServer);
    return 1;
}

int  LuaSMM_Vm_setIsServer(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint8_t isServer = luaL_checkint(L, 1);
    SM::LuaManager::getInstancePtr()->m_bIsServer = isServer;

    lua_settop(L, 0);
    return 0;
}