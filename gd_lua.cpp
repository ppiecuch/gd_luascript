#include "gd_lua.h"

extern "C" {
	#include "lua/luaconf.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

#include "core/version.h"
#include "core/version_hash.gen.h"
#include "common/gd_core.h"

#include <vector>

constexpr const char *__init_func = "gd_init";
constexpr const char *__tick_func = "gd_tick";
constexpr const char *__draw_func = "gd_draw";
constexpr const char *__event_func = "gd_event";
constexpr const char *__term_func = "gd_term";

// Some macros
#define luapush_int(L, value)    lua_pushinteger(L, value)
#define luapush_float(L, value)  lua_pushnumber(L, value);
#define luapush_bool(L, value)   lua_pushboolean(L, value)
#define luapush_string(L, value) lua_pushstring(L, value)

#define luapush_opaque_type(L, str)                      luapush_opaque(L, &str, sizeof(str))
#define luapush_opaque_type_with_metatable(L, str, meta) luapush_opaque_with_metatable(L, &str, sizeof(str), #meta)

#define luaset_global_string(L, name, value) \
	lua_pushstring(L, value); \
	lua_setglobal(L, name)

#define luaset_global_bool(L, name, value) \
	lua_pushboolean(L, value); \
	lua_setglobal(L, name)

static void luapush_opaque(lua_State* L, void *ptr, size_t size) {
	void *ud = lua_newuserdata(L, size);
	memcpy(ud, ptr, size);
}

static void luapush_opaque_with_metatable(lua_State* L, void *ptr, size_t size, const char *metatable_name) {
	void *ud = lua_newuserdata(L, size);
	memcpy(ud, ptr, size);
	luaL_setmetatable(L, metatable_name);
}

static String get_full_version_string() {
	static String _version;
	if (_version.empty()) {
		String hash = String(VERSION_HASH);
		if (hash.length() != 0) {
			hash = "." + hash.left(9);
		}
		_version = String(VERSION_NAME) + " v" + String(VERSION_FULL_BUILD) + hash + " - " + String(VERSION_WEBSITE);
	}
	return _version;
}

static const std::vector<String> _default_locations = {
	"user://%s",
	"user://%s.lua",
	"user://lua/%s",
	"user://lua/%s.lua",
	"res://%s",
	"res://%s.lua",
	"res://lua/%s",
	"res://lua/%s.lua",
};

static int gd_loader(lua_State* state) {
	// get the module name
	const char* name = lua_tostring(state, 1);
	print_verbose(vformat("Trying to load module '%s' from resources ...", name));

	for (const auto &p : _default_locations) {
		String full = vformat(p, name);
		if (FileAccess::exists(full)) {
			FileAccessRef file = FileAccess::create_for_path(full);
			if (file) {
				Error err;
				String lua_string = file->get_file_as_string(full, &err);
				ERR_FAIL_COND_V_MSG(err != OK, 0, "Can not open file.");
				luaL_loadstring(state, lua_string.utf8().c_str());
			}
		}
	}
	return 0;
}

extern "C" void init_preloaders(lua_State *L);

// These 2 macros helps us in constructing general metamethods.
// We can use "lua" as a "Lua" pointer and arg1, arg2, ..., arg5 as Variants objects
// Check examples in createVector2Metatable
#define LUA_LAMBDA_TEMPLATE(_f_) \
	[] (lua_State* inner_state) -> int { \
		lua_pushstring(inner_state,"__Lua"); \
		lua_rawget(inner_state,LUA_REGISTRYINDEX); \
		GdLua* lua = (GdLua*) lua_touserdata(inner_state,-1); \
		lua_pop(inner_state,1); \
		Variant arg1 = lua->get_variant(1); \
		Variant arg2 = lua->get_variant(2); \
		Variant arg3 = lua->get_variant(3); \
		Variant arg4 = lua->get_variant(4); \
		Variant arg5 = lua->get_variant(5); \
		_f_ \
	}
#define LUA_METAMETHOD_TEMPLATE( lua_state , metatable_index , metamethod_name , _f_ ) \
	lua_pushstring(lua_state,metamethod_name); \
	lua_pushcfunction(lua_state,LUA_LAMBDA_TEMPLATE( _f_ )); \
	lua_settable(lua_state,metatable_index-2);

// expose a GDScript function to lua
void GdLua::expose_function(Object *instance, String function, String name) {

	// Createing lamda function so we can capture the object instanse and call the GDScript method.
	// Or in theory other scripting languages?
	auto f = [](lua_State* L) -> int {
		const Object *instance2 = (const Object*) lua_topointer(L, lua_upvalueindex(1));
		class GdLua *obj = (class GdLua*) lua_topointer(L, lua_upvalueindex(2));
		const char *function2 = lua_tostring(L, lua_upvalueindex(3));

		Variant arg1 = obj->get_variant(1);
		Variant arg2 = obj->get_variant(2);
		Variant arg3 = obj->get_variant(3);
		Variant arg4 = obj->get_variant(4);
		Variant arg5 = obj->get_variant(5);

		ScriptInstance *scriptInstance = instance2->get_script_instance();
		Variant returned = scriptInstance->call(function2, arg1, arg2, arg3, arg4, arg5);

		// Always returns something. If script instance doesn't returns anything, it will returns a NIL value anyway
		obj->push_variant(returned);
		return 1;
	};
	// Pushing the object instnace to the stack to be retrived when the function is called
	lua_pushlightuserdata(state, instance);
	// Pushing the referance of the class
	lua_pushlightuserdata(state, this);
	// Pushing the script function name string to the stack to br retrived when called
	lua_pushstring(state, function.ascii().get_data());
	// Pushing the actual lambda function to the stack
	lua_pushcclosure(state, f, 3);
	// Setting the global name for the function in lua
	lua_setglobal(state, name.ascii().get_data());
}

// call a Lua function from GDScript
Variant GdLua::call_function(String function_name, Array args, bool protected_call, Object* callback_caller, String callback) {
	Variant toReturn;
	int stack_size = lua_gettop(state);
	// put global function name on stack
	lua_getglobal(state, function_name.ascii().get_data());
	// push args
	for (int i = 0; i < args.size(); ++i) {
		push_variant(args[i]);
	}
	if ( protected_call ) {
		int ret = lua_pcall(state,args.size(), 1 , 0);
		if( ret != LUA_OK ) {
			// Default error handling:
			if ( callback_caller == nullptr || callback == String() ) {
				print_error( vformat("Error during \"GdLua::callFunction\" on Lua function \"%s\": ",function_name));
				handle_error( state , ret );
			} else {
				// Custom error handling:
				ScriptInstance *scriptInstance = callback_caller->get_script_instance();
				scriptInstance->call(callback, String(lua_tostring(state,-1)));
			}
		}
		toReturn = get_variant(1);
		lua_pop(state, 1);
	} else {
		lua_call(state, args.size(), 1);
		toReturn = get_variant(1);
		lua_pop(state, 1);
	}
	return toReturn;
}

bool GdLua::lua_function_exists(String function_name) {
	int type = lua_getglobal(state, function_name.ascii().get_data());
	lua_pop(state, 1);
	return type == LUA_TFUNCTION;
}

// doFile() will just load the file's text and call doString()
void GdLua::do_file(String fileName, bool protected_call, Object* callback_caller, String callback) {
	_File file;
	file.open(fileName,_File::ModeFlags::READ);
	String code = file.get_as_text();
	file.close();
	do_string(code, protected_call , callback_caller , callback);
}

// Called every line of lua that is ran
void GdLua::line_hook(lua_State *L, lua_Debug *ar) {
}

// Run lua string in a thread if threading is enabled
void GdLua::do_string(String code, bool protected_call, Object* callback_caller, String callback) {
	run_lua(state, code, protected_call, callback_caller, callback, &executing);
}

// Execute a lua script string and, if protected_call, call the passed callback function with the error as the aurgument if an error occurees
void GdLua::run_lua(lua_State *L, String code, bool protected_call, Object* callback_caller, String callback, bool *executing) {
	*executing = true;
	if (protected_call) {
		int ret = luaL_dostring(L, code.ascii().get_data());
		if( ret != LUA_OK ) {
			// Default error handling:
			if (callback_caller == nullptr || callback == String()) {
				handle_error(L, ret);
			} else {
				// Custom error handling:
				ScriptInstance *scriptInstance = callback_caller->get_script_instance();
				scriptInstance->call(callback, String(lua_tostring(L,-1)));
			}
		}
	} else {
		// Call not protected (crashes and exit the program if error!)
		luaL_loadstring(L, code.ascii().get_data()) ;
		lua_call(L, 0 /* nargs */ , 0 /* nresults */) ;
	}
	*executing = false;
}

// Push a GD Variant to the lua stack and return false if type is not supported (in this case, returns a nil value).
bool GdLua::push_variant(Variant var) {
	std::wstring str;
	switch (var.get_type()) {
		case Variant::Type::NIL:
			lua_pushnil(state);
			break;
		case Variant::Type::STRING:
			str = (var.operator String().c_str());
			lua_pushstring(state, std::string( str.begin(), str.end() ).c_str());
			break;
		case Variant::Type::INT:
			lua_pushinteger(state, (int64_t)var);
			break;
		case Variant::Type::REAL:
			lua_pushnumber(state,var.operator double());
			break;
		case Variant::Type::BOOL:
			lua_pushboolean(state, (bool)var);
			break;
		case Variant::Type::POOL_BYTE_ARRAY:
		case Variant::Type::POOL_INT_ARRAY:
		case Variant::Type::POOL_STRING_ARRAY:
		case Variant::Type::POOL_REAL_ARRAY:
		case Variant::Type::POOL_VECTOR2_ARRAY:
		case Variant::Type::POOL_VECTOR3_ARRAY:
		case Variant::Type::POOL_COLOR_ARRAY:
		case Variant::Type::ARRAY: {
			Array array = var.operator Array();
			lua_newtable(state);
			for(int i = 0; i < array.size(); i++) {
				Variant key = i+1;
				Variant value = array[i];
				push_variant(key);
				push_variant(value);
				lua_settable(state,-3);
			}
			break;
		}
		case Variant::Type::DICTIONARY:
			lua_newtable(state);
			for(int i = 0; i < ((Dictionary)var).size(); i++) {
				Variant key = ((Dictionary)var).keys()[i];
				Variant value = ((Dictionary)var)[key];
				push_variant(key);
				push_variant(value);
				lua_settable(state, -3);
			}
			break;
		case Variant::Type::VECTOR2: {
			void* userdata = (Variant*)lua_newuserdata(state , sizeof(Variant));
			memcpy(userdata, (void*)&var, sizeof(Variant));
			luaL_setmetatable(state,"godot.Vector2");
			break;
		}
		case Variant::Type::VECTOR3: {
			void* userdata = (Variant*)lua_newuserdata(state , sizeof(Variant));
			memcpy(userdata , (void*)&var, sizeof(Variant));
			luaL_setmetatable(state, "godot.Vector3");
			break;
		}
		case Variant::Type::RECT2: {
			void* userdata = (Variant*)lua_newuserdata(state , sizeof(Variant));
			memcpy(userdata, (void*)&var , sizeof(Variant));
			luaL_setmetatable(state, "godot.Rect");
			break;
		}
		case Variant::Type::COLOR: {
			void* userdata = (Variant*)lua_newuserdata( state , sizeof(Variant));
			memcpy(userdata , (void*)&var , sizeof(Variant));
			luaL_setmetatable(state, "godot.Color");
			break;
		}
		default:
			print_error( vformat("Can't pass Variants of type \"%s\" to Lua." , Variant::get_type_name( var.get_type() ) ) );
			lua_pushnil(state);
			return false;
	}
	return true;
}

// Call pushVariant() and set it to a global name
bool GdLua::push_global_variant(Variant var, String name) {
	if (push_variant(var)) {
		std::wstring str = name.c_str();
		lua_setglobal(state,std::string( str.begin(), str.end() ).c_str());
		return true;
	}
	return false;
}

// Pull a global variant from Lua to GDScript
Variant GdLua::pull_variant(String name) {
	int type = lua_getglobal(state, name.ascii().get_data());
	Variant val = get_variant(1);
	lua_pop(state, 1);
	return val;
}
// get a value at the given index and return as a variant
Variant GdLua::get_variant(int index) {
	Variant result;
	int type = lua_type(state, index);
	switch (type) {
		case LUA_TSTRING:
			result = lua_tostring(state, index);
			break;
		case LUA_TNUMBER:
			result = lua_tonumber(state, index);
			break;
		case LUA_TBOOLEAN:
			result = (bool)lua_toboolean(state, index);
			break;
		case LUA_TUSERDATA:
			result = *(Variant*)lua_touserdata(state,index);
			break;
		case LUA_TTABLE: {
			Dictionary dict;
			lua_pushnil(state);  /* first key */
			while (lua_next( state , (index<0)?(index-1):(index)  ) != 0) {
				Variant key = get_variant(-2);
				Variant value = get_variant(-1);
				dict[key] = value;
				lua_pop(state, 1);
			}
			result = dict;
			break;
		}
		default:
			result = Variant();
	}
	return result;
}

void GdLua::expose_constructors() {
	lua_pushcfunction(state, LUA_LAMBDA_TEMPLATE({
		int argc = lua_gettop(inner_state);
		if (argc == 0) {
			lua->push_variant(Vector2());
		} else {
			lua->push_variant(Vector2(arg1.operator double(), arg2.operator double() ));
		}
		return 1;
	}));
	lua_setglobal(state, "godot.Vector2");

	lua_pushcfunction(state, LUA_LAMBDA_TEMPLATE({
		int argc = lua_gettop(inner_state);
		if (argc == 0) {
			lua->push_variant(Vector3());
		} else {
			lua->push_variant(Vector3(arg1.operator double(), arg2.operator double(), arg3.operator double()));
		}
		return 1;
	}));
	lua_setglobal(state, "godot.Vector3");

	lua_pushcfunction(state,LUA_LAMBDA_TEMPLATE({
		int argc = lua_gettop(inner_state);
		if (argc == 3) {
			lua->push_variant(Color( arg1.operator double(), arg2.operator double() , arg3.operator double()));
		} else if (argc == 4) {
			lua->push_variant(Color( arg1.operator double(), arg2.operator double() , arg3.operator double() , arg4.operator double()));
		} else {
			lua->push_variant(Color());
		}
		return 1;
	}));
	lua_setglobal(state, "godot.Color");

	lua_pushcfunction(state,LUA_LAMBDA_TEMPLATE({
		int argc = lua_gettop(inner_state);
		if (argc == 4) {
			lua->push_variant(Rect2( arg1.operator double(), arg2.operator double() , arg3.operator double() , arg4.operator double()));
		} else {
			lua->push_variant(Rect2());
		}
		return 1;
	}));
	lua_setglobal(state, "godot.Rect");
}

// Create metatable for Vector2 and saves it at LUA_REGISTRYINDEX with name "godot.Vector2"
void GdLua::create_vector2_metatable() {
	luaL_newmetatable(state , "godot.Vector2");

	LUA_METAMETHOD_TEMPLATE(state , -1, "__index", {
		lua->push_variant(arg1.get(arg2));
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__newindex", {
		// We can't use arg1 here because we need to reference the userdata
		((Variant*)lua_touserdata(inner_state, 1))->set(arg2 , arg3);
		return 0;
	});	

	LUA_METAMETHOD_TEMPLATE(state, -1, "__add", {
		lua->push_variant(arg1.operator Vector2() + arg2.operator Vector2());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__sub", {
		lua->push_variant(arg1.operator Vector2() - arg2.operator Vector2());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__mul", {
		switch( arg2.get_type() ) {
			case Variant::Type::VECTOR2:
				lua->push_variant(arg1.operator Vector2() * arg2.operator Vector2());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Vector2() * arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__div", {
		switch(arg2.get_type()) {
			case Variant::Type::VECTOR2:
				lua->push_variant(arg1.operator Vector2() / arg2.operator Vector2());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Vector2() / arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	lua_pop(state, 1); // Stack is now unmodified
}

// Create metatable for Vector3 and saves it at LUA_REGISTRYINDEX with name "godot.Vector3"
void GdLua::create_vector3_metatable() {

	luaL_newmetatable(state, "godot.Vector3");

	LUA_METAMETHOD_TEMPLATE(state, -1, "__index", {
		lua->push_variant(arg1.get(arg2));
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__newindex", {
		// We can't use arg1 here because we need to reference the userdata
		((Variant*)lua_touserdata(inner_state,1))->set(arg2 , arg3);
		return 0;
	});	

	LUA_METAMETHOD_TEMPLATE(state, -1, "__add", {
		lua->push_variant(arg1.operator Vector3() + arg2.operator Vector3());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__sub", {
		lua->push_variant(arg1.operator Vector3() - arg2.operator Vector3());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__mul", {
		switch(arg2.get_type()) {
			case Variant::Type::VECTOR3:
				lua->push_variant(arg1.operator Vector3() * arg2.operator Vector3());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Vector3() * arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__div", {
		switch( arg2.get_type() ){
			case Variant::Type::VECTOR3:
				lua->push_variant(arg1.operator Vector3() / arg2.operator Vector3());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Vector3() / arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	lua_pop(state, 1); // Stack is now unmodified
}

// Create metatable for Color and saves it at LUA_REGISTRYINDEX with name "godot.Color"
void GdLua::create_color_metatable() {

	luaL_newmetatable(state, "godot.Color");

	LUA_METAMETHOD_TEMPLATE(state, -1, "__index", {
		lua->push_variant(arg1.get(arg2));
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__newindex", {
		// We can't use arg1 here because we need to reference the userdata
		((Variant*)lua_touserdata(inner_state,1))->set(arg2 , arg3);
		return 0;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__add", {
		lua->push_variant(arg1.operator Color() + arg2.operator Color());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE( state, -1 , "__sub", {
		lua->push_variant(arg1.operator Color() - arg2.operator Color());
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1 , "__mul", {
		switch( arg2.get_type() ) {
			case Variant::Type::COLOR:
				lua->push_variant(arg1.operator Color() * arg2.operator Color());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Color() * arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__div", {
		switch( arg2.get_type() ) {
			case Variant::Type::COLOR:
				lua->push_variant(arg1.operator Color() / arg2.operator Color());
				return 1;
			case Variant::Type::INT:
			case Variant::Type::REAL:
				lua->push_variant(arg1.operator Color() / arg2.operator double());
				return 1;
			default:
				return 0;
		}
	});

	lua_pop(state, 1); // Stack is now unmodified
}

// Create metatable for Rect and saves it at LUA_REGISTRYINDEX with name "godot.Rect"
void GdLua::create_rect_metatable() {

	luaL_newmetatable(state, "godot.Rect");

	LUA_METAMETHOD_TEMPLATE(state, -1, "__index", {
		lua->push_variant(arg1.get(arg2));
		return 1;
	});

	LUA_METAMETHOD_TEMPLATE(state, -1, "__newindex", {
		// We can't use arg1 here because we need to reference the userdata
		((Variant*)lua_touserdata(inner_state,1))->set(arg2 , arg3);
		return 0;
	});

	lua_pop(state, 1); // Stack is now unmodified
}

// Assumes there is a error in the top of the stack. Pops it.
void GdLua::handle_error(lua_State* L , int lua_error) {
	String msg;
	switch(lua_error) {
		case LUA_ERRRUN:
			msg += "[LUA_ERRNUN - runtime error ] ";
			break;
		case LUA_ERRMEM:
			msg += "[LUA_ERRMEM - memory allocation error ] ";
			break;
		case LUA_ERRERR:
			msg += "[LUA_ERRERR - error while handling another error ] ";
			break;
		default: break;
	}
	msg += lua_tostring(L, -1);
	print_error(msg);
	lua_pop(L,1);
}

// Lua functions
// Change lua's print function to print to the Godot console by default
int GdLua::lua_print(lua_State* state) {
	int args = lua_gettop(state);
	String final_string;
	for ( int n=1; n<=args; ++n) {
		String it_string;
		switch( lua_type(state,n) ) {
			case LUA_TUSERDATA: {
				Variant var = *(Variant*) lua_touserdata(state,n);
				it_string = var.operator String();
				break;
			}
			default: {
				it_string = lua_tostring(state, n);
				break;
			}
		}
		final_string += it_string;
		if (n < args) final_string += ", ";
	}
	print_line(final_string);
	return 0;
}

void GdLua::set_debug() {
	// set error reporting
	lua_getglobal(state, "debug");
	lua_getfield(state, -1, "traceback");
}

void GdLua::_bind_methods() {
	ClassDB::bind_method(D_METHOD("do_file", "File", "ProtectedCall" , "CallbackCaller" , "Callback" ), &GdLua::do_file, DEFVAL(true) , DEFVAL(Variant()) , DEFVAL(String()));
	ClassDB::bind_method(D_METHOD("do_string", "Code", "ProtectedCall" , "CallbackCaller" , "Callback" ), &GdLua::do_string, DEFVAL(true) , DEFVAL(Variant()) , DEFVAL(String()) );
	ClassDB::bind_method(D_METHOD("push_variant", "var", "Name"),&GdLua::push_global_variant);
	ClassDB::bind_method(D_METHOD("pull_variant", "Name"),&GdLua::pull_variant);
	ClassDB::bind_method(D_METHOD("expose_function", "NodeObject", "GDFunction", "LuaFunctionName"), &GdLua::expose_function);
	ClassDB::bind_method(D_METHOD("call_function","LuaFunctionName", "Args", "ProtectedCall", "CallbackCaller", "Callback" ), &GdLua::call_function, DEFVAL(true) , DEFVAL(Variant()), DEFVAL(String()));
	ClassDB::bind_method(D_METHOD("lua_function_exists","LuaFunctionName"), &GdLua::lua_function_exists);
}

GdLua::GdLua() {
	// Createing lua state instance
	state = luaL_newstate();

	luapush_bool(state, true);
#if defined(OSX_ENABLED)
    lua_setglobal(state, "PLATFORM_OSX");
#elif defined(UWP_ENABLED)
    lua_setglobal(state, "PLATFORM_WINDOWS_UWP");
#elif defined(ANDROID_ENABLES)
    lua_setglobal(state, "PLATFORM_ANDROID");
#elif defined(JAVASCRIPT_ENABLED)
    lua_setglobal(state, "PLATFORM_JAVASCRIPT");
#endif
#ifdef DEBUG_ENABLED
	luapush_bool(state, true);
#else
	luapush_bool(state, false);
#endif
    lua_setglobal(state, "PLATFORM_DEBUG");

	luaset_global_string(state, "PLATFORM_VERSION", get_full_version_string().utf8().c_str());
	luaset_global_bool(state, "PLATFORM_VERBOSE", OS::get_singleton()->is_stdout_verbose());
	luaset_global_string(state, "PLATFORM_DATA_DIR", OS::get_singleton()->get_data_path().utf8().c_str());
	luaset_global_string(state, "PLATFORM_USER_DATA_DIR", OS::get_singleton()->get_user_data_dir().utf8().c_str());

	// loading base libs
	luaL_openlibs(state);

	// add custom loader
	lua_getglobal(state, "package");
	lua_getfield(state, -1, "loaders");
	const int num_loaders = lua_objlen(state, -1);
	lua_pushcfunction(state, gd_loader);
	lua_rawseti(state, -2, num_loaders+1);
	lua_pop(state, 1);
	// additional search path
	lua_getfield(state, -1, "path");
	lua_pushstring(state, ";/mnt/sdcard/?.lua");
	lua_concat(state, 2);
	lua_setfield(state, -2, "path");
	lua_pop(state, 1);
	// end customization

	lua_sethook(state, &line_hook, LUA_MASKLINE, 0);
	lua_register(state, "print", lua_print);

	// saving this object into registry
	lua_pushstring(state,"__Lua");
	lua_pushlightuserdata(state , this);
	lua_rawset(state , LUA_REGISTRYINDEX);

    init_preloaders(state);

	// Creating basic types metatables and saving them in registry
	create_vector2_metatable(); // "godot.Vector2"
	create_vector3_metatable(); // "godot.Vector3"
	create_color_metatable(); // "godot.Color"
	// Exposing basic types constructors
	expose_constructors();
}

GdLua::~GdLua() {
	// Warning users about object destruction if code is currently being executed see https://github.com/Trey2k/lua/issues/9
	if (executing) {
		WARN_PRINT("WARNING! Lua object is being destroyed while code is currently being executed.");
	}
	// Destroying lua state instance
	lua_close(state);
}


void GdLuaInstance::set_script_path(const String &p_script_path) {
	script_path = p_script_path;
}

String GdLuaInstance::get_script_path() const {
	return script_path;
}

void GdLuaInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_script_path"), &GdLuaInstance::set_script_path);
	ClassDB::bind_method(D_METHOD("get_script_path"), &GdLuaInstance::get_script_path);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "script_path", PROPERTY_HINT_FILE, "*.lua"), "set_script_path", "get_script_path");
}

GdLuaInstance::GdLuaInstance() {
	lua = newref(GdLua);
}

GdLuaInstance::~GdLuaInstance() { }
