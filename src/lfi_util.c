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
#ifdef FIF_BMP
    FIF_BMP,
#endif
#ifdef FIF_ICO
    FIF_ICO,
#endif
#ifdef FIF_JPEG
    FIF_JPEG,
#endif
#ifdef FIF_JNG
    FIF_JNG,
#endif
#ifdef FIF_KOALA
    FIF_KOALA,
#endif
#ifdef FIF_LBM
    FIF_LBM,
#endif
#ifdef FIF_IFF
    FIF_IFF,
#endif
#ifdef FIF_MNG
    FIF_MNG,
#endif
#ifdef FIF_PBM
    FIF_PBM,
#endif
#ifdef FIF_PBMRAW
    FIF_PBMRAW,
#endif
#ifdef FIF_PCD
    FIF_PCD,
#endif
#ifdef FIF_PCX
    FIF_PCX,
#endif
#ifdef FIF_PGM
    FIF_PGM,
#endif
#ifdef FIF_PGMRAW
    FIF_PGMRAW,
#endif
#ifdef FIF_PNG
    FIF_PNG,
#endif
#ifdef FIF_PPM
    FIF_PPM,
#endif
#ifdef FIF_PPMRAW
    FIF_PPMRAW,
#endif
#ifdef FIF_RAS
    FIF_RAS,
#endif
#ifdef FIF_TARGA
    FIF_TARGA,
#endif
#ifdef FIF_TIFF
    FIF_TIFF,
#endif
#ifdef FIF_WBMP
    FIF_WBMP,
#endif
#ifdef FIF_PSD
    FIF_PSD,
#endif
#ifdef FIF_CUT
    FIF_CUT,
#endif
#ifdef FIF_XBM
    FIF_XBM,
#endif
#ifdef FIF_XPM
    FIF_XPM,
#endif
#ifdef FIF_DDS
    FIF_DDS,
#endif
#ifdef FIF_GIF
    FIF_GIF,
#endif
#ifdef FIF_HDR
    FIF_HDR,
#endif
#ifdef FIF_FAXG3
    FIF_FAXG3,
#endif
#ifdef FIF_SGI
    FIF_SGI,
#endif
#ifdef FIF_EXR
    FIF_EXR,
#endif
#ifdef FIF_J2K
    FIF_J2K,
#endif
#ifdef FIF_JP2
    FIF_JP2,
#endif
#ifdef FIF_PFM
    FIF_PFM,
#endif
#ifdef FIF_PICT
    FIF_PICT,
#endif
#ifdef FIF_RAW
    FIF_RAW,
#endif
    FIF_UNKNOWN
};

static const char *const lfi_format_names[] = {
#ifdef FIF_BMP
    "bmp",
#endif
#ifdef FIF_ICO
    "ico",
#endif
#ifdef FIF_JPEG
    "jpeg",
#endif
#ifdef FIF_JNG
    "jng",
#endif
#ifdef FIF_KOALA
    "koala",
#endif
#ifdef FIF_LBM
    "lbm",
#endif
#ifdef FIF_IFF
    "iff",
#endif
#ifdef FIF_MNG
    "mng",
#endif
#ifdef FIF_PBM
    "pbm",
#endif
#ifdef FIF_PBMRAW
    "pbmraw",
#endif
#ifdef FIF_PCD
    "pcd",
#endif
#ifdef FIF_PCX
    "pcx",
#endif
#ifdef FIF_PGM
    "pgm",
#endif
#ifdef FIF_PGMRAW
    "pgmraw",
#endif
#ifdef FIF_PNG
    "png",
#endif
#ifdef FIF_PPM
    "ppm",
#endif
#ifdef FIF_PPMRAW
    "ppmraw",
#endif
#ifdef FIF_RAS
    "ras",
#endif
#ifdef FIF_TARGA
    "targa",
#endif
#ifdef FIF_TIFF
    "tiff",
#endif
#ifdef FIF_WBMP
    "wbmp",
#endif
#ifdef FIF_PSD
    "psd",
#endif
#ifdef FIF_CUT
    "cut",
#endif
#ifdef FIF_XBM
    "xbm",
#endif
#ifdef FIF_XPM
    "xpm",
#endif
#ifdef FIF_DDS
    "dds",
#endif
#ifdef FIF_GIF
    "gif",
#endif
#ifdef FIF_HDR
    "hdr",
#endif
#ifdef FIF_FAXG3
    "faxg3",
#endif
#ifdef FIF_SGI
    "sgi",
#endif
#ifdef FIF_EXR
    "exr",
#endif
#ifdef FIF_J2K
    "j2k",
#endif
#ifdef FIF_JP2
    "jp2",
#endif
#ifdef FIF_PFM
    "pfm",
#endif
#ifdef FIF_PICT
    "pict",
#endif
#ifdef FIF_RAW
    "raw",
#endif
    "unknown", NULL
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

