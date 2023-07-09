NAME := \x1B[35mBreak-Enigma\x1B[39m
MAJOR := 0
MINOR := 0.5
CODENAME := T.E.D. - The Enemy Dial
CODENAME1 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dial\x1B[39m
LIBNAME := $(NAME)
LIBNAME1 := T.E.D. - The Enemy Dial
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
CC_MACOS := gcc
CC_LINUX := gcc
CC_WIN64 := x86_64-w64-mingw32-gcc
CC_WIN32 := i686-w64-mingw32-gcc
PREFIX := /usr/local
CFLAGSY := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION) - KOENIG MARTIN\""
ifeq ($(UNAME),Linux)
LIBS0 := -L/usr/lib
INCLUDES0 := -I/usr/include  -I/usr/include/libxml2
BER := -lm -lxml2 -lpthread
endif
ifeq ($(UNAME),Darwin)
LIBS0 := -L/usr/lib
INCLUDES0 := -I/usr/include -I/usr/include/libxml2
BER := -lm -lxml2 -lpthread
endif
ifeq ($(UNAME),Linux)
.PHONY: all install uninstall clean version

all: enigma enigma32 enigma64 version

enigma: break_enigma.c enigma.c NBestList.c scoreText.c
	$(CC_LINUX) break_enigma.c enigma.c NBestList.c scoreText.c -o break_enigma $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
enigma32: break_enigma.c enigma.c NBestList.c scoreText.c
	$(CC_WIN32) break_enigma.c enigma.c NBestList.c scoreText.c -o break_enigma32.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
enigma64: break_enigma.c enigma.c NBestList.c scoreText.c
	$(CC_WIN64) break_enigma.c enigma.c NBestList.c scoreText.c -o break_enigma64.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
endif

ifeq ($(UNAME),Darwin)
.PHONY: all sign install uninstall clean version

all: x86_app arm_app sign universal_app sign2 version

x86_app: break_enigma.c enigma.c NBestList.c scoreText.c
	$(CC_MACOS) break_enigma.c enigma.c NBestList.c scoreText.c -o x86_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target x86_64-apple-macos10.12

arm_app: break_enigma.c enigma.c NBestList.c scoreText.c
	$(CC_MACOS) break_enigma.c enigma.c NBestList.c scoreText.c -o arm_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target arm64-apple-macos11

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
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none x86_app arm_app
	
sign2:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none universal_app
	
endif
ifeq ($(UNAME),Linux)
install:
	cp ./break_enigma $(PREFIX)/bin/break_enigma
	chmod u+x $(PREFIX)/bin/break_enigma
	mkdir -p $(PREFIX)/share/
	mkdir -p $(PREFIX)/share/enigma/
	cp ./config.xml $(PREFIX)/share/enigma/config.xml
endif
ifeq ($(UNAME),Darwin)
install:
	cp ./universal_app $(PREFIX)/bin/break_enigma
	chmod u+x $(PREFIX)/bin/break_enigma
	mkdir -p $(PREFIX)/share/
	mkdir -p $(PREFIX)/share/enigma/
	cp ./config.xml $(PREFIX)/share/enigma/config.xml
endif
	
uninstall:
	rm $(PREFIX)/bin/break_enigma
	rm $(PREFIX)/share/enigma/config.xml
	
ifeq ($(UNAME),Linux)
clean:
	rm -rf break_enigma
	rm -rf break_enigma32.exe
	rm -rf break_enigma64.exe
endif
ifeq ($(UNAME),Darwin)
clean:
	rm -rf universal_app
	rm -rf x86_app
	rm -rf arm_app
endif
