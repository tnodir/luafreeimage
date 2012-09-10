/* Lua FreeImage */

#include "common.h"

#include "lfi_util.c"


static const char *g_ErrorMessage;

static void
lfi_errorhandler(FREE_IMAGE_FORMAT fif, const char *msg) {
  (void) fif;
  g_ErrorMessage = msg; 
}

/*
 * Returns: nil, string
 */
static int
lfi_seterror (lua_State *L)
{
  lua_pushnil(L);
  lua_pushstring(L, g_ErrorMessage);
  lua_pushvalue(L, -1);
  lua_setglobal(L, "errorMessage");
  return 2;
}

/*
 * Returns: version (string), copyright (string)
 */
static int
lfi_getInfo (lua_State *L)
{
  lua_pushstring(L, FreeImage_GetVersion());
  lua_pushstring(L, FreeImage_GetCopyrightMessage());
  return 2;
}

/*
 * Arguments: path (string)
 * Returns: image_format (string)
 */
static int
lfi_getFileType (lua_State *L)
{
  const char *path = luaL_checkstring(L, 1);

  return lfi_pushoption(L, FreeImage_GetFileType(path, 0),
   lfi_format_values, lfi_format_names);
}

/*
 * Arguments: color (string)
 * Returns: color (number: 0xAABBGGRR)
 */
static int
lfi_getColor (lua_State *L)
{
  RGBQUAD col;

  lfi_tocolor(L, 1, &col);
  return lfi_pushcolor(L, &col);
}


static int
lfi_uninit (lua_State *L)
{
  (void) L;
  FreeImage_DeInitialise();
  return 0;
}


#include "lfi_plug.c"

#include "dib.c"
#include "dib_info.c"
#include "dib_pix.c"
#include "dib_conv.c"
#include "dib_rot.c"
#include "dib_samp.c"
#include "dib_col.c"
#include "dib_chan.c"
#include "dib_copy.c"

static luaL_Reg dib_meth[] = {
  DIB_METHODS,
  DIB_INFO_METHODS,
  DIB_PIX_METHODS,
  DIB_CONV_METHODS,
  DIB_ROT_METHODS,
  DIB_SAMP_METHODS,
  DIB_COL_METHODS,
  DIB_CHAN_METHODS,
  DIB_COPY_METHODS,
  {NULL, NULL}
};

static luaL_Reg filib[] = {
  {"getInfo",		lfi_getInfo},
  {"getFileType",	lfi_getFileType},
  {"getColor",	lfi_getColor},
  {"bitmap",		dib_new},
  LFI_PLUG_METHODS,
  {NULL, NULL}
};


LUALIB_API int luaopen_freeimage (lua_State *L);

LUALIB_API int
luaopen_freeimage (lua_State *L)
{
  FreeImage_Initialise(0);
  FreeImage_SetOutputMessage(lfi_errorhandler);

  luaL_register(L, "freeimage", filib);

  /* library finalizer */
  lua_newuserdata(L, 0);
  lua_newtable(L);  /* metatable */
  lua_pushvalue(L, -1);
  lua_pushliteral(L, "__gc");
  lua_pushcfunction(L, lfi_uninit);
  lua_rawset(L, -3);
  lua_setmetatable(L, -3);
  lua_rawset(L, -3);

  luaL_newmetatable(L, DIB_TYPENAME);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  luaL_register(L, NULL, dib_meth);
  lua_pop(L, 1);
  return 1;
}
