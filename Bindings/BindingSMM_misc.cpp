#include "pch.h"
#include "BindingSMM_misc.h"
#include <list>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

int LuaSMM_Misc_test(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 argument, got %d", lua_gettop(L));
    }

    uint32_t body_id = luaL_checkinteger(L, 1);

    boost::uuids::string_generator gen;
    // Paint Tool UUID
    boost::uuids::uuid uuid_painttool = gen("c60b9627-fc2b-4319-97c5-05921cb976c6");

    SM::ToolManager_GetToolByName_Result result;
    SM::ToolManager::getInstancePtr()->GetToolByName(&result, uuid_painttool);
    
    SM::PaintTool::PaintData x = { 0 };
    x.m_bodyId = body_id;
    x.m_boolTrue = 1;
    x.m_faceX = 0.5f;
    x.m_faceY = 0.5f;
    x.m_faceZ = 0.5f;
    x.zero_0 = 0;
    x.zero_1 = 0;
    x.m_negativeOne = -1;
    x.m_startX = 200;
    x.m_startY = 200;
    x.m_startZ = 200;
    x.m_endX = -200;
    x.m_endY = -200;
    x.m_endZ = -200;
    
    const auto offset_paintblock = PVOID(reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + 0x2CAB20);
    bool (*paintBlock)(SM::PaintTool*, SM::PaintTool::PaintData*, unsigned int);
    paintBlock = decltype(paintBlock)(offset_paintblock);

    paintBlock((SM::PaintTool*)result.pToolImplementation, &x, 0xFFFF0000);

    lua_settop(L, 0);
    return 0;
}