// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "loader.h"
#include <cstdio>
#include <iostream>

bool RedirectConsoleIO()
{
    bool result = true;
    FILE* fp;

    // Redirect STDIN if the console has an input handle
    if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
            result = false;
        else
            setvbuf(stdin, NULL, _IONBF, 0);

    // Redirect STDOUT if the console has an output handle
    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
            result = false;
        else
            setvbuf(stdout, NULL, _IONBF, 0);

    // Redirect STDERR if the console has an error handle
    if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
            result = false;
        else
            setvbuf(stderr, NULL, _IONBF, 0);

    // Make C++ standard streams point to console as well.
    std::ios::sync_with_stdio(true);

    // Clear the error state for each of the C++ standard streams.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

    return result;
}

const luaL_reg scripts_registration[] =
{
    {"smm",         lua_CFunction(&smm)},
    {"smm.chat",    lua_CFunction(&smm_chat)},
    {"smm.vm",      lua_CFunction(&smm_vm)},
    {"smm.steam",      lua_CFunction(&smm_steam)},
    {"smm.misc",      lua_CFunction(&smm_misc)},
    {"smm.color",      lua_CFunction(&smm_color)},
    {NULL, NULL}
};


luaL_Reg* clientLibs;
luaL_Reg* serverLibs;
luaL_Reg* sharedLibs;

lua_CFunction o_clientFunc;
lua_CFunction o_serverFunc;
lua_CFunction o_sharedFunc;

void Initialize(HMODULE hModule)
{
    const auto process_base = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    //
    // Bind to game's console
    //
    RedirectConsoleIO();

    //
    // Insert our loader into client, server and shared list.
    //
    const auto g_clientLibraries = reinterpret_cast<luaL_Reg**>(process_base + 0xDB5B98);
    const auto g_serverLibraries = reinterpret_cast<luaL_Reg**>(process_base + 0xDB5BB0);
    const auto g_sharedLibraries = reinterpret_cast<luaL_Reg*> (process_base + 0xDDF5C0);

    clientLibs = (luaL_Reg*)&scripts_registration;
    o_clientFunc = (*g_clientLibraries)->func;
    (*g_clientLibraries)->func = lua_CFunction(LoadClientLibraries);

    serverLibs = (luaL_Reg*)&scripts_registration;
    o_serverFunc = (*g_serverLibraries)->func;
    (*g_serverLibraries)->func = lua_CFunction(LoadServerLibraries);

    sharedLibs = (luaL_Reg*)&scripts_registration;
    o_sharedFunc = g_sharedLibraries->func;
    g_sharedLibraries->func = lua_CFunction(LoadSharedLibraries);

    //
    // Lazy loading
    //
    if (const auto lua_manager = SM::LuaManager::getInstancePtr())
    {
        if (lua_manager->m_bIsServer)
            LoadServerLibraries(lua_manager->m_pLuaVM);
        else
            LoadClientLibraries(lua_manager->m_pLuaVM);

        LoadSharedLibraries(lua_manager->m_pLuaVM);
    }

    ExitThread(TRUE);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(Initialize), hModule, NULL, NULL);
        break;
    case DLL_PROCESS_DETACH:
        const auto process_base = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

        const auto g_clientLibraries = reinterpret_cast<luaL_Reg**>(process_base + 0xDB5B98);
        const auto g_serverLibraries = reinterpret_cast<luaL_Reg**>(process_base + 0xDB5BB0);
        const auto g_sharedLibraries = reinterpret_cast<luaL_Reg*> (process_base + 0xDDF5C0);

        if (const auto lua_manager = SM::LuaManager::getInstancePtr())
        {
            // Unloading has a high likelyhood of causing a lua error and even crash
            lua_State* L = lua_manager->m_pLuaVM->luaState;

            lua_pushnil(L);
            
            for (auto& x : scripts_registration)
            {
                if (x.name)
                {
                    lua_setglobal(L, x.name);
                }
            }
            
            lua_pop(L, 1);
        }


        if (o_clientFunc)
            (*g_clientLibraries)->func = o_clientFunc;

        if (o_serverFunc)
            (*g_serverLibraries)->func = o_serverFunc;

        if (o_sharedFunc)
            g_sharedLibraries->func = o_sharedFunc;
        break;
    }
    return TRUE;
}

