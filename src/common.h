#ifndef COMMON_H
#define COMMON_H

#include <string.h>	/* memset, memchr */

#include <FreeImage.h>

#define LUA_LIB

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


#ifdef NO_CHECK_UDATA
#define checkudata(L,i,tname)	lua_touserdata(L, i)
#else
#define checkudata(L,i,tname)	luaL_checkudata(L, i, tname)
#endif

#define lua_boxpointer(L,u) \
    (*(void **) (lua_newuserdata(L, sizeof(void *))) = (u))

#define lua_unboxpointer(L,i,tname) \
    (*(void **) (checkudata(L, i, tname)))


#endif
