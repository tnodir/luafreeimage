/* Lua FreeImage: Bitmap management functions */

#define DIB_TYPENAME	"freeimage.bitmap"


/*
 * Returns: dib_udata
 */
static int
dib_new (lua_State *L)
{
    lua_boxpointer(L, NULL);
    luaL_getmetatable(L, DIB_TYPENAME);
    lua_setmetatable(L, -2);
    return 1;
}

/*
 * Argumants: dib_udata, ...
 * Returns: [dib_udata]
 */
static int
dib_checkerror (lua_State *L, FIBITMAP *dib)
{
    if (dib) {
	lua_settop(L, 1);
	return 1;
    }
    return lfi_seterror(L);
}

/*
 * Arguments: dib_udata, [image_type (string)],
 *	width (number), height (number), bpp (number),
 *	[red_mask (number), green_mask (number), blue_mask (number)]
 * Returns: [dib_udata]
 */
static int
dib_allocate (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    const int arg_offset = (lua_type(L, 1) == LUA_TNUMBER) ? 0 : 1;
    const FREE_IMAGE_TYPE type = lfi_gettype(L, 2);
    const int w = lua_tointeger(L, arg_offset + 2);
    const int h = lua_tointeger(L, arg_offset + 3);
    const int bpp = luaL_optint(L, arg_offset + 4, 8);
    const unsigned int rmask = lua_tointeger(L, arg_offset + 5);
    const unsigned int gmask = lua_tointeger(L, arg_offset + 6);
    const unsigned int bmask = lua_tointeger(L, arg_offset + 7);

    *dibp = FreeImage_AllocateT(type, w, h, bpp, rmask, gmask, bmask);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: ..., options (string) ...
 */
static int
dib_getbkoptions (lua_State *L, int idx)
{
    static const FREE_IMAGE_FORMAT flag_values[] = {
	FI_COLOR_IS_RGB_COLOR, FI_COLOR_IS_RGBA_COLOR, FI_COLOR_FIND_EQUAL_COLOR,
	FI_COLOR_ALPHA_IS_INDEX, FI_COLOR_PALETTE_SEARCH_MASK
    };
    static const char *const flag_names[] = {
	"is_rgb_color", "is_rgba_color", "find_equal_color",
	"alpha_is_index", "palette_search_mask",
	NULL
    };
    const int top = lua_gettop(L);
    int flags = 0;

    for (; idx <= top; ++idx) {
	flags |= flag_values[luaL_checkoption(L, idx, NULL, flag_names)];
    }
    return flags;
}

/*
 * Arguments: dib_udata, [image_type (string)],
 *	width (number), height (number), bpp (number),
 *	Color, [palette (lightuserdata),
 *	red_mask (number), green_mask (number), blue_mask (number),
 *	options (string) ...]
 * Returns: [dib_udata]
 */
static int
dib_allocateEx (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    const int arg_offset = (lua_type(L, 1) == LUA_TNUMBER) ? 0 : 1;
    const FREE_IMAGE_TYPE type = lfi_gettype(L, 2);
    const int w = lua_tointeger(L, arg_offset + 2);
    const int h = lua_tointeger(L, arg_offset + 3);
    const int bpp = luaL_optint(L, arg_offset + 4, 8);
    RGBQUAD col;
    const RGBQUAD *palp = lua_touserdata(L, arg_offset + 6);
    const unsigned int rmask = lua_tointeger(L, arg_offset + 7);
    const unsigned int gmask = lua_tointeger(L, arg_offset + 8);
    const unsigned int bmask = lua_tointeger(L, arg_offset + 9);
    const int flags = dib_getbkoptions(L, arg_offset + 10);

    lfi_tocolor(L, arg_offset + 5, &col);

    *dibp = FreeImage_AllocateExT(type, w, h, bpp, &col, flags, palp, rmask, gmask, bmask);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata, Color, [options (string) ...]
 * Returns: [dib_udata]
 */
static int
dib_fillBackground (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    RGBQUAD col;
    const int flags = dib_getbkoptions(L, 3);

    lfi_tocolor(L, 2, &col);

    return dib_checkerror(L,
     FreeImage_FillBackground(dib, &col, flags) ? dib : NULL);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	left (number), top (number), right (number), bottom (number),
 *	Color, [options (string) ...]
 * Returns: [dest. dib_udata]
 */
static int
dib_enlargeCanvas (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
    const int left = lua_tointeger(L, 3);
    const int top = lua_tointeger(L, 4);
    const int right = lua_tointeger(L, 5);
    const int bottom = lua_tointeger(L, 6);
    RGBQUAD col;
    const int flags = dib_getbkoptions(L, 8);

    lfi_tocolor(L, 7, &col);

    *dibp = FreeImage_EnlargeCanvas(dib, left, top, right, bottom, &col, flags);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: ..., format_options (string | number) ...
 */
static int
dib_getformatflags (lua_State *L, int idx)
{
    static const FREE_IMAGE_FORMAT flag_values[] = {
	BMP_SAVE_RLE,
	GIF_LOAD256, GIF_PLAYBACK, ICO_MAKEALPHA,
	JPEG_FAST, JPEG_ACCURATE, JPEG_CMYK, JPEG_EXIFROTATE,
	JPEG_QUALITYSUPERB, JPEG_QUALITYGOOD, JPEG_QUALITYNORMAL,
	JPEG_QUALITYAVERAGE, JPEG_QUALITYBAD, JPEG_PROGRESSIVE,
	TARGA_LOAD_RGB888,
#ifdef FIF_EXR
	EXR_FLOAT, EXR_NONE, EXR_ZIP, EXR_PIZ,
	EXR_PXR24, EXR_B44, EXR_LC,
	PCD_BASE, PCD_BASEDIV4, PCD_BASEDIV16,
	PNG_IGNOREGAMMA, PNM_SAVE_ASCII,
	TIFF_CMYK, TIFF_PACKBITS, TIFF_DEFLATE, TIFF_ADOBE_DEFLATE,
	TIFF_NONE, TIFF_CCITTFAX3, TIFF_CCITTFAX4, TIFF_LZW, TIFF_JPEG,
#endif
	0
    };
    static const char *const flag_names[] = {
	"bmp_save_rle",
	"gif_load256", "gif_playback", "ico_makealpha",
	"jpeg_fast", "jpeg_accurate", "jpeg_cmyk", "jpeg_exifrotate",
	"jpeg_qualitysuperb", "jpeg_qualitygood", "jpeg_qualitynormal",
	"jpeg_qualityaverage", "jpeg_qualitybad", "jpeg_progressive",
	"targa_load_rgb888",
#ifdef FIF_EXR
	"exr_float", "exr_none", "exr_zip", "exr_piz",
	"exr_pxr24", "exr_b44", "exr_lc",
	"pcd_base", "pcd_basediv4", "pcd_basediv16",
	"png_ignoregamma", "pnm_save_ascii",
	"tiff_cmyk", "tiff_packbits", "tiff_deflate", "tiff_adobe_deflate",
	"tiff_none", "tiff_ccittfax3", "tiff_ccittfax4", "tiff_lzw", "tiff_jpeg",
#endif
	"default", NULL
    };
    const int top = lua_gettop(L);
    int flags = 0;

    for (; idx <= top; ++idx) {
	flags |= (lua_type(L, idx) == LUA_TNUMBER)
	 ? lua_tointeger(L, idx)
	 : flag_values[luaL_checkoption(L, idx, NULL, flag_names)];
    }
    return flags;
}

/*
 * Arguments: dib_udata, image_format (string), path (string),
 *	[format_options (string) ...]
 * Returns: [dib_udata]
 */
static int
dib_load (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    const FREE_IMAGE_FORMAT fif = lfi_getformat(L, 2);
    const char *path = luaL_checkstring(L, 3);
    const int flags = dib_getformatflags(L, 4);

    *dibp = FreeImage_Load(fif, path, flags);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata, image_format (string), path (string),
 *	[format_options (string) ...]
 * Returns: [dib_udata]
 */
static int
dib_save (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const FREE_IMAGE_FORMAT fif = lfi_getformat(L, 2);
    const char *path = luaL_checkstring(L, 3);
    const int flags = dib_getformatflags(L, 4);

    return dib_checkerror(L,
     FreeImage_Save(fif, dib, path, flags) ? dib : NULL);
}

/*
 * Arguments: dest. dib_udata, source dib_udata
 * Returns: [dest. dib_udata]
 */
static int
dib_clone (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);

    *dibp = FreeImage_Clone(dib);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata
 */
static int
dib_close (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);

    if (*dibp) {
	FreeImage_Unload(*dibp);
	*dibp = NULL;
    }
    return 0;
}


#define DIB_METHODS \
    {"allocate",	dib_allocate}, \
    {"allocateEx",	dib_allocateEx}, \
    {"fillBackground",	dib_fillBackground}, \
    {"enlargeCanvas",	dib_enlargeCanvas}, \
    {"load",		dib_load}, \
    {"save",		dib_save}, \
    {"clone",		dib_clone}, \
    {"close",		dib_close}, \
    {"__gc",		dib_close}
