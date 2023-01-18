NAME := \x1B[35mEnigma\x1B[39m
MAJOR := 0
MINOR := 0.4
CODENAME := T.E.D. - The Enemy Dail
CODENAME1 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m
LIBNAME := $(NAME)
LIBNAME1 := T.E.D. - The Enemy Dail
LIBNAME2 := Enigma
VERSION := $(LIBNAME) $(MAJOR).$(MINOR) $(CODENAME1)
SIGNING_IDENTITY="Apple Development: sonar@gmx.com (XXXXXXXXXX)"
CC := clang
PREFIX := /usr/local
CFLAGSY := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION) - KOENIG MARTIN\""
LIBS0 := -L/usr/local/lib
INCLUDES0 := -I/usr/local/include
BER := -lc++ -ldl -lxml2


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
	cp ./universal_app $(PREFIX)/bin/enigma
	chmod u+x $(PREFIX)/bin/enigma
	mkdir -p $(PREFIX)/share/enigma
	cp ../share/*.xml $(PREFIX)/share/enigma/
	cp ../share/*.txt $(PREFIX)/share/enigma/

uninstall:
	rm -f $(PREFIX)/bin/enigma
	rm -rf $(PREFIX)/share/enigma

clean:
	rm -rf universal_app
	rm -rf x86_app
	rm -rf arm_app
