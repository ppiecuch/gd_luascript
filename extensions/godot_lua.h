#ifndef GODOT_LUA_H
#define GODOT_LUA_H

#include "core/rid.h"

class VisualServer;
struct lua_State;

RID get_canvas(lua_State *L);
VisualServer *get_vs();

#endif // GODOT_LUA_H
