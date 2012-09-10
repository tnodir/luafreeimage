/* Lua FreeImage: Bitmap information functions */


/*
 * Arguments: dib_udata
 * Returns: boolean
 */
static int
dib_hasPixels (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushboolean(L, FreeImage_HasPixels(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: image_type (string)
 */
static int
dib_getImageType (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

  return lfi_pushoption(L, type, lfi_type_values, lfi_type_names);
}

/*
 * Arguments: dib_udata
 * Returns: palette_size (number)
 */
static int
dib_getColorsUsed (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetColorsUsed(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: number
 */
static int
dib_getBPP (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetBPP(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: width in pixels (number)
 */
static int
dib_getWidth (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetWidth(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: number
 */
static int
dib_getHeight (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetHeight(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: width in bytes (number)
 */
static int
dib_getLine (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetLine(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: stride (number)
 */
static int
dib_getPitch (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetPitch(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: [palette (lightuserdata)]
 */
static int
dib_getPalette (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  void *p = FreeImage_GetPalette(dib);

  if (p) {
    lua_pushlightuserdata(L, p);
    return 1;
  }
  return 0;
}

/*
 * Arguments: dib_udata
 * Returns: horizontal_resolution (number)
 */
static int
dib_getDotsPerMeterX (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetDotsPerMeterX(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: vertical_resolution (number)
 */
static int
dib_getDotsPerMeterY (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetDotsPerMeterY(dib));
  return 1;
}

/*
 * Arguments: dib_udata, horizontal_resolution (number)
 */
static int
dib_setDotsPerMeterX (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const unsigned int res = lua_tointeger(L, 2);

  FreeImage_SetDotsPerMeterX(dib, res);
  return dib_checkerror(L, dib);
}

/*
 * Arguments: dib_udata, vertical_resolution (number)
 * Returns: [dib_udata]
 */
static int
dib_setDotsPerMeterY (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const unsigned int res = lua_tointeger(L, 2);

  FreeImage_SetDotsPerMeterY(dib, res);
  return dib_checkerror(L, dib);
}

/*
 * Arguments: dib_udata
 * Returns: string
 */
static int
dib_getColorType (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const char *s = NULL;

  switch (FreeImage_GetColorType(dib)) {
  case FIC_MINISWHITE: s = "miniswhite"; break;
  case FIC_MINISBLACK: s = "minisblack"; break;
  case FIC_PALETTE: s = "palette"; break;
  case FIC_RGB: s = "rgb"; break;
  case FIC_RGBALPHA: s = "rgba"; break;
  case FIC_CMYK: s = "cmyk"; break;
  }
  lua_pushstring(L, s);
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: red_mask (number), green_mask (number), blue_mask (number)
 */
static int
dib_getColorMask (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetRedMask(dib));
  lua_pushinteger(L, FreeImage_GetGreenMask(dib));
  lua_pushinteger(L, FreeImage_GetBlueMask(dib));
  return 3;
}

/*
 * Arguments: dib_udata
 * Returns: number
 */
static int
dib_getTransparencyCount (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushinteger(L, FreeImage_GetTransparencyCount(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: [table (lightuserdata)]
 */
static int
dib_getTransparencyTable (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  void *table = FreeImage_GetTransparencyTable(dib);

  if (table) {
    lua_pushlightuserdata(L, table);
    return 1;
  }
  return 0;
}

/*
 * Arguments: dib_udata, table (lightuserdata), count (number)
 * Returns: [dib_udata]
 */
static int
dib_setTransparencyTable (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  void *table = lua_touserdata(L, 2);
  const int count = lua_tointeger(L, 3);

  FreeImage_SetTransparencyTable(dib, table, count);
  return dib_checkerror(L, dib);
}

/*
 * Arguments: dib_udata, enabled (boolean)
 * Returns: [dib_udata]
 */
static int
dib_setTransparency (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const int enabled = lua_toboolean(L, 2);

  FreeImage_SetTransparent(dib, enabled);
  return dib_checkerror(L, dib);
}

/*
 * Arguments: dib_udata
 * Returns: boolean
 */
static int
dib_isTransparent (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushboolean(L, FreeImage_IsTransparent(dib));
  return 1;
}

/*
 * Arguments: dib_udata, index (number)
 * Returns: [dib_udata]
 */
static int
dib_setTransparentIndex (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const int i = lua_tointeger(L, 2);

  FreeImage_SetTransparentIndex(dib, i);
  return dib_checkerror(L, dib);
}

/*
 * Arguments: dib_udata
 * Returns: [number]
 */
static int
dib_getTransparentIndex (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  const int i = FreeImage_GetTransparentIndex(dib);

  if (i != -1) {
    lua_pushinteger(L, i);
    return 1;
  }
  return 0;
}

/*
 * Arguments: dib_udata
 * Returns: boolean
 */
static int
dib_hasBackgroundColor (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  lua_pushboolean(L, FreeImage_HasBackgroundColor(dib));
  return 1;
}

/*
 * Arguments: dib_udata
 * Returns: [Color]
 */
static int
dib_getBackgroundColor (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  RGBQUAD col;

  if (FreeImage_GetBackgroundColor(dib, &col)) {
    return lfi_pushcolor(L, &col);
  }
  return 0;
}

/*
 * Arguments: dib_udata, [Color]
 * Returns: [dib_udata]
 */
static int
dib_setBackgroundColor (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  RGBQUAD col, *colp;

  colp = (lua_gettop(L) > 1) ? lfi_tocolor(L, 2, &col), &col : NULL;

  return dib_checkerror(L,
   FreeImage_SetBackgroundColor(dib, colp) ? dib : NULL);
}

/*
 * Arguments: dest. dib_udata, source dib_udata
 * Returns: [dest. dib_udata]
 */
static int
dib_getThumbnail (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);

  *dibp = FreeImage_GetThumbnail(dib);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata, [thumbnail (dib_udata)]
 * Returns: [dib_udata]
 */
static int
dib_setThumbnail (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  FIBITMAP *thumbnail = lua_isuserdata(L, 2)
   ? lua_unboxpointer(L, 2, DIB_TYPENAME) : NULL;

  return dib_checkerror(L,
   FreeImage_SetThumbnail(dib, thumbnail) ? dib : NULL);
}


#define DIB_INFO_METHODS \
  {"hasPixels",			dib_hasPixels}, \
  {"getImageType",		dib_getImageType}, \
  {"getColorsUsed",		dib_getColorsUsed}, \
  {"getBPP",			dib_getBPP}, \
  {"getWidth",			dib_getWidth}, \
  {"getHeight",			dib_getHeight}, \
  {"getLine",			dib_getLine}, \
  {"getPitch",			dib_getPitch}, \
  {"getPalette",		dib_getPalette}, \
  {"getDotsPerMeterX",		dib_getDotsPerMeterX}, \
  {"getDotsPerMeterY",		dib_getDotsPerMeterY}, \
  {"setDotsPerMeterX",		dib_setDotsPerMeterX}, \
  {"setDotsPerMeterY",		dib_setDotsPerMeterY}, \
  {"getColorType",		dib_getColorType}, \
  {"getColorMask",		dib_getColorMask}, \
  {"getTransparencyCount",	dib_getTransparencyCount}, \
  {"getTransparencyTable",	dib_getTransparencyTable}, \
  {"setTransparencyTable",	dib_setTransparencyTable}, \
  {"setTransparency",		dib_setTransparency}, \
  {"isTransparent",		dib_isTransparent}, \
  {"setTransparentIndex",	dib_setTransparentIndex}, \
  {"getTransparentIndex",	dib_getTransparentIndex}, \
  {"hasBackgroundColor",	dib_hasBackgroundColor}, \
  {"getBackgroundColor",	dib_getBackgroundColor}, \
  {"setBackgroundColor",	dib_setBackgroundColor}, \
  {"getThumbnail",		dib_getThumbnail}, \
  {"setThumbnail",		dib_setThumbnail}
