/*
** Lua binding: godot
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_godot_open (lua_State* tolua_S);
LUALIB_API int luaopen_godot (lua_State* tolua_S);

#include "core/variant.h"
#include "core/resource.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/rect2.h"
#include "servers/visual_server.h"
#include "godot_lua.h"
#define NinePatchAxisMode VisualServer::NinePatchAxisMode
#define NINE_PATCH_STRETCH VisualServer::NINE_PATCH_STRETCH
#define NINE_PATCH_TILE VisualServer::NINE_PATCH_TILE
#define NINE_PATCH_TILE_FIT VisualServer::NINE_PATCH_TILE_FIT

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_RID (lua_State* tolua_S)
{
 RID* self = (RID*) tolua_tousertype(tolua_S,1,0);
 tolua_release(tolua_S,self);
 delete self;
 return 0;
}

static int tolua_collect_Vector2 (lua_State* tolua_S)
{
 Vector2* self = (Vector2*) tolua_tousertype(tolua_S,1,0);
 tolua_release(tolua_S,self);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Vector2");
 tolua_usertype(tolua_S,"Vector<Color>");
 tolua_usertype(tolua_S,"Vector<float>");
 tolua_usertype(tolua_S,"Rect2");
 tolua_usertype(tolua_S,"Vector<Point2>");
 tolua_usertype(tolua_S,"RID");
 tolua_usertype(tolua_S,"Transform2D");
 tolua_usertype(tolua_S,"Vector<int>");
 tolua_usertype(tolua_S,"VisualServer");
 tolua_usertype(tolua_S,"Color");
 tolua_usertype(tolua_S,"Vector3");
}

/* method: length of class  Vector2 */
static int tolua_godot_godot_Vector2_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Vector2",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Vector2* self = (const Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'",NULL);
#endif
 {
  float tolua_ret = (float)  self->length();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}

/* method: length_squared of class  Vector2 */
static int tolua_godot_godot_Vector2_length_squared00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Vector2",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Vector2* self = (const Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length_squared'",NULL);
#endif
 {
  float tolua_ret = (float)  self->length_squared();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length_squared'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  Vector2 */
static int tolua_godot_godot_Vector2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Vector2",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  float p_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float p_y = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  Vector2* tolua_ret = (Vector2*)  new Vector2(p_x,p_y);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector2");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: length of class  Vector3 */
static int tolua_godot_godot_Vector3_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Vector3",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Vector3* self = (const Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'",NULL);
#endif
 {
  float tolua_ret = (float)  self->length();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}

/* method: length_squared of class  Vector3 */
static int tolua_godot_godot_Vector3_length_squared00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Vector3",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Vector3* self = (const Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length_squared'",NULL);
#endif
 {
  float tolua_ret = (float)  self->length_squared();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length_squared'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  Vector3 */
static int tolua_godot_godot_Vector3_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Vector3",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  float p_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float p_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float p_z = ((float)  tolua_tonumber(tolua_S,4,0));
 {
  Vector3* tolua_ret = (Vector3*)  new Vector3(p_x,p_y,p_z);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: get_center of class  Rect2 */
static int tolua_godot_godot_Rect2_get_center00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Rect2",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Rect2* self = (const Rect2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_center'",NULL);
#endif
 {
  Vector2 tolua_ret =  self->get_center();
 {
#ifdef __cplusplus
 void* tolua_obj = new Vector2(tolua_ret);
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,tolua_collect_Vector2),"Vector2");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector2));
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,NULL),"Vector2");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_center'.",&tolua_err);
 return 0;
#endif
}

