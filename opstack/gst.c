#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <lua.h>
#include <lauxlib.h>

#define ANY  (void*)


static int checkbool(lua_State* L, int index);
static void* checkpointer(lua_State* L, int index);
static void pushpointer(lua_State* L, void* p);

static void init(lua_State* L);


static int l_list(lua_State* L);



int luaopen_gst(lua_State* L)
{
	init(L);

#define FUNC(NAME) { #NAME, &l_##NAME }
	static const luaL_Reg lib[] = {
		FUNC(list),
		{ NULL, NULL },
	};
#undef FUNC
	luaL_newlib(L, lib);

	return 1;
}




static int checkbool(lua_State* L, int index)
{
	if (lua_isnil(L, index)) return 0;
	if (!lua_isboolean(L, index)) luaL_error(L, "boolean expected");
	return lua_toboolean(L, index);
}

static void* checkpointer(lua_State* L, int index)
{
	if (lua_isnil(L, index)) return NULL;
	if (!lua_isuserdata(L, index)) luaL_error(L, "userdata expected");
	return lua_touserdata(L, index);
}

static void pushpointer(lua_State* L, void* p)
{
	if (p) lua_pushlightuserdata(L, p);
	else lua_pushnil(L);
}




static void init(lua_State* L)
{
	gst_init(NULL, NULL);
/*
	GList* elems = gst_element_factory_list_get_elements(
			GST_ELEMENT_FACTORY_TYPE_ANY, 0);
	{
		void $(GstElementFactory* factory)
		{
			printf("%-20.20s [%-19.19s] %-37.37s\n",
				gst_plugin_feature_get_name(ANY factory),
				gst_element_factory_get_metadata(factory, "klass"),
				gst_element_factory_get_metadata(factory, "long-name"));
		}
		g_list_foreach(elems, ANY $, NULL);
	}
	gst_plugin_feature_list_free(elems);
*/
}




static int l_list(lua_State* L)
{
	lua_newtable(L);

	GList* elems = gst_element_factory_list_get_elements(
			GST_ELEMENT_FACTORY_TYPE_ANY, 0);
	{
		void pushinfo(GstElementFactory* factory)
		{
			lua_createtable(L, 0, 3);
			lua_pushstring(L,
				gst_element_factory_get_metadata(factory, "klass"));
			lua_setfield(L, -2, "category");
			lua_pushstring(L,
				gst_element_factory_get_metadata(factory, "long-name"));
			lua_setfield(L, -2, "longname");
			lua_pushstring(L,
				gst_element_factory_get_metadata(factory, "description"));
			lua_setfield(L, -2, "description");

			lua_setfield(L, -2, gst_plugin_feature_get_name(ANY factory));
		}
		g_list_foreach(elems, ANY pushinfo, NULL);
	}
	gst_plugin_feature_list_free(elems);

	return 1;
}

