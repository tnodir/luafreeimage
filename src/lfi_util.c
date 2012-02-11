/* Lua FreeImage: Utilities */


/* Image types */
static const FREE_IMAGE_TYPE lfi_type_values[] = {
    FIT_BITMAP,
    FIT_UINT16, FIT_INT16, FIT_UINT32, FIT_INT32,
    FIT_FLOAT, FIT_DOUBLE, FIT_COMPLEX,
    FIT_RGB16, FIT_RGBA16, FIT_RGBF, FIT_RGBAF
};

static const char *const lfi_type_names[] = {
    "bitmap",
    "uint16", "int16", "uint32", "int32",
    "float", "double", "complex",
    "rgb16", "rgba16", "rgbf", "rgbaf", NULL
};


/* Image formats */
static const FREE_IMAGE_FORMAT lfi_format_values[] = {
    FIF_BMP,
    FIF_ICO,
    FIF_JPEG,
    FIF_JNG,
    FIF_KOALA,
    FIF_LBM,
    FIF_IFF,
    FIF_MNG,
    FIF_PBM,
    FIF_PBMRAW,
    FIF_PCD,
    FIF_PCX,
    FIF_PGM,
    FIF_PGMRAW,
    FIF_PNG,
    FIF_PPM,
    FIF_PPMRAW,
    FIF_RAS,
    FIF_TARGA,
    FIF_TIFF,
    FIF_WBMP,
    FIF_PSD,
    FIF_CUT,
    FIF_XBM,
    FIF_XPM,
    FIF_DDS,
    FIF_GIF,
    FIF_HDR,
    FIF_FAXG3,
    FIF_SGI,
    FIF_EXR,
    FIF_J2K,
    FIF_JP2,
    FIF_PFM,
    FIF_PICT,
    FIF_RAW,
    FIF_UNKNOWN
};

static const char *const lfi_format_names[] = {
    "bmp",
    "ico",
    "jpeg",
    "jng",
    "koala",
    "lbm",
    "iff",
    "mng",
    "pbm",
    "pbmraw",
    "pcd",
    "pcx",
    "pgm",
    "pgmraw",
    "png",
    "ppm",
    "ppmraw",
    "ras",
    "targa",
    "tiff",
    "wbmp",
    "psd",
    "cut",
    "xbm",
    "xpm",
    "dds",
    "gif",
    "hdr",
    "faxg3",
    "sgi",
    "exr",
    "j2k",
    "jp2",
    "pfm",
    "pict",
    "raw",
    "unknown",
    NULL
};


static int
lfi_pushoption (lua_State *L, const int flag, const int *flags,
                const char*const lst[]) {
    int i;

    for (i = 0; lst[i]; i++) {
	if (flags[i] == flag) break;
    }
    lua_pushstring(L, lst[i]);
    return 1;
}

/*
 * Arguments: ..., image_type (string)
 */
static FREE_IMAGE_TYPE
lfi_gettype (lua_State *L, int idx)
{
    return lfi_type_values[luaL_checkoption(L, idx, "bitmap", lfi_type_names)];
}

/*
 * Arguments: ..., image_format (string)
 */
static FREE_IMAGE_FORMAT
lfi_getformat (lua_State *L, int idx)
{
    return lfi_format_values[luaL_checkoption(L, idx, "unknown", lfi_format_names)];
}

/*
 * Arguments: ..., image_color_channel (string)
 */
static FREE_IMAGE_COLOR_CHANNEL
lfi_getchannel (lua_State *L, int idx)
{
    static const FREE_IMAGE_COLOR_CHANNEL channel_values[] = {
	FICC_RGB, FICC_RED, FICC_GREEN, FICC_BLUE, FICC_ALPHA,
	FICC_BLACK, FICC_REAL, FICC_IMAG, FICC_MAG, FICC_PHASE
    };
    static const char *const channel_names[] = {
	"rgb", "red", "green", "blue", "alpha",
	"black", "real", "imag", "mag", "phase", NULL
    };
    return channel_values[luaL_checkoption(L, idx, "rgb", channel_names)];
}

/*
 * Arguments: color (number: 0xAABBGGRR)
 *	| string (SVGColorName | "#RRGGBBAA")
 */
static void
lfi_tocolor (lua_State *L, int idx, RGBQUAD *col)
{
    if (lua_type(L, idx) == LUA_TNUMBER) {
	const lua_Number num = lua_tonumber(L, idx);
	const unsigned int abgr = (unsigned int) num;

	col->rgbRed = (BYTE) abgr;
	col->rgbGreen = (BYTE) (abgr >> 8);
	col->rgbBlue = (BYTE) (abgr >> 16);
	col->rgbReserved = (BYTE) (abgr >> 24);
    }
    else {
	const char *s = lua_tostring(L, idx);

	if (s != NULL) {
	    memset(col, 0, sizeof(RGBQUAD));  /* transparent */
	}
	else if (*s == '#') {
	    BYTE rgba[4];
	    int i;

	    for (i = 0; *(++s) && i < 8; ++i) {
		unsigned int c = *s;
		c = (c >= '0' && c <= '9')
		 ? c - '0' : (c | 0x20) - 'a' + 0xA;
		rgba[i >> 1] |= c << ((i & 1) ? 0 : 4);
	    }

	    col->rgbRed = rgba[0];
	    col->rgbGreen = rgba[1];
	    col->rgbBlue = rgba[2];
	    col->rgbReserved = (i > 6) ? rgba[3] : 0xFF;
	}
	else {
	    FreeImage_LookupSVGColor(s,
	     &col->rgbRed, &col->rgbGreen, &col->rgbBlue);
	    col->rgbReserved = 0xFF;
	}
    }
}

/*
 * Returns: color (number: 0xAABBGGRR)
 */
static int
lfi_pushcolor (lua_State *L, RGBQUAD *col)
{
    lua_pushnumber(L, col->rgbRed | (col->rgbGreen << 8)
     | (col->rgbBlue << 16) | (col->rgbReserved << 24));
    return 1;
}

