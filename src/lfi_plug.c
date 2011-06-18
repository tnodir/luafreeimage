/* Lua FreeImage: Plugin functions */


/*
 * Returns: number
 */
static int
lfi_getFIFCount (lua_State *L)
{
    lua_pushinteger(L, FreeImage_GetFIFCount());
    return 1;
}

/*
 * Arguments: image_format (string), [enable (boolean)]
 * Returns: [previous_state (boolean)]
 */
static int
lfi_setPluginEnabled (lua_State *L)
{
    const FREE_IMAGE_FORMAT fif = lfi_getformat(L, 1);
    const BOOL enable = lua_isnoneornil(L, 2) || lua_toboolean(L, 2);
    const int old = FreeImage_SetPluginEnabled(fif, enable);

    if (old != -1) {
	lua_pushboolean(L, old);
	return 1;
    }
    return 0;
}

/*
 * Arguments: image_format (string)
 * Returns: [boolean]
 */
static int
lfi_isPluginEnabled (lua_State *L)
{
    const FREE_IMAGE_FORMAT fif = lfi_getformat(L, 1);
    const int enabled = FreeImage_IsPluginEnabled(fif);

    if (enabled != -1) {
	lua_pushboolean(L, enabled);
	return 1;
    }
    return 0;
}

/*
 * Arguments: image_format (string)
 * Returns: string
 */
static int
lfi_getFIFMimeType (lua_State *L)
{
    const FREE_IMAGE_FORMAT fif = lfi_getformat(L, 1);

    lua_pushstring(L, FreeImage_GetFIFMimeType(fif));
    return 1;
}

/*
 * Arguments: path (string)
 * Returns: image_format (string)
 */
static int
lfi_getFIFFromFilename (lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path);

    return lfi_pushoption(L, fif, lfi_format_values, lfi_format_names);
}


#define LFI_PLUG_METHODS \
    {"getFIFCount",		lfi_getFIFCount}, \
    {"setPluginEnabled",	lfi_setPluginEnabled}, \
    {"isPluginEnabled",		lfi_isPluginEnabled}, \
    {"getFIFMimeType",		lfi_getFIFMimeType}, \
    {"getFIFFromFilename",	lfi_getFIFFromFilename}
