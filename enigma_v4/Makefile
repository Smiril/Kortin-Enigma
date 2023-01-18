NAME := \x1B[35mEnigma\x1B[39m
MAJOR := 0
MINOR := 0.4
CODENAME := T.E.D. - The Enemy Dail
CODENAME1 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m
LIBNAME := $(NAME)
LIBNAME1 := T.E.D. - The Enemy Dail
LIBNAME2 := Enigma
VERSION := $(LIBNAME) $(MAJOR).$(MINOR) $(CODENAME1)
SIGNING_IDENTITY="Apple Development: florit@gmx.at (86ZX5898TT)"
CC := clang
PREFIX := /usr/local/bin
CFLAGSY := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION) - KOENIG MARTIN\""
LIBS0 := -L/usr/local/lib -L/opt/homebrew/Cellar/libxml2/2.10.3_2/lib/
INCLUDES0 := -I/usr/local/include -I/opt/homebrew/Cellar/libxml2/2.10.3_2/include/libxml2/libxml
BER := -lc++ -ldl -lxml2 -lssl -lcrypto -lpthread


.PHONY: all sign install uninstall clean version

all: x86_app arm_app universal_app sign version

x86_app: main.c
	$(CC) main.c -o x86_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target x86_64-apple-macos10.12

arm_app: main.c
	$(CC) main.c -o arm_app $(CFLAGSY) $(BER) $(INCLUDES0) $(LIBS0) -target arm64-apple-macos11

universal_app: x86_app arm_app
	lipo -create -output universal_app x86_app arm_app
    
version:
	@echo " +****************************************************************************+"
	@echo " + $(LIBNAME2) $(MAJOR).$(MINOR) $(LIBNAME1)       by:    Art-ifact-Crew           +"
	@echo " +****************************************************************************+"
sign:
	export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
	xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none x86_app arm_app universal_app
	
install:
	cp ./universal_app $(PREFIX)/enigma
	chmod u+x $(PREFIX)/enigma
	mkdir -p /usr/local/share/enigma
	cp ./*.xml /usr/local/share/enigma/
	cp ./*.txt /usr/local/share/enigma/

uninstall:
	rm $(PREFIX)/enigma
	rm -rf /usr/local/share/enigma

clean:
	rm -rf universal_app
	rm -rf x86_app
	rm -rf arm_app
