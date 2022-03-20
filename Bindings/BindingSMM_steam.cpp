#include "pch.h"
#include "BindingSMM_steam.h"

int LuaSMM_Steam_getName(lua_State* L)
{
    lua_settop(L, 0);
    lua_pushstring(L, SteamFriends()->GetPersonaName());
    return 1;
}

int LuaSMM_Steam_clearRichPresence(lua_State* L)
{
    SteamFriends()->ClearRichPresence();
    lua_settop(L, 0);
    return 0;
}

int LuaSMM_Steam_getFriendPassphrase(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint64_t steam_id = luaL_checkinteger(L, 1);
    lua_settop(L, 0);
    lua_pushstring(L, SteamFriends()->GetFriendRichPresence(CSteamID(steam_id), "passphrase"));
    return 1;
}
