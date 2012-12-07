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

  *dibp = FreeImage_AllocateExT(type, w, h, bpp, &col, flags, palp,
   rmask, gmask, bmask);
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

  *dibp = FreeImage_EnlargeCanvas(dib, left, top, right, bottom,
   &col, flags);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: ..., format_options (string | number) ...
 */
static int
dib_getformatflags (lua_State *L, int idx)
{
  static const FREE_IMAGE_FORMAT flag_values[] = {
    0,
    /* FIF_BMP */
#ifdef BMP_SAVE_RLE
    BMP_SAVE_RLE,
#endif
    /* FIF_ICO */
#ifdef ICO_MAKEALPHA
    ICO_MAKEALPHA,
#endif
    /* FIF_JPEG */
#ifdef JPEG_FAST
    JPEG_FAST,
#endif
#ifdef JPEG_ACCURATE
    JPEG_ACCURATE,
#endif
#ifdef JPEG_CMYK
    JPEG_CMYK,
#endif
#ifdef JPEG_EXIFROTATE
    JPEG_EXIFROTATE,
#endif
#ifdef JPEG_GREYSCALE
    JPEG_GREYSCALE,
#endif
#ifdef JPEG_QUALITYSUPERB
    JPEG_QUALITYSUPERB,
#endif
#ifdef JPEG_QUALITYGOOD
    JPEG_QUALITYGOOD,
#endif
#ifdef JPEG_QUALITYNORMAL
    JPEG_QUALITYNORMAL,
#endif
#ifdef JPEG_QUALITYAVERAGE
    JPEG_QUALITYAVERAGE,
#endif
#ifdef JPEG_QUALITYBAD
    JPEG_QUALITYBAD,
#endif
#ifdef JPEG_PROGRESSIVE
    JPEG_PROGRESSIVE,
#endif
#ifdef JPEG_SUBSAMPLING_411
    JPEG_SUBSAMPLING_411,
#endif
#ifdef JPEG_SUBSAMPLING_420
    JPEG_SUBSAMPLING_420,
#endif
#ifdef JPEG_SUBSAMPLING_422
    JPEG_SUBSAMPLING_422,
#endif
#ifdef JPEG_SUBSAMPLING_444
    JPEG_SUBSAMPLING_444,
#endif
#ifdef JPEG_OPTIMIZE
    JPEG_OPTIMIZE,
#endif
#ifdef JPEG_BASELINE
    JPEG_BASELINE,
#endif
    /* FIF_JNG */
    /* FIF_KOALA */
    /* FIF_LBM */
    /* FIF_IFF */
    /* FIF_MNG */
    /* FIF_PBM, FIF_PGM, FIF_PPM */
#ifdef PNM_SAVE_RAW
    PNM_SAVE_RAW,
#endif
#ifdef PNM_SAVE_ASCII
    PNM_SAVE_ASCII,
#endif
    /* FIF_PBMRAW */
    /* FIF_PCD */
#ifdef PCD_BASE
    PCD_BASE,
#endif
#ifdef PCD_BASEDIV4
    PCD_BASEDIV4,
#endif
#ifdef PCD_BASEDIV4
    PCD_BASEDIV16,
#endif
    /* FIF_PCX */
    /* FIF_PGMRAW */
    /* FIF_PNG */
#ifdef PNG_IGNOREGAMMA
    PNG_IGNOREGAMMA,
#endif
#ifdef PNG_Z_BEST_SPEED
    PNG_Z_BEST_SPEED,
#endif
#ifdef PNG_Z_DEFAULT_COMPRESSION
    PNG_Z_DEFAULT_COMPRESSION,
#endif
#ifdef PNG_Z_BEST_COMPRESSION
    PNG_Z_BEST_COMPRESSION,
#endif
#ifdef PNG_Z_NO_COMPRESSION
    PNG_Z_NO_COMPRESSION,
#endif
#ifdef PNG_INTERLACED
    PNG_INTERLACED,
#endif
    /* FIF_PPMRAW */
    /* FIF_RAS */
    /* FIF_TARGA */
#ifdef TARGA_LOAD_RGB888
    TARGA_LOAD_RGB888,
#endif
#ifdef TARGA_SAVE_RLE
    TARGA_SAVE_RLE,
#endif
    /* FIF_TIFF */
#ifdef TIFF_CMYK
    TIFF_CMYK,
#endif
#ifdef TIFF_PACKBITS
    TIFF_PACKBITS,
#endif
#ifdef TIFF_DEFLATE
    TIFF_DEFLATE,
#endif
#ifdef TIFF_ADOBE_DEFLATE
    TIFF_ADOBE_DEFLATE,
#endif
#ifdef TIFF_NONE
    TIFF_NONE,
#endif
#ifdef TIFF_CCITTFAX3
    TIFF_CCITTFAX3,
#endif
#ifdef TIFF_CCITTFAX4
    TIFF_CCITTFAX4,
#endif
#ifdef TIFF_LZW
    TIFF_LZW,
#endif
#ifdef TIFF_JPEG
    TIFF_JPEG,
#endif
#ifdef TIFF_LOGLUV
    TIFF_LOGLUV,
#endif
    /* FIF_WBMP */
    /* FIF_PSD */
#ifdef PSD_CMYK
    PSD_CMYK,
#endif
#ifdef PSD_LAB
    PSD_LAB,
#endif
    /* FIF_CUT */
    /* FIF_XBM */
    /* FIF_XPM */
    /* FIF_DDS */
    /* FIF_GIF */
#ifdef GIF_LOAD256
    GIF_LOAD256,
#endif
#ifdef GIF_PLAYBACK
    GIF_PLAYBACK,
#endif
    /* FIF_HDR */
    /* FIF_FAXG3 */
    /* FIF_SGI */
    /* FIF_EXR */
#ifdef EXR_FLOAT
    EXR_FLOAT,
#endif
#ifdef EXR_NONE
    EXR_NONE,
#endif
#ifdef EXR_ZIP
    EXR_ZIP,
#endif
#ifdef EXR_PIZ
    EXR_PIZ,
#endif
#ifdef EXR_PXR24
    EXR_PXR24,
#endif
#ifdef EXR_B44
    EXR_B44,
#endif
#ifdef EXR_LC
    EXR_LC,
#endif
    /* FIF_J2K */
    /* FIF_JP2 */
    /* FIF_PFM */
    /* FIF_PICT */
    /* FIF_RAW */
#ifdef RAW_PREVIEW
    RAW_PREVIEW,
#endif
#ifdef RAW_DISPLAY
    RAW_DISPLAY,
#endif
#ifdef RAW_HALFSIZE
    RAW_HALFSIZE,
#endif
    FIF_LOAD_NOPIXELS
  };
  static const char *const flag_names[] = {
    "default",
    /* FIF_BMP */
#ifdef BMP_SAVE_RLE
    "bmp_save_rle",
#endif
    /* FIF_ICO */
#ifdef ICO_MAKEALPHA
    "ico_makealpha",
#endif
    /* FIF_JPEG */
#ifdef JPEG_FAST
    "jpeg_fast",
#endif
#ifdef JPEG_ACCURATE
    "jpeg_accurate",
#endif
#ifdef JPEG_CMYK
    "jpeg_cmyk",
#endif
#ifdef JPEG_EXIFROTATE
    "jpeg_exifrotate",
#endif
#ifdef JPEG_GREYSCALE
    "jpeg_greyscale",
#endif
#ifdef JPEG_QUALITYSUPERB
    "jpeg_qualitysuperb",
#endif
#ifdef JPEG_QUALITYGOOD
    "jpeg_qualitygood",
#endif
#ifdef JPEG_QUALITYNORMAL
    "jpeg_qualitynormal",
#endif
#ifdef JPEG_QUALITYAVERAGE
    "jpeg_qualityaverage",
#endif
#ifdef JPEG_QUALITYBAD
    "jpeg_qualitybad",
#endif
#ifdef JPEG_PROGRESSIVE
    "jpeg_progressive",
#endif
#ifdef JPEG_SUBSAMPLING_411
    "jpeg_subsampling_411",
#endif
#ifdef JPEG_SUBSAMPLING_420
    "jpeg_subsampling_420",
#endif
#ifdef JPEG_SUBSAMPLING_422
    "jpeg_subsampling_422",
#endif
#ifdef JPEG_SUBSAMPLING_444
    "jpeg_subsampling_444",
#endif
#ifdef JPEG_OPTIMIZE
    "jpeg_optimize",
#endif
#ifdef JPEG_BASELINE
    "jpeg_baseline",
#endif
    /* FIF_JNG */
    /* FIF_KOALA */
    /* FIF_LBM */
    /* FIF_IFF */
    /* FIF_MNG */
    /* FIF_PBM, FIF_PGM, FIF_PPM */
#ifdef PNM_SAVE_RAW
    "pnm_save_raw",
#endif
#ifdef PNM_SAVE_ASCII
    "pnm_save_ascii",
#endif
    /* FIF_PBMRAW */
    /* FIF_PCD */
#ifdef PCD_BASE
    "pcd_base",
#endif
#ifdef PCD_BASEDIV4
    "pcd_basediv4",
#endif
#ifdef PCD_BASEDIV4
    "pcd_basediv16",
#endif
    /* FIF_PCX */
    /* FIF_PGMRAW */
    /* FIF_PNG */
#ifdef PNG_IGNOREGAMMA
    "png_ignoregamma",
#endif
#ifdef PNG_Z_BEST_SPEED
    "png_z_best_speed",
#endif
#ifdef PNG_Z_DEFAULT_COMPRESSION
    "png_z_default_compression",
#endif
#ifdef PNG_Z_BEST_COMPRESSION
    "png_z_best_compression",
#endif
#ifdef PNG_Z_NO_COMPRESSION
    "png_z_no_compression",
#endif
#ifdef PNG_INTERLACED
    "png_interlaced",
#endif
    /* FIF_PPMRAW */
    /* FIF_RAS */
    /* FIF_TARGA */
#ifdef TARGA_LOAD_RGB888
    "targa_load_rgb888",
#endif
#ifdef TARGA_SAVE_RLE
    "targa_save_rle",
#endif
    /* FIF_TIFF */
#ifdef TIFF_CMYK
    "tiff_cmyk",
#endif
#ifdef TIFF_PACKBITS
    "tiff_packbits",
#endif
#ifdef TIFF_DEFLATE
    "tiff_deflate",
#endif
#ifdef TIFF_ADOBE_DEFLATE
    "tiff_adobe_deflate",
#endif
#ifdef TIFF_NONE
    "tiff_none",
#endif
#ifdef TIFF_CCITTFAX3
    "tiff_ccittfax3",
#endif
#ifdef TIFF_CCITTFAX4
    "tiff_ccittfax4",
#endif
#ifdef TIFF_LZW
    "tiff_lzw",
#endif
#ifdef TIFF_JPEG
    "tiff_jpeg",
#endif
#ifdef TIFF_LOGLUV
    "tiff_logluv",
#endif
    /* FIF_WBMP */
    /* FIF_PSD */
#ifdef PSD_CMYK
    "psd_cmyk",
#endif
#ifdef PSD_LAB
    "psd_lab",
#endif
    /* FIF_CUT */
    /* FIF_XBM */
    /* FIF_XPM */
    /* FIF_DDS */
    /* FIF_GIF */
#ifdef GIF_LOAD256
    "gif_load256",
#endif
#ifdef GIF_PLAYBACK
    "gif_playback",
#endif
    /* FIF_HDR */
    /* FIF_FAXG3 */
    /* FIF_SGI */
    /* FIF_EXR */
#ifdef EXR_FLOAT
    "exr_float",
#endif
#ifdef EXR_NONE
    "exr_none",
#endif
#ifdef EXR_ZIP
    "exr_zip",
#endif
#ifdef EXR_PIZ
    "exr_piz",
#endif
#ifdef EXR_PXR24
    "exr_pxr24",
#endif
#ifdef EXR_B44
    "exr_b44",
#endif
#ifdef EXR_LC
    "exr_lc",
#endif
    /* FIF_J2K */
    /* FIF_JP2 */
    /* FIF_PFM */
    /* FIF_PICT */
    /* FIF_RAW */
#ifdef RAW_PREVIEW
    "raw_preview",
#endif
#ifdef RAW_DISPLAY
    "raw_display",
#endif
#ifdef RAW_HALFSIZE
    "raw_halfsize",
#endif
    "load_nopixels",
    NULL
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
  {"allocate",		dib_allocate}, \
  {"allocateEx",	dib_allocateEx}, \
  {"fillBackground",	dib_fillBackground}, \
  {"enlargeCanvas",	dib_enlargeCanvas}, \
  {"load",		dib_load}, \
  {"save",		dib_save}, \
  {"clone",		dib_clone}, \
  {"close",		dib_close}, \
  {"__gc",		dib_close}
