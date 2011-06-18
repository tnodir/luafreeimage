/* Lua FreeImage: Color manipulation  */


/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	[tone_mapping_operator (string: "drago", "reinhard", "fattal"),
 *	first_param (number), second_param (number)]
 * Returns: [dest. dib_udata]
 */
static int
dib_toneMapping (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
    const char *tmo_name = luaL_checkstring(L, 3);
    FREE_IMAGE_TMO tmo = FITMO_DRAGO03;
    const double first_param = (double) lua_tonumber(L, 4);
    const double second_param = (double) lua_tonumber(L, 5);

    if (tmo_name) {
	switch (*tmo_name) {
	case 'r': tmo = FITMO_REINHARD05; break;
	case 'f': tmo = FITMO_FATTAL02; break;
	}
    }

    *dibp = FreeImage_ToneMapping(dib, tmo, first_param, second_param);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dib_udata, gamma (number)
 * Returns: [dib_udata]
 */
static int
dib_adjustGamma (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const double gamma = (double) lua_tonumber(L, 2);

    return dib_checkerror(L,
     FreeImage_AdjustGamma(dib, gamma) ? dib : NULL);
}

/*
 * Arguments: dib_udata, percentage (number)
 * Returns: [dib_udata]
 */
static int
dib_adjustBrightness (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const double percentage = (double) lua_tonumber(L, 2);

    return dib_checkerror(L,
     FreeImage_AdjustBrightness(dib, percentage) ? dib : NULL);
}

/*
 * Arguments: dib_udata, percentage (number)
 * Returns: [dib_udata]
 */
static int
dib_adjustContrast (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const double percentage = (double) lua_tonumber(L, 2);

    return dib_checkerror(L,
     FreeImage_AdjustContrast(dib, percentage) ? dib : NULL);
}

/*
 * Arguments: dib_udata
 * Returns: [dib_udata]
 */
static int
dib_invert (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

    return dib_checkerror(L,
     FreeImage_Invert(dib) ? dib : NULL);
}

/*
 * Arguments: dib_udata, brightness (number), contrast (number),
 *	gamma (number), invert (boolean)
 * Returns: [dib_udata]
 */
static int
dib_adjustColors (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const double brightness = (double) lua_tonumber(L, 2);
    const double contrast = (double) lua_tonumber(L, 3);
    const double gamma = (double) lua_tonumber(L, 4);
    const BOOL invert = lua_toboolean(L, 5);

    return dib_checkerror(L,
     FreeImage_AdjustColors(dib, brightness, contrast, gamma, invert)
     ? dib : NULL);
}

/*
 * Arguments: dib_udata, color_a (Color), color_b (Color),
 *	ignore_alpha (boolean)
 * Returns: [changed_pixels (number)]
 */
static int
dib_swapColors (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    RGBQUAD color_a, color_b;
    const BOOL ignore_alpha = lua_toboolean(L, 4);

    lfi_tocolor(L, 2, &color_a);
    lfi_tocolor(L, 3, &color_b);

    lua_pushinteger(L,
     FreeImage_SwapColors(dib, &color_a, &color_b, ignore_alpha));
    return 1;
}

/*
 * Arguments: dib_udata, index_a (number), index_b (number)
 * Returns: [changed_pixels (number)]
 */
static int
dib_swapPaletteIndices (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    BYTE index_a = lua_tointeger(L, 2);
    BYTE index_b = lua_tointeger(L, 3);

    lua_pushinteger(L,
     FreeImage_SwapPaletteIndices(dib, &index_a, &index_b));
    return 1;
}


#define DIB_COL_METHODS \
    {"toneMapping",		dib_toneMapping}, \
    {"adjustGamma",		dib_adjustGamma}, \
    {"adjustBrightness",	dib_adjustBrightness}, \
    {"adjustContrast",		dib_adjustContrast}, \
    {"invert",			dib_invert}, \
    {"adjustColors",		dib_adjustColors}, \
    {"swapColors",		dib_swapColors}, \
    {"swapPaletteIndices",	dib_swapPaletteIndices}
