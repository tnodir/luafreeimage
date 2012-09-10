/* Lua FreeImage: Copy / Paste / Composite routines */


/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	left (number), top (number), right (number), bottom (number)
 * Returns: [dest. dib_udata]
 */
static int
dib_copy (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const int left = lua_tointeger(L, 3);
  const int top = lua_tointeger(L, 4);
  const int right = lua_tointeger(L, 5);
  const int bottom = lua_tointeger(L, 6);

  *dibp = FreeImage_Copy(dib, left, top, right, bottom);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	left (number), top (number), alpha (number)
 * Returns: [dest. dib_udata]
 */
static int
dib_paste (lua_State *L)
{
  FIBITMAP *dst_dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
  FIBITMAP *src_dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const int left = lua_tointeger(L, 3);
  const int top = lua_tointeger(L, 4);
  const int alpha = lua_tointeger(L, 5);

  return dib_checkerror(L,
   FreeImage_Paste(dst_dib, src_dib, left, top, alpha) ? dst_dib : NULL);
}

/*
 * Arguments: dest. dib_udata, foreground dib_udata,
 *	[use_file_background (boolean), app_background_color (Color),
 *	background dib_udata]
 * Returns: [dest. dib_udata]
 */
static int
dib_composite (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *fg_dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const BOOL use_file_bkg = lua_toboolean(L, 3);
  RGBQUAD app_bkcolor;
  const int i = lua_isnoneornil(L, 4)
   ? 0 : (lfi_tocolor(L, 4, &app_bkcolor), 1);
  FIBITMAP *bg_dib = lua_isuserdata(L, 4 + i)
   ? lua_unboxpointer(L, 4 + i, DIB_TYPENAME) : NULL;

  *dibp = FreeImage_Composite(fg_dib, use_file_bkg,
   i ? &app_bkcolor : NULL, bg_dib);
  return dib_checkerror(L, *dibp);
}


#define DIB_COPY_METHODS \
  {"copy",		dib_copy}, \
  {"paste",		dib_paste}, \
  {"composite",		dib_composite}
