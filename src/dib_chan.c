/* Lua FreeImage: Channel processing */


/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	image_color_channel (string)
 * Returns: [dest. dib_udata]
 */
static int
dib_getChannel (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
    FREE_IMAGE_COLOR_CHANNEL channel = lfi_getchannel(L, 3);

    *dibp = FreeImage_GetChannel(dib, channel);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	image_color_channel (string)
 * Returns: [dest. dib_udata]
 */
static int
dib_setChannel (lua_State *L)
{
    FIBITMAP *dib = lua_unboxpointer(L, 1, DIB_TYPENAME);
    FIBITMAP *dib8 = lua_unboxpointer(L, 2, DIB_TYPENAME);
    FREE_IMAGE_COLOR_CHANNEL channel = lfi_getchannel(L, 3);

    return dib_checkerror(L,
     FreeImage_SetChannel(dib, dib8, channel) ? dib : NULL);
}


#define DIB_CHAN_METHODS \
    {"getChannel",		dib_getChannel}, \
    {"setChannel",		dib_setChannel}
