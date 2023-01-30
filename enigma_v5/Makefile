NAME := \x1B[35mEnigma\x1B[39m
MAJOR := 0
MINOR := 0.5
CODENAME := T.E.D. - The Enemy Dail
CODENAME1 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m
LIBNAME := $(NAME)
LIBNAME1 := T.E.D. - The Enemy Dail
LIBNAME2 := Enigma
VERSION := $(LIBNAME) $(MAJOR).$(MINOR) $(CODENAME1)
##
## Detect Operating System
##

UNAME                   := $(shell uname -s)

# we need to strip the windows version number to be able to build enigma on cygwin hosts
UNAME                   := $(patsubst CYGWIN_NT-%,CYGWIN,$(UNAME))

# same for msys
UNAME                   := $(patsubst MSYS_NT-%,MSYS2,$(UNAME))
UNAME                   := $(patsubst MINGW32_NT-%,MSYS2,$(UNAME))
UNAME                   := $(patsubst MINGW64_NT-%,MSYS2,$(UNAME))

ifeq (,$(filter $(UNAME),Linux Darwin CYGWIN MSYS2))
$(error "! Your Operating System ($(UNAME)) is not supported by this Makefile")
endif
ifeq ($(UNAME),Darwin)
SIGNING_IDENTITY="Apple Development: sonar@gmx.com (XXXXXXXXXX)"
endif
CC_MACOS := clang
CC_LINUX := clang
CC_WIN64 := x86_64-w64-mingw32-gcc
CC_WIN32 := i686-w64-mingw32-gcc
PREFIX := /usr/local
CFLAGSY := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION) - KOENIG MARTIN\""
ifeq ($(UNAME),Linux)
LIBS0 := -L/usr/lib -L/usr/lib/iconv_hook
INCLUDES0 := -I/usr/include/libxml2 -I/usr/lib/iconv_hook
BER := -lc++ -ldl -lxml2 -liconv_hook -lssl -lcrypto -lpthread
endif
ifeq ($(UNAME),Darwin)
LIBS0 := -L/usr/lib
INCLUDES0 :=
BER := -lc++ -ldl -lxml2 -liconv -lssl -lcrypto -lpthread
endif
ifeq ($(UNAME),Linux)
.PHONY: all install uninstall clean version

all: enigma enigma32 enigma64 version

enigma: main.c
	$(CC_LINUX) main.c -o enigma $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
enigma32: main.c
	$(CC_WIN32) main.c -o enigma32.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -lws2_w32 -s
	
enigma64: main.c
	$(CC_WIN64) main.c -o enigma64.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -lws2_w32 -s
	
endif

ifeq ($(UNAME),Darwin)
.PHONY: all sign install uninstall clean version

all: x86_app arm_app universal_app sign version

x86_app: main.c
	$(CC_MACOS) main.c -o x86_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target x86_64-apple-macos10.12

arm_app: main.c
	$(CC_MACOS) main.c -o arm_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target arm64-apple-macos11

universal_app: x86_app arm_app
	lipo -create -output universal_app x86_app arm_app
    
endif
version:
	@echo " +****************************************************************************+"
	@echo " + $(LIBNAME2) $(MAJOR).$(MINOR) $(LIBNAME1)       by:    Art-ifact-Crew           +"
	@echo " +****************************************************************************+"
ifeq ($(UNAME),Darwin)
sign:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none x86_app arm_app universal_app
	
endif
ifeq ($(UNAME),Linux)
install:
	cp ./enigma $(PREFIX)/bin/enigma
	chmod u+x $(PREFIX)/bin/enigma
	mkdir -p /usr/local/share/enigma
	cp ../share/*.xml /usr/local/share/enigma/
	cp ../share/*.txt /usr/local/share/enigma/
endif
ifeq ($(UNAME),Darwin)
install:
	cp ./universal_app $(PREFIX)/bin/enigma
	chmod u+x $(PREFIX)/bin/enigma
	mkdir -p /usr/local/share/enigma
	cp ../share/*.xml /usr/local/share/enigma/
	cp ../share/*.txt /usr/local/share/enigma/
endif

uninstall:
	rm $(PREFIX)/bin/enigma
	rm -rf /usr/local/share/enigma
ifeq ($(UNAME),Linux)
clean:
	rm -rf enigma
endif
ifeq ($(UNAME),Darwin)
clean:
	rm -rf universal_app
	rm -rf x86_app
	rm -rf arm_app
endif
