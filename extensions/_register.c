#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

// preloaders to statically link in binary lua modules
#define REGISTER_LOADER(module_name, loader) \
	int loader(lua_State *L); \
	lua_pushcfunction(L, loader); \
	lua_setfield(L, -2, module_name)

void init_preloaders(lua_State *L) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");

	// add your custom loaders here, they look like this:
	REGISTER_LOADER("collision.native", luaopen_collision_native);
	REGISTER_LOADER("godot", luaopen_godot);
	REGISTER_LOADER("memarray", luaopen_memarray);

#ifdef EXTRA_LOADERS
# include EXTRA_LOADERS
#endif

	lua_pop(L, 2);
}
