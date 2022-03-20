#include "pch.h"
#include "BindingSMM_color.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/algorithm/clamp.hpp>

struct Vec3
{
    float x;
    float y;
    float z;
};

union IntColor
{
    unsigned int all;
    struct
    {
        unsigned char a;
        unsigned char b;
        unsigned char g;
        unsigned char r;
    } data;
};

union BigEndianIntColor
{
    unsigned int all;
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } data;
};

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    }
    else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if (in.r >= max)                           // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta;        // between yellow & magenta
    else
        if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if (out.h < 0.0)
        out.h += 360.0;

    return out;
}
rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

int LuaSMM_Color_new(lua_State* L)
{
    if (lua_gettop(L) == 1)
    {
        int type_1 = lua_type(L, 1);
        if (type_1 == LUA_TSTRING)
        {
            BigEndianIntColor int_color;
            auto string_color = std::string(luaL_checkstring(L, 1));
            for (size_t i = 0, middle = string_color.size() / 2, size = string_color.size(); i < middle; i += 2)
            {
                std::swap(string_color[i], string_color[size - i - 2]);
                std::swap(string_color[i + 1], string_color[size - i - 1]);
            }

            int_color.all = stoul(string_color, 0, 16);

            Color* result = (Color*)lua_newuserdata(L, sizeof(Color));
            result->r = int_color.data.r / 255.f;
            result->g = int_color.data.g / 255.f;
            result->b = int_color.data.b / 255.f;

            if (string_color.size() <= 6)
            {
                result->a = 1.f;
            }
            else
            {
                result->a = int_color.data.a / 255.f;
            } 

            lua_getfield(L, LUA_REGISTRYINDEX, "Color");
            lua_setmetatable(L, -2);

            return 1;
        }
        else if (type_1 == LUA_TNUMBER)
        {
            IntColor int_color;
            int_color.all = luaL_checkint(L, 1);

            Color* result = (Color *)lua_newuserdata(L, sizeof(Color));
            result->a = int_color.data.a / 255.f;
            result->r = int_color.data.r / 255.f;
            result->g = int_color.data.g / 255.f;
            result->b = int_color.data.b / 255.f;
            
            lua_getfield(L, LUA_REGISTRYINDEX, "Color");
            lua_setmetatable(L, -2);

            return 1;
        } 
        else if (type_1 == LUA_TUSERDATA)
        {
            auto c = (Color*)luaL_checkudata(L, 1, "Color");
            printf("A:%f R:%f G:%f B:%f\n", c->a, c->r, c->g, c->b);

            // Add own functions to metatable

            return 1;
        }
        else {
            return luaL_error(L, "Expected string or number, got %s", lua_typename(L, 1));
        }
    }
    else if (lua_gettop(L) == 4)
    {
        Color* result = (Color*)lua_newuserdata(L, sizeof(Color));
        
        result->r = boost::algorithm::clamp(luaL_checknumber(L, 1), 0.f, 1.f);
        result->g = boost::algorithm::clamp(luaL_checknumber(L, 2), 0.f, 1.f);
        result->b = boost::algorithm::clamp(luaL_checknumber(L, 3), 0.f, 1.f);
        result->a = boost::algorithm::clamp(luaL_checknumber(L, 4), 0.f, 1.f);

        lua_getfield(L, LUA_REGISTRYINDEX, "Color");
        lua_setmetatable(L, -2);

        return 1;
    }

    return luaL_error(L, "Expected 1 or 4 arguments, got %d", lua_gettop(L));
}

int LuaSMM_Color_paintBlock(lua_State* L)
{
    if (lua_gettop(L) != 4)
    {
        return luaL_error(L, "Expected 4 arguments, got %d", lua_gettop(L));
    }

    auto bodyID = (int*)luaL_checkudata(L, 1, "Body");
    auto startPos = (Vec3*)luaL_checkudata(L, 2, "Vec3");
    auto endPos = (Vec3*)luaL_checkudata(L, 3, "Vec3");
    auto c = (Color*)luaL_checkudata(L, 4, "Color");

    //printf("Body ID: %d\n", *bodyID);
    //printf("= Start\nX: %f\nY: %f\nZ: %f\n", startPos->x, startPos->y, startPos->z);
    //printf("= End\nX: %f\nY: %f\nZ: %f\n", endPos->x, endPos->y, endPos->z);
    //printf("Color: A:%f R:%f G:%f B:%f\n", c->a, c->r, c->g, c->b);

    static boost::uuids::string_generator gen;
    // Paint Tool UUID
    static boost::uuids::uuid uuid_painttool = gen("c60b9627-fc2b-4319-97c5-05921cb976c6");

    static SM::ToolManager_GetToolByName_Result result;
    if (!result.pToolImplementation)
        SM::ToolManager::getInstancePtr()->GetToolByName(&result, uuid_painttool);

    static SM::PaintTool::PaintData x = { 0 };
    x.m_bodyId = *bodyID;
    x.m_boolTrue = 1;
    x.m_faceX = 0.5f;
    x.m_faceY = 0.5f;
    x.m_faceZ = 0.5f;
    x.zero_0 = 0;
    x.zero_1 = 0;
    x.m_negativeOne = -1;
    x.m_startX = startPos->x;
    x.m_startY = startPos->y;
    x.m_startZ = startPos->z;
    x.m_endX = endPos->x;
    x.m_endY = endPos->y;
    x.m_endZ = endPos->z;

    static const auto offset_paintblock = PVOID(reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + 0x2CAB20);
    static bool (*paintBlock)(SM::PaintTool*, SM::PaintTool::PaintData*, unsigned int) = decltype(paintBlock)(offset_paintblock);

    static BigEndianIntColor color{ 0 };
    color.data.r = c->r * 255.f;
    color.data.g = c->g * 255.f;
    color.data.b = c->b * 255.f;
    color.data.a = c->a * 255.f;

    //printf("Color2: %llx\n", color.all);
    //printf("Color2 A:%d R:%d G:%df B:%d\n", color.data.a, color.data.r, color.data.g, color.data.b);

    paintBlock((SM::PaintTool*) result.pToolImplementation, &x, color.all);

    return 0;
}

