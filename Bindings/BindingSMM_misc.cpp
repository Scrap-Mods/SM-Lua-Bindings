#include "pch.h"
#include "BindingSMM_misc.h"
#include <list>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

int LuaSMM_Misc_test(lua_State* L)
{
    printf("Listing %d class listeners for client_onFixedUpdate\n", SM::LuaManager::getInstancePtr()->m_onFixedUpdateStates.size());

    for (const auto& x : SM::LuaManager::getInstancePtr()->m_onFixedUpdateStates)
    {
        printf("ScriptRef: %d\n", x->scriptRef);
    }

    return 0;
}