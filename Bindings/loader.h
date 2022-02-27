#pragma once

extern lua_CFunction o_clientFunc;
extern luaL_Reg* clientLibs;
int LoadClientLibraries(SM::LuaVM* vm);

extern lua_CFunction o_serverFunc;
extern luaL_Reg* serverLibs;
int LoadServerLibraries(SM::LuaVM* vm);

extern lua_CFunction o_sharedFunc;
extern luaL_Reg* sharedLibs;
int LoadSharedLibraries(SM::LuaVM* vm);