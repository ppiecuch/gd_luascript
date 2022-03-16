#include "core/version.h"
#include "servers/visual_server.h"

#include "godot_lua.h"

extern "C" {
	#include "lua/lauxlib.h"
}

// Internal type definitions
struct lua_Constant {
	char *str;
	int  value;
};

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

// Check the number of arguments on the Lua stack
static int bad_args(lua_State *L, int n, char const *name) {
	if(lua_gettop(L) != n) {
		lua_settop(L, 0);
		lua_pushstring(L, "Bad arguments passed to function: ");
		lua_pushstring(L, name);
		lua_concat(L, 2);
		lua_error(L);
		return 1;
	}
	return 0;
}

// Get a numeric value from a Lua table
static void get_number(lua_State *L, int index, char *key, lua_Number *num) {
	lua_pushstring(L, key);
	if(index < 0) {
		index--;
	}
	lua_rawget(L, index );
	if(!lua_isnil(L, -1)) {
		*num = lua_tonumber(L, -1);
	}
	lua_remove(L, -1);
}

// Add constants to the table on top of the stack
static void add_constants(lua_State *L, struct lua_Constant *cn) {
	while(cn->str) {
		lua_pushstring(L, cn->str);
		lua_pushnumber(L, cn->value);
		lua_rawset(L, -3);
		++ cn;
	}
}

// Report error for unsupported functions
static void unsupported_function(lua_State *L, char const *name) {
	lua_settop(L, 0);
	lua_pushstring(L, "Unsupported function: ");
	lua_pushstring(L, name);
	lua_concat(L, 2);
	lua_error(L);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern "C" int tolua_godot_open (lua_State *L);
extern "C" int luaopen_godot (lua_State *L);

RID get_canvas(lua_State *L) {
	return RID();
}

VisualServer *get_vs() {
	return VisualServer::get_singleton();
}
