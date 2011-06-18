/* Lua FreeImage: Upsampling / Downsampling */


/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	dst_width (number), dst_height (number), filter (string)
 * Returns: [dest. dib_udata]
 */
static int
dib_rescale (lua_State *L)
{
    static const FREE_IMAGE_FILTER filter_values[] = {
	FILTER_BOX, FILTER_BICUBIC, FILTER_BILINEAR,
	FILTER_BSPLINE, FILTER_CATMULLROM, FILTER_LANCZOS3
    };
    static const char *const filter_names[] = {
	"box", "bicubic", "bilinear",
	"bspline", "catmullrom", "lanczos3", NULL
    };
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
    const int dst_width = lua_tointeger(L, 3);
    const int dst_height = lua_tointeger(L, 4);
    FREE_IMAGE_FILTER filter = filter_values[
     luaL_checkoption(L, 5, NULL, filter_names)];

    *dibp = FreeImage_Rescale(dib, dst_width, dst_height, filter);
    return dib_checkerror(L, *dibp);
}

/*
 * Arguments: dest. dib_udata, source dib_udata,
 *	max_pixel_size (number), [convert (boolean)]
 * Returns: [dest. dib_udata]
 */
static int
dib_makeThumbnail (lua_State *L)
{
    FIBITMAP **dibp = checkudata(L, 1, DIB_TYPENAME);
    FIBITMAP *dib = lua_unboxpointer(L, 2, DIB_TYPENAME);
    const int max_pixel_size = lua_tointeger(L, 3);
    const BOOL convert = lua_isnoneornil(L, 4) || lua_toboolean(L, 4);

    *dibp = FreeImage_MakeThumbnail(dib, max_pixel_size, convert);
    return dib_checkerror(L, *dibp);
}


#define DIB_SAMP_METHODS \
    {"rescale",			dib_rescale}, \
    {"makeThumbnail",		dib_makeThumbnail}