/* method: get_area of class  Rect2 */
static int tolua_godot_godot_Rect2_get_area00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Rect2",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Rect2* self = (const Rect2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_area'",NULL);
#endif
 {
  float tolua_ret = (float)  self->get_area();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_area'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  Rect2 */
static int tolua_godot_godot_Rect2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Rect2",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  float p_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float p_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float p_width = ((float)  tolua_tonumber(tolua_S,4,0));
  float p_height = ((float)  tolua_tonumber(tolua_S,5,0));
 {
  Rect2* tolua_ret = (Rect2*)  new Rect2(p_x,p_y,p_width,p_height);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Rect2");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  Color */
static int tolua_godot_godot_Color_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Color",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  float p_r = ((float)  tolua_tonumber(tolua_S,2,0));
  float p_g = ((float)  tolua_tonumber(tolua_S,3,0));
  float p_b = ((float)  tolua_tonumber(tolua_S,4,0));
  float p_a = ((float)  tolua_tonumber(tolua_S,5,1.0));
 {
  Color* tolua_ret = (Color*)  new Color(p_r,p_g,p_b,p_a);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Color");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_line of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_line00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Point2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Point2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Color",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,6,1,&tolua_err) || 
 !tolua_isboolean(tolua_S,7,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Point2* p_from = ((const Point2*)  tolua_tousertype(tolua_S,3,0));
  const Point2* p_to = ((const Point2*)  tolua_tousertype(tolua_S,4,0));
  const Color* p_color = ((const Color*)  tolua_tousertype(tolua_S,5,0));
  float p_width = ((float)  tolua_tonumber(tolua_S,6,1.0));
  bool p_antialiased = ((bool)  tolua_toboolean(tolua_S,7,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_line'",NULL);
#endif
 {
  self->canvas_item_add_line(p_item,*p_from,*p_to,*p_color,p_width,p_antialiased);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_line'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_polyline of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_polyline00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Vector<Color>",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,1,&tolua_err) || 
 !tolua_isboolean(tolua_S,6,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Vector<Point2>* p_points = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,3,0));
  const Vector<Color>* p_colors = ((const Vector<Color>*)  tolua_tousertype(tolua_S,4,0));
  float p_width = ((float)  tolua_tonumber(tolua_S,5,1.0));
  bool p_antialiased = ((bool)  tolua_toboolean(tolua_S,6,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_polyline'",NULL);
#endif
 {
  self->canvas_item_add_polyline(p_item,*p_points,*p_colors,p_width,p_antialiased);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_polyline'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_multiline of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_multiline00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Vector<Color>",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,1,&tolua_err) || 
 !tolua_isboolean(tolua_S,6,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Vector<Point2>* p_points = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,3,0));
  const Vector<Color>* p_colors = ((const Vector<Color>*)  tolua_tousertype(tolua_S,4,0));
  float p_width = ((float)  tolua_tonumber(tolua_S,5,1.0));
  bool p_antialiased = ((bool)  tolua_toboolean(tolua_S,6,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_multiline'",NULL);
#endif
 {
  self->canvas_item_add_multiline(p_item,*p_points,*p_colors,p_width,p_antialiased);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_multiline'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_rect of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_rect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Color",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Rect2* p_rect = ((const Rect2*)  tolua_tousertype(tolua_S,3,0));
  const Color* p_color = ((const Color*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_rect'",NULL);
#endif
 {
  self->canvas_item_add_rect(p_item,*p_rect,*p_color);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_rect'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_circle of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_circle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Point2",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Color",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Point2* p_pos = ((const Point2*)  tolua_tousertype(tolua_S,3,0));
  float p_radius = ((float)  tolua_tonumber(tolua_S,4,0));
  const Color* p_color = ((const Color*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_circle'",NULL);
#endif
 {
  self->canvas_item_add_circle(p_item,*p_pos,p_radius,*p_color);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_circle'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_texture_rect of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_texture_rect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"RID",0,&tolua_err) || 
 !tolua_isboolean(tolua_S,5,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"const Color",1,&tolua_err) || 
 !tolua_isboolean(tolua_S,7,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,9,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Rect2* p_rect = ((const Rect2*)  tolua_tousertype(tolua_S,3,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,4,0));
  bool p_tile = ((bool)  tolua_toboolean(tolua_S,5,false));
  const Color* p_modulate = ((const Color*)  tolua_tousertype(tolua_S,6, new Color(1, 1, 1)));
  bool p_transpose = ((bool)  tolua_toboolean(tolua_S,7,false));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,8, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,9, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_texture_rect'",NULL);
#endif
 {
  self->canvas_item_add_texture_rect(p_item,*p_rect,p_texture,p_tile,*p_modulate,p_transpose,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_texture_rect'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_texture_rect_region of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_texture_rect_region00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"const Color",1,&tolua_err) || 
 !tolua_isboolean(tolua_S,7,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,9,"RID",1,&tolua_err) || 
 !tolua_isboolean(tolua_S,10,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Rect2* p_rect = ((const Rect2*)  tolua_tousertype(tolua_S,3,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,4,0));
  const Rect2* p_src_rect = ((const Rect2*)  tolua_tousertype(tolua_S,5,0));
  const Color* p_modulate = ((const Color*)  tolua_tousertype(tolua_S,6, new Color(1, 1, 1)));
  bool p_transpose = ((bool)  tolua_toboolean(tolua_S,7,false));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,8, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,9, new RID()));
  bool p_clip_uv = ((bool)  tolua_toboolean(tolua_S,10,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_texture_rect_region'",NULL);
#endif
 {
  self->canvas_item_add_texture_rect_region(p_item,*p_rect,p_texture,*p_src_rect,*p_modulate,p_transpose,p_normal_map,p_mask,p_clip_uv);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_texture_rect_region'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_nine_patch of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_nine_patch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Rect2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"const Vector2",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,7,"const Vector2",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,8,1,&tolua_err) || 
 !tolua_isnumber(tolua_S,9,1,&tolua_err) || 
 !tolua_isboolean(tolua_S,10,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,11,"const Color",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,12,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,13,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,14,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Rect2* p_rect = ((const Rect2*)  tolua_tousertype(tolua_S,3,0));
  const Rect2* p_source = ((const Rect2*)  tolua_tousertype(tolua_S,4,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,5,0));
  const Vector2* p_topleft = ((const Vector2*)  tolua_tousertype(tolua_S,6,0));
  const Vector2* p_bottomright = ((const Vector2*)  tolua_tousertype(tolua_S,7,0));
  NinePatchAxisMode p_x_axis_mode = ((NinePatchAxisMode) (int)  tolua_tonumber(tolua_S,8,NINE_PATCH_STRETCH));
  NinePatchAxisMode p_y_axis_mode = ((NinePatchAxisMode) (int)  tolua_tonumber(tolua_S,9,NINE_PATCH_STRETCH));
  bool p_draw_center = ((bool)  tolua_toboolean(tolua_S,10,true));
  const Color* p_modulate = ((const Color*)  tolua_tousertype(tolua_S,11, new Color(1, 1, 1)));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,12, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,13, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_nine_patch'",NULL);
#endif
 {
  self->canvas_item_add_nine_patch(p_item,*p_rect,*p_source,p_texture,*p_topleft,*p_bottomright,p_x_axis_mode,p_y_axis_mode,p_draw_center,*p_modulate,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_nine_patch'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_primitive of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_primitive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Vector<Color>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"RID",0,&tolua_err) || 
 !tolua_isnumber(tolua_S,7,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,9,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Vector<Point2>* p_points = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,3,0));
  const Vector<Color>* p_colors = ((const Vector<Color>*)  tolua_tousertype(tolua_S,4,0));
  const Vector<Point2>* p_uvs = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,5,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,6,0));
  float p_width = ((float)  tolua_tonumber(tolua_S,7,1.0));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,8, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,9, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_primitive'",NULL);
#endif
 {
  self->canvas_item_add_primitive(p_item,*p_points,*p_colors,*p_uvs,p_texture,p_width,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_primitive'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_polygon of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_polygon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Vector<Color>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Vector<Point2>",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,7,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"RID",1,&tolua_err) || 
 !tolua_isboolean(tolua_S,9,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Vector<Point2>* p_points = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,3,0));
  const Vector<Color>* p_colors = ((const Vector<Color>*)  tolua_tousertype(tolua_S,4,0));
  const Vector<Point2>* p_uvs = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,5, new Vector<Point2>()));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,6, new RID()));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,7, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,8, new RID()));
  bool p_antialiased = ((bool)  tolua_toboolean(tolua_S,9,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_polygon'",NULL);
#endif
 {
  self->canvas_item_add_polygon(p_item,*p_points,*p_colors,*p_uvs,p_texture,p_normal_map,p_mask,p_antialiased);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_polygon'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_triangle_array of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_triangle_array00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const Vector<int>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Vector<Point2>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Vector<Color>",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"const Vector<Point2>",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,7,"const Vector<int>",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"const Vector<float>",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,9,"RID",1,&tolua_err) || 
 !tolua_isnumber(tolua_S,10,1,&tolua_err) || 
 !tolua_isusertype(tolua_S,11,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,12,"RID",1,&tolua_err) || 
 !tolua_isboolean(tolua_S,13,1,&tolua_err) || 
 !tolua_isboolean(tolua_S,14,1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,15,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const Vector<int>* p_indices = ((const Vector<int>*)  tolua_tousertype(tolua_S,3,0));
  const Vector<Point2>* p_points = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,4,0));
  const Vector<Color>* p_colors = ((const Vector<Color>*)  tolua_tousertype(tolua_S,5,0));
  const Vector<Point2>* p_uvs = ((const Vector<Point2>*)  tolua_tousertype(tolua_S,6, new Vector<Point2>()));
  const Vector<int>* p_bones = ((const Vector<int>*)  tolua_tousertype(tolua_S,7, new Vector<int>()));
  const Vector<float>* p_weights = ((const Vector<float>*)  tolua_tousertype(tolua_S,8, new Vector<float>()));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,9, new RID()));
  int p_count = ((int)  tolua_tonumber(tolua_S,10,-1));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,11, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,12, new RID()));
  bool p_antialiased = ((bool)  tolua_toboolean(tolua_S,13,false));
  bool p_antialiasing_use_indices = ((bool)  tolua_toboolean(tolua_S,14,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_triangle_array'",NULL);
#endif
 {
  self->canvas_item_add_triangle_array(p_item,*p_indices,*p_points,*p_colors,*p_uvs,*p_bones,*p_weights,p_texture,p_count,p_normal_map,p_mask,p_antialiased,p_antialiasing_use_indices);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_triangle_array'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_mesh of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_mesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"const RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"const Transform2D",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"const Color",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,7,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,8,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  const RID* p_mesh = ((const RID*)  tolua_tousertype(tolua_S,3,0));
  const Transform2D* p_transform = ((const Transform2D*)  tolua_tousertype(tolua_S,4, new Transform2D()));
  const Color* p_modulate = ((const Color*)  tolua_tousertype(tolua_S,5, new Color(1, 1, 1)));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,6, new RID()));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,7, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,8, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_mesh'",NULL);
#endif
 {
  self->canvas_item_add_mesh(p_item,*p_mesh,*p_transform,*p_modulate,p_texture,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_mesh'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_multimesh of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_multimesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  RID p_mesh = *((RID*)  tolua_tousertype(tolua_S,3,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,4, new RID()));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,5, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,6, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_multimesh'",NULL);
#endif
 {
  self->canvas_item_add_multimesh(p_item,p_mesh,p_texture,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_multimesh'.",&tolua_err);
 return 0;
#endif
}

/* method: canvas_item_add_particles of class  VisualServer */
static int tolua_godot_godot_VisualServer_canvas_item_add_particles00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"VisualServer",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,2,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,3,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,4,"RID",0,&tolua_err) || 
 !tolua_isusertype(tolua_S,5,"RID",1,&tolua_err) || 
 !tolua_isusertype(tolua_S,6,"RID",1,&tolua_err) || 
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  VisualServer* self = (VisualServer*)  tolua_tousertype(tolua_S,1,0);
  RID p_item = *((RID*)  tolua_tousertype(tolua_S,2,0));
  RID p_particles = *((RID*)  tolua_tousertype(tolua_S,3,0));
  RID p_texture = *((RID*)  tolua_tousertype(tolua_S,4,0));
  RID p_normal_map = *((RID*)  tolua_tousertype(tolua_S,5, new RID()));
  RID p_mask = *((RID*)  tolua_tousertype(tolua_S,6, new RID()));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'canvas_item_add_particles'",NULL);
#endif
 {
  self->canvas_item_add_particles(p_item,p_particles,p_texture,p_normal_map,p_mask);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'canvas_item_add_particles'.",&tolua_err);
 return 0;
#endif
}

/* function: get_canvas */
static int tolua_godot_godot_get_canvas00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
 {
  RID tolua_ret =  get_canvas(L);
 {
#ifdef __cplusplus
 void* tolua_obj = new RID(tolua_ret);
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,tolua_collect_RID),"RID");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(RID));
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,NULL),"RID");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_canvas'.",&tolua_err);
 return 0;
#endif
}

/* function: get_vs */
static int tolua_godot_godot_get_vs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  VisualServer* tolua_ret = (VisualServer*)  get_vs();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"VisualServer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_vs'.",&tolua_err);
 return 0;
#endif
}

/* Open lib function */
LUALIB_API int luaopen_godot (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"godot",0);
 tolua_beginmodule(tolua_S,"godot");
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Vector2","Vector2","",tolua_collect_Vector2);
#else
 tolua_cclass(tolua_S,"Vector2","Vector2","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Vector2");
 tolua_function(tolua_S,"length",tolua_godot_godot_Vector2_length00);
 tolua_function(tolua_S,"length_squared",tolua_godot_godot_Vector2_length_squared00);
 tolua_function(tolua_S,"new",tolua_godot_godot_Vector2_new00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Vector3","Vector3","",NULL);
 tolua_beginmodule(tolua_S,"Vector3");
 tolua_function(tolua_S,"length",tolua_godot_godot_Vector3_length00);
 tolua_function(tolua_S,"length_squared",tolua_godot_godot_Vector3_length_squared00);
 tolua_function(tolua_S,"new",tolua_godot_godot_Vector3_new00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Rect2","Rect2","",NULL);
 tolua_beginmodule(tolua_S,"Rect2");
 tolua_function(tolua_S,"get_center",tolua_godot_godot_Rect2_get_center00);
 tolua_function(tolua_S,"get_area",tolua_godot_godot_Rect2_get_area00);
 tolua_function(tolua_S,"new",tolua_godot_godot_Rect2_new00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Color","Color","",NULL);
 tolua_beginmodule(tolua_S,"Color");
 tolua_function(tolua_S,"new",tolua_godot_godot_Color_new00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"NINE_PATCH_STRETCH",NINE_PATCH_STRETCH);
 tolua_constant(tolua_S,"NINE_PATCH_TILE",NINE_PATCH_TILE);
 tolua_constant(tolua_S,"NINE_PATCH_TILE_FIT",NINE_PATCH_TILE_FIT);
 tolua_cclass(tolua_S,"VisualServer","VisualServer","",NULL);
 tolua_beginmodule(tolua_S,"VisualServer");
 tolua_function(tolua_S,"canvas_item_add_line",tolua_godot_godot_VisualServer_canvas_item_add_line00);
 tolua_function(tolua_S,"canvas_item_add_polyline",tolua_godot_godot_VisualServer_canvas_item_add_polyline00);
 tolua_function(tolua_S,"canvas_item_add_multiline",tolua_godot_godot_VisualServer_canvas_item_add_multiline00);
 tolua_function(tolua_S,"canvas_item_add_rect",tolua_godot_godot_VisualServer_canvas_item_add_rect00);
 tolua_function(tolua_S,"canvas_item_add_circle",tolua_godot_godot_VisualServer_canvas_item_add_circle00);
 tolua_function(tolua_S,"canvas_item_add_texture_rect",tolua_godot_godot_VisualServer_canvas_item_add_texture_rect00);
 tolua_function(tolua_S,"canvas_item_add_texture_rect_region",tolua_godot_godot_VisualServer_canvas_item_add_texture_rect_region00);
 tolua_function(tolua_S,"canvas_item_add_nine_patch",tolua_godot_godot_VisualServer_canvas_item_add_nine_patch00);
 tolua_function(tolua_S,"canvas_item_add_primitive",tolua_godot_godot_VisualServer_canvas_item_add_primitive00);
 tolua_function(tolua_S,"canvas_item_add_polygon",tolua_godot_godot_VisualServer_canvas_item_add_polygon00);
 tolua_function(tolua_S,"canvas_item_add_triangle_array",tolua_godot_godot_VisualServer_canvas_item_add_triangle_array00);
 tolua_function(tolua_S,"canvas_item_add_mesh",tolua_godot_godot_VisualServer_canvas_item_add_mesh00);
 tolua_function(tolua_S,"canvas_item_add_multimesh",tolua_godot_godot_VisualServer_canvas_item_add_multimesh00);
 tolua_function(tolua_S,"canvas_item_add_particles",tolua_godot_godot_VisualServer_canvas_item_add_particles00);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"resources",0);
 tolua_beginmodule(tolua_S,"resources");
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"sound",0);
 tolua_beginmodule(tolua_S,"sound");
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"get_canvas",tolua_godot_godot_get_canvas00);
 tolua_function(tolua_S,"get_vs",tolua_godot_godot_get_vs00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_godot_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_godot);
 lua_pushstring(tolua_S, "godot");
 lua_call(tolua_S, 1, 0);
 return 1;
}
