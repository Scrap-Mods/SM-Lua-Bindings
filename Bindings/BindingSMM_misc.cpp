#include "pch.h"
#include "BindingSMM_misc.h"
#include <list>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

int LuaSMM_Misc_isDeveloperMode(lua_State* L)
{
    lua_pushboolean(L, *SM::IsDeveloperMode());
    return 1;
}