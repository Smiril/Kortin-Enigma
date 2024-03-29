NAME := \x1B[35mEnigma\x1B[39m
MAJOR := 2
MINOR := 2.5
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
CC_MACOS := clang
CC_LINUX := clang
CC_WIN64 := x86_64-w64-mingw32-gcc
CC_WIN32 := i686-w64-mingw32-gcc
PREFIX := /usr/local
CFLAGSY := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION) - LARAS CRAFT\""
ifeq ($(UNAME),Linux)
LIBS0 := -L/usr/lib 
INCLUDES0 := -I/usr/include/libxml2 
BER := -ldl -lxml2 -std=c17
endif
ifeq ($(UNAME),Darwin)
LIBS0 := -L/usr/lib
INCLUDES0 :=
BER := -lc++ -ldl -lxml2 -std=c2x
endif
ifeq ($(UNAME),Linux)
.PHONY: all install uninstall clean version

all: enigma enigma32 enigma64 version

enigma: main.m
	$(CC_LINUX) main.m -o enigma $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
enigma32: main.m
	$(CC_WIN32) main.m -o enigma32.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
enigma64: main.m
	$(CC_WIN64) main.m -o enigma64.exe $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -s
	
endif

ifeq ($(UNAME),Darwin)
.PHONY: all sign0 sign1 sign2 install uninstall clean version

all: x86_app sign0 arm_app sign1 universal_app sign2 version

x86_app: main.m
	$(CC_MACOS) main.m -o x86_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target x86_64-apple-macos10.12

arm_app: main.m
	$(CC_MACOS) main.m -o arm_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target arm64-apple-macos11

universal_app: x86_app arm_app
	lipo -create -output universal_app x86_app arm_app
    
endif
version:
	@echo " +****************************************************************************+"
	@echo " + $(LIBNAME2) $(MAJOR).$(MINOR) $(LIBNAME1)       by:    Art-ifact-Crew           +"
	@echo " +****************************************************************************+"
	
ifeq ($(UNAME),Darwin)
sign0:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none x86_app
	
sign1:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none arm_app
	
sign2:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none universal_app
	
endif
ifeq ($(UNAME),Linux)
install:
	cp ./enigma $(PREFIX)/bin/enigma
	chmod u+x $(PREFIX)/bin/enigma
	mkdir -p /usr/local/share/
	mkdir -p /usr/local/share/enigma
	cp ../share/norway*.xml /usr/local/share/enigma/
endif
ifeq ($(UNAME),Darwin)
install:
	cp ./universal_app $(PREFIX)/bin/enigma
	chmod u+x $(PREFIX)/bin/enigma
	mkdir -p /usr/local/share/
	mkdir -p /usr/local/share/enigma
	cp ../share/norway*.xml /usr/local/share/enigma/
endif

uninstall:
	rm $(PREFIX)/bin/enigma
	rm $(PREFIX)/share/enigma/norway*.xml
	
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
