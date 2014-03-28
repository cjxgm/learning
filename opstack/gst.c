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


static int l_elements(lua_State* L);
static int l_element(lua_State* L);
static int l_set(lua_State* L);
static int l_add(lua_State* L);
static int l_link(lua_State* L);
static int l_play(lua_State* L);
static int l_stop(lua_State* L);



int luaopen_gst(lua_State* L)
{
	init(L);

#define FUNC(NAME) { #NAME, &l_##NAME }
	static const luaL_Reg lib[] = {
		FUNC(elements),
		FUNC(element),
		FUNC(set),
		FUNC(add),
		FUNC(link),
		FUNC(play),
		FUNC(stop),
		//FUNC(unlink),
		//FUNC(state),
		//FUNC(states),
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
}




static int l_elements(lua_State* L)
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

static int l_element(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	GstElement* elem = gst_element_factory_make(name, NULL);
	pushpointer(L, elem);
	return 1;
}

static int l_set(lua_State* L)
{
	/*
	GstElement* elem = checkpointer(L, 1);
	const char* k = luaL_checkstring(L, 2);
	switch (lua_type(L, 3)) {
	}
	*/
	return 0;
}

static int l_add(lua_State* L)
{
	GstBin* bin = checkpointer(L, 1);
	GstElement* elem = checkpointer(L, 2);
	gst_bin_add(bin, elem);
	return 0;
}

static int l_link(lua_State* L)
{
	GstElement* src = checkpointer(L, 1);
	GstElement* sink= checkpointer(L, 2);
	gst_element_link(src, sink);
	return 0;
}

static int l_play(lua_State* L)
{
	GstElement* elem= checkpointer(L, 1);
	gst_element_set_state(elem, GST_STATE_PLAYING);
	return 0;
}

static int l_stop(lua_State* L)
{
	GstElement* elem= checkpointer(L, 1);
	gst_element_set_state(elem, GST_STATE_NULL);
	return 0;
}

