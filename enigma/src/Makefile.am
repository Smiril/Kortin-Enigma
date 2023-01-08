bin_PROGRAMS = enigma
SIGNING_IDENTITY="Apple Development: sonar@gmx.com (XXXXXXXXXX)"
CXX = clang
AM_CFLAGS = -Wall -Wextra \
	-ferror-limit=500 \
        -DVERSION="\"$(VERSION)\""

LIBS0 = -L/usr/local/lib -L../deps/
INCLUDES0 = -I/usr/local/include -I../deps/
AM_LDFLAGS = 

all-am: sign

enigma_SOURCES = 

sign:
    export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
    xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none enigma
    
