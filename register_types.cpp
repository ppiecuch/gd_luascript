#include "core/class_db.h"
#include "register_types.h"
#include "gd_lua.h"

void register_gd_luascript_types(){
	ClassDB::register_class<GdLua>();
	ClassDB::register_class<GdLuaInstance>();
}

void unregister_gd_luascript_types() {
}
