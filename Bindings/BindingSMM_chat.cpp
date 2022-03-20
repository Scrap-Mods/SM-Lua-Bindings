#include "pch.h"
#include "BindingSMM_chat.h"

int LuaSMM_Chat_getName(lua_State* L)
{
    lua_settop(L, 0);
    lua_pushstring(L, SM::InGameGuiManager::getInstancePtr()->m_chatGui->m_name.c_str());
    return 1;
}

int LuaSMM_Chat_clear(lua_State* L)
{
    lua_settop(L, 0);

    const auto chatgui = SM::InGameGuiManager::getInstancePtr()->m_chatGui;
    //chatgui->m_totalMessages = 0;
    chatgui->m_widgetReceivedMessages->setCaption("");

    return 0;
}

int LuaSMM_Chat_setName(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    const char* name = nullptr;
    size_t size = -1;

    if (name = luaL_checklstring(L, 1, &size))
    {
        SM::InGameGuiManager::getInstancePtr()->m_chatGui->m_name.assign(name, size);
    }

    lua_settop(L, 0);
    return 0;
}

int LuaSMM_Chat_sendChatMessage(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    const char* message = nullptr;
    size_t size = -1;

    if (message = luaL_checklstring(L, 1, &size))
    {
        SM::InGameGuiManager::getInstancePtr()->m_chatGui->m_widgetMessage->setCaptionWithReplacing(std::string(message, size));
        SM::InGameGuiManager::getInstancePtr()->m_chatGui->m_widgetMessage->_riseKeyButtonPressed(MyGUI::KeyCode::Return, 0xd);
    }

    lua_settop(L, 0);
    return 0;
}
