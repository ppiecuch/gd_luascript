#ifndef GD_LUA_H
#define GD_LUA_H

#include "core/reference.h"
#include "core/bind/core_bind.h"
#include "scene/main/node.h"

#include "lua/lua.h"

#include <string>
#include <thread>
#include <mutex>

class GdLua : public Reference {
	GDCLASS(GdLua, Reference);

	lua_State *state;
	bool executing;

	void expose_constructors();

	void create_vector2_metatable();
	void create_vector3_metatable();
	void create_color_metatable();
	void create_rect_metatable();

	static void handle_error(lua_State *L, int lua_error);

protected:
	static void _bind_methods();

public:
	void set_debug();

	void expose_function(Object *instance, String function, String name);
	void do_file(String fileName, bool protected_call = true, Object* callback_caller = nullptr, String callback = String());
	void do_string(String code, bool protected_call = true, Object* callback_caller = nullptr, String callback = String());

	static void run_lua(lua_State *L, String code, bool protected_call, Object* callback_caller, String callback, bool *executing);

	bool push_variant(Variant var);
	bool push_global_variant(Variant var, String name);
	bool lua_function_exists(String function_name);

	Variant pull_variant(String name);
	Variant get_variant(int index = -1);
	Variant call_function(String function_name, Array args, bool protected_call = true, Object* callback_caller = nullptr, String callback = String());

	// Lua functions
	static void line_hook(lua_State *L, lua_Debug *ar);
	static int lua_print(lua_State* state);

	GdLua();
	~GdLua();
};

class GdLuaInstance : public Node {
	GDCLASS(GdLuaInstance, Node);

	Ref<GdLua> lua;
	String script_path;

protected:
	static void _bind_methods();

public:
	void set_script_path(const String &p_script_path);
	String get_script_path() const;

	GdLuaInstance();
	~GdLuaInstance();
};

#endif // GD_LUA_H
