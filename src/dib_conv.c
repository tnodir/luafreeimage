/* Lua FreeImage: Conversion functions */


/*
 * Arguments: dest. dib_udata, source dib_udata
 * Returns: [dest. dib_udata]
 */
static int
dib_convert (lua_State *L, FIBITMAP *(DLL_CALLCONV * conv_func)(FIBITMAP *))
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);

  *dibp = conv_func(dib);
  return dib_checkerror(L, *dibp);
}

static int
dib_convertTo4Bits (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo4Bits);
}

static int
dib_convertTo8Bits (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo8Bits);
}

static int
dib_convertToGreyscale (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertToGreyscale);
}

static int
dib_convertTo16Bits555 (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo16Bits555);
}

static int
dib_convertTo16Bits565 (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo16Bits565);
}

static int
dib_convertTo24Bits (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo24Bits);
}

static int
dib_convertTo32Bits (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertTo32Bits);
}

static int
dib_convertToFloat (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertToFloat);
}

static int
dib_convertToRGBF (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertToRGBF);
}

static int
dib_convertToUINT16 (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertToUINT16);
}

static int
dib_convertToRGB16 (lua_State *L)
{
  return dib_convert(L, FreeImage_ConvertToRGB16);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	quantize (string: "wu", "nn")
 * Returns: [dest. dib_udata]
 */
static int
dib_colorQuantize (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const char *quant_name = luaL_checkstring(L, 3);
  FREE_IMAGE_QUANTIZE quantize = (*quant_name == 'w')
   ? FIQ_WUQUANT : FIQ_NNQUANT;

  *dibp = FreeImage_ColorQuantize(dib, quantize);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata, threshold (number)
 * Returns: [dest. dib_udata]
 */
static int
dib_threshold (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const BYTE threshold = lua_tointeger(L, 3);

  *dibp = FreeImage_Threshold(dib, threshold);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	algorithm (string: "fs", "bayer", "cluster"),
 *	order (number: 2, 3, 4, 8)
 * Returns: [dest. dib_udata]
 */
static int
dib_dither (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  const char *algo_name = luaL_checkstring(L, 3);
  const int order = lua_tointeger(L, 4);
  FREE_IMAGE_DITHER algorithm = FID_FS;

  switch (*algo_name) {
  case 'b':
    switch (order) {
    case 2: algorithm = FID_BAYER4x4; break;
    case 3: algorithm = FID_BAYER8x8; break;
    case 4: algorithm = FID_BAYER16x16; break;
    };
    break;
  case 'c':
    switch (order) {
    case 3: algorithm = FID_CLUSTER6x6; break;
    case 4: algorithm = FID_CLUSTER8x8; break;
    case 8: algorithm = FID_CLUSTER16x16; break;
    };
    break;
  }

  *dibp = FreeImage_Dither(dib, algorithm);
  return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	dest. image_type (string), [scale_linear (boolean)]
 * Returns: [dest. dib_udata]
 */
static int
dib_convertToType (lua_State *L)
{
  FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
  FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
  FREE_IMAGE_TYPE dst_type = lfi_gettype(L, 3);
  const BOOL scale_linear = lua_isnoneornil(L, 4) || lua_toboolean(L, 4);

  *dibp = FreeImage_ConvertToType(dib, dst_type, scale_linear);
  return dib_checkerror(L, *dibp);
}


#define DIB_CONV_METHODS \
  {"convertTo4Bits",		dib_convertTo4Bits}, \
  {"convertTo8Bits",		dib_convertTo8Bits}, \
  {"convertToGreyscale",	dib_convertToGreyscale}, \
  {"convertTo16Bits555",	dib_convertTo16Bits555}, \
  {"convertTo16Bits565",	dib_convertTo16Bits565}, \
  {"convertTo24Bits",		dib_convertTo24Bits}, \
  {"convertTo32Bits",		dib_convertTo32Bits}, \
  {"convertToFloat",		dib_convertToFloat}, \
  {"convertToRGBF",		dib_convertToRGBF}, \
  {"convertToUINT16",		dib_convertToUINT16}, \
  {"convertToRGB16",		dib_convertToRGB16}, \
  {"colorQuantize",		dib_colorQuantize}, \
  {"threshold",			dib_threshold}, \
  {"dither",			dib_dither}, \
  {"convertToType",		dib_convertToType}
