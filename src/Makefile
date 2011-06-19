
PLAT= generic

LUA= /tmp/lua-5.1.4
FREEIMAGE= /tmp/FreeImage

T= freeimage.so
A= $(LUA)/src/libluafreeimage.a

COPT= -O2 -fPIC -DNDEBUG
CWARNS = -W -Wall -Wextra -pedantic \
	-Waggregate-return \
	-Wbad-function-cast \
	-Wcast-align \
	-Wcast-qual \
	-Wdeclaration-after-statement \
	-Wdisabled-optimization \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Wpointer-arith \
	-Wshadow \
	-Wsign-compare \
	-Wstrict-prototypes \
	-Wundef \
	-Wwrite-strings \
	-Wunreachable-code

CFLAGS= $(COPT) $(CWARNS) $(MYCFLAGS) -I$(LUA)/src -I$(FREEIMAGE)/Source
LIBS= -lstdc++ -L$(FREEIMAGE) -lfreeimage $(MYLIBS)

MYCFLAGS=
MYLDFLAGS= -shared -Bdynamic
MYLIBS=

CC= gcc
MAKE_DLL= gcc $(MYLDFLAGS) -o
AR= ar rcu
RANLIB= ranlib
RM= rm -f
STRIP= strip

PLATS= generic osx

OBJS= luafreeimage.o
LDOBJS= $(OBJS)


default: $(PLAT)

all:	$(T)

a:	$(A)

$(T): $(OBJS)
	$(MAKE_DLL) $@ $(LDOBJS) $(LIBS)
	$(STRIP) $@

$(A): $(OBJS)
	$(AR) $@ $?
	$(RANLIB) $@


none:
	@echo "Please choose a platform:"
	@echo "	$(PLATS)"

generic:
	$(MAKE) all MYCFLAGS=

osx:
	$(MAKE) all MYCFLAGS= STRIP="echo strip" \
	MYLDFLAGS="-bundle -undefined dynamic_lookup /usr/lib/bundle1.o"

clean:
	$(RM) $(A) $(T) *.o


.PHONY: all $(PLATS) default a clean


luafreeimage.o: luafreeimage.c dib.c dib_chan.c dib_col.c dib_conv.c dib_copy.c \
    dib_info.c dib_pix.c dib_rot.c dib_samp.c lfi_plug.c lfi_util.c common.h
