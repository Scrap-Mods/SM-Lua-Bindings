#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

int LuaSMM_Color_new(lua_State* L);
int LuaSMM_Color_paintBlock(lua_State* L);
int LuaSMM_Color_paintPart(lua_State* L);

int LuaSMM_Color_getHexInt(lua_State* L);
int LuaSMM_Color_getHSVA(lua_State* L);

int LuaSMM_Color_fromHSVA(lua_State* L);


const luaL_reg smm_color[] =
{
    {"new",             LuaSMM_Color_new},
    {"paintBlock",      LuaSMM_Color_paintBlock},
    {"paintPart",       LuaSMM_Color_paintPart},
    {"getHexInt",       LuaSMM_Color_getHexInt},
    {"getHSVA",         LuaSMM_Color_getHSVA},
    {"fromHSVA",        LuaSMM_Color_fromHSVA},
    {NULL, NULL}
};
