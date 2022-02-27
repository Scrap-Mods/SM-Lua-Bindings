#include "pch.h"
#include "loader.h"
#include <cstdio>

int LoadClientLibraries(SM::LuaVM* vm)
{
	printf("[SMM] Loading CLIENT libraries\n");

	unsigned lib_index = 0;
	while (luaL_Reg* plib_entry = (luaL_Reg*)&clientLibs[lib_index++])
	{
		if (!plib_entry->name) break;
		printf("Loading lib: %s\n", plib_entry->name);

		luaL_register(vm->luaState, plib_entry->name, (luaL_reg*)plib_entry->func);
		lua_pop(vm->luaState, 1);
	}

	return o_clientFunc((lua_State*)vm);
}

int LoadServerLibraries(SM::LuaVM* vm)
{
	printf("[SMM] Loading SERVER libraries\n");

	unsigned lib_index = 0;
	while (luaL_Reg* plib_entry = (luaL_Reg*)&serverLibs[lib_index++])
	{
		if (!plib_entry->name) break;
		printf("Loading lib: %s\n", plib_entry->name);

		luaL_register(vm->luaState, plib_entry->name, (luaL_reg*)plib_entry->func);
		lua_pop(vm->luaState, 1);
	}

	return o_serverFunc((lua_State*)vm);
}

int LoadSharedLibraries(SM::LuaVM* vm)
{
	printf("[SMM] Loading SHARED libraries\n");

	unsigned lib_index = 0;
	while (luaL_Reg* plib_entry = (luaL_Reg *)&sharedLibs[lib_index++])
	{
		if (!plib_entry->name) break;
		printf("Loading lib: %s\n", plib_entry->name);

		luaL_register(vm->luaState, plib_entry->name, (luaL_reg*)plib_entry->func);
		lua_pop(vm->luaState, 1);
	}

	return o_sharedFunc((lua_State*)vm);
}