/* Lua FreeImage: Rotation and flipping */


/*
 * Arguments: dest. dib_udata, source dib_udata, angle (number),
 *	[bkcolor (Color)]
 * Returns: [dest. dib_udata]
 */
static int
dib_rotate (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const double angle = (double) lua_tonumber(L, 3);
  RGBQUAD bkcolor, *bkcolorp = NULL;

  if (!lua_isnoneornil(L, 4)) {
    lfi_tocolor(L, 3, &bkcolor);
    bkcolorp = &bkcolor;
  }

  *dibp = FreeImage_Rotate(dib, angle, bkcolorp);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata, angle (number),
 *	x_shift (number), y_shift (number),
 *	x_origin (number), y_origin (number), use_mask (boolean)
 * Returns: [dest. dib_udata]
 */
static int
dib_rotateEx (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const double angle = (double) lua_tonumber(L, 3);
  const double x_shift = (double) lua_tonumber(L, 4);
  const double y_shift = (double) lua_tonumber(L, 5);
  const double x_origin = (double) lua_tonumber(L, 6);
  const double y_origin = (double) lua_tonumber(L, 7);
  const BOOL use_mask = lua_isnoneornil(L, 8) || lua_toboolean(L, 8);

  *dibp = FreeImage_RotateEx(dib, angle, x_shift, y_shift,
   x_origin, y_origin, use_mask);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata
 * Returns: [dib_udata]
 */
static int
dib_flipHorizontal (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  return dib_checkerror(L,
   FreeImage_FlipHorizontal(dib) ? dib : NULL);
}

/*
 * Arguments: dib_udata
 * Returns: [dib_udata]
 */
static int
dib_flipVertical (lua_State *L)
{
  FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

  return dib_checkerror(L,
   FreeImage_FlipVertical(dib) ? dib : NULL);
}


#define DIB_ROT_METHODS \
  {"rotate",			dib_rotate}, \
  {"rotateEx",			dib_rotateEx}, \
  {"flipHorizontal",		dib_flipHorizontal}, \
  {"flipVertical",		dib_flipVertical}