int LuaSMM_Color_paintPart(lua_State* L)
{
    if (lua_gettop(L) != 4)
    {
        return luaL_error(L, "Expected 4 arguments, got %d", lua_gettop(L));
    }

    auto bodyID = (int*)luaL_checkudata(L, 1, "Body");
    auto startPos = (Vec3*)luaL_checkudata(L, 2, "Vec3");
    auto endPos = (Vec3*)luaL_checkudata(L, 3, "Vec3");
    auto c = (Color*)luaL_checkudata(L, 4, "Color");

    static boost::uuids::string_generator gen;
    static boost::uuids::uuid uuid_painttool = gen("c60b9627-fc2b-4319-97c5-05921cb976c6");

    static SM::ToolManager_GetToolByName_Result result;
    if (!result.pToolImplementation)
        SM::ToolManager::getInstancePtr()->GetToolByName(&result, uuid_painttool);

    static SM::PaintTool::PaintData x = { 0 };
    x.m_bodyId = *bodyID;
    x.m_boolTrue = 1;
    x.m_faceX = 0.5f;
    x.m_faceY = 0.5f;
    x.m_faceZ = 0.5f;
    x.zero_0 = 0;
    x.zero_1 = 0;
    x.m_negativeOne = -1;
    x.m_startX = startPos->x;
    x.m_startY = startPos->y;
    x.m_startZ = startPos->z;
    x.m_endX = endPos->x;
    x.m_endY = endPos->y;
    x.m_endZ = endPos->z;

    static const auto offset_paintpart = PVOID(reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + 0x2CA000);
    static bool (*paintPart)(SM::PaintTool*, SM::PaintTool::PaintData*, unsigned int) = decltype(paintPart)(offset_paintpart);

    static BigEndianIntColor color{ 0 };
    color.data.r = c->r * 255.f;
    color.data.g = c->g * 255.f;
    color.data.b = c->b * 255.f;
    color.data.a = c->a * 255.f;

    paintPart((SM::PaintTool*)result.pToolImplementation, &x, color.all);

    return 0;
}

int LuaSMM_Color_getHexInt(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 arguments, got %d", lua_gettop(L));
    }

    auto c = (Color*)luaL_checkudata(L, 1, "Color");
    static BigEndianIntColor color{ 0 };
    color.data.r = c->r * 255.f;
    color.data.g = c->g * 255.f;
    color.data.b = c->b * 255.f;
    color.data.a = c->a * 255.f;

    lua_pushinteger(L, color.all);

    return 1;
}

int LuaSMM_Color_getHSVA(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "Expected 1 arguments, got %d", lua_gettop(L));
    }

    auto color = (Color*)luaL_checkudata(L, 1, "Color");
    auto hsv = rgb2hsv(rgb{ color->r, color->g, color->b });

    lua_pushnumber(L, hsv.h);
    lua_pushnumber(L, hsv.s);
    lua_pushnumber(L, hsv.v);
    lua_pushnumber(L, color->a);

    return 4;
}

int LuaSMM_Color_fromHSVA(lua_State* L)
{
    if (lua_gettop(L) != 4)
    {
        return luaL_error(L, "Expected 4 arguments, got %d", lua_gettop(L));
    }

    double H = luaL_checknumber(L, 1);
    double S = boost::algorithm::clamp(luaL_checknumber(L, 2), 0.f, 1.f);
    double V = boost::algorithm::clamp(luaL_checknumber(L, 3), 0.f, 1.f);
    double A = boost::algorithm::clamp(luaL_checknumber(L, 4), 0.f, 1.f);
    Color* result = (Color*)lua_newuserdata(L, sizeof(Color));
    result->a = A;
    
    auto rgb = hsv2rgb(hsv{ H, S, V });
    result->r = rgb.r;
    result->g = rgb.g;
    result->b = rgb.b;

    lua_getfield(L, LUA_REGISTRYINDEX, "Color");
    lua_setmetatable(L, -2);

    return 1;
}