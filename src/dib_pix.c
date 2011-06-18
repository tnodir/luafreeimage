/* Lua FreeImage: Pixel access functions */


/*
 * Arguments: dib_udata
 * Returns: lightuserdata
 */
static int
dib_getBits (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);

    lua_pushlightuserdata(L, FreeImage_GetBits(dib));
    return 1;
}

/*
 * Arguments: dib_udata, scanline (number)
 * Returns: lightuserdata
 */
static int
dib_getScanLine (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const int scanline = lua_tointeger(L, 2);

    lua_pushlightuserdata(L, FreeImage_GetScanLine(dib, scanline));
    return 1;
}

/*
 * Arguments: dib_udata, x (number), y (number)
 * Returns: [index (number)]
 */
static int
dib_getPixelIndex (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const unsigned int x = lua_tointeger(L, 2);
    const unsigned int y = lua_tointeger(L, 3);
    BYTE i;

    if (FreeImage_GetPixelIndex(dib, x, y, &i)) {
	lua_pushinteger(L, i);
	return 1;
    }
    return 0;
}

/*
 * Arguments: dib_udata, x (number), y (number), index (number)
 * Returns: [dib_udata]
 */
static int
dib_setPixelIndex (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const unsigned int x = lua_tointeger(L, 2);
    const unsigned int y = lua_tointeger(L, 3);
    BYTE i = lua_tointeger(L, 4);

    return dib_checkerror(L,
     FreeImage_SetPixelIndex(dib, x, y, &i) ? dib : NULL);
}

/*
 * Arguments: dib_udata, x (number), y (number)
 * Returns: [Color]
 */
static int
dib_getPixelColor (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const unsigned int x = lua_tointeger(L, 2);
    const unsigned int y = lua_tointeger(L, 3);
    RGBQUAD col;

    if (FreeImage_GetPixelColor(dib, x, y, &col)) {
	return lfi_pushcolor(L, &col);
    }
    return 0;
}

/*
 * Arguments: dib_udata, x (number), y (number), Color
 * Returns: [dib_udata]
 */
static int
dib_setPixelColor (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    const unsigned int x = lua_tointeger(L, 2);
    const unsigned int y = lua_tointeger(L, 3);
    RGBQUAD col;

    lfi_tocolor(L, 4, &col);

    return dib_checkerror(L,
     FreeImage_SetPixelColor(dib, x, y, &col) ? dib : NULL);
}


#define DIB_PIX_METHODS \
    {"getBits",			dib_getBits}, \
    {"getScanLine",		dib_getScanLine}, \
    {"getPixelIndex",		dib_getPixelIndex}, \
    {"setPixelIndex",		dib_setPixelIndex}, \
    {"getPixelColor",		dib_getPixelColor}, \
    {"setPixelColor",		dib_setPixelColor}
