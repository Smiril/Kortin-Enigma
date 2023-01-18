# openssl
## Cloning

```
git clone https://github.com/openssl/openssl.git

```

## Building

```
export MACOSX_DEPLOYMENT_TARGET=10.9
cp -r openssl openssl-arm64 
cp -r openssl openssl-x86_x64 
cd openssl-x86_x64 
./Configure darwin64-x86_64-cc shared
make
export MACOSX_DEPLOYMENT_TARGET=10.15 /* arm64 only with Big Sur -> minimum might be 10.16 or 11.0 */)
cd ../openssl-arm64 
./Configure enable-rc5 zlib darwin64-arm64-cc no-asm
make
cd ..
mkdir openssl-mac
lipo -create openssl-arm64/libcrypto.a openssl-x86_x64/libcrypto.a -output openssl-mac/libcrypto.a
lipo -create openssl-arm64/libssl.a openssl-x86_x64/libssl.a -output openssl-mac/libssl.a
lipo -create openssl-arm64/libcrypto.dylib openssl-x86_x64/libcrypto.dylib -output openssl-mac/libcrypto.dylib
lipo -create openssl-arm64/libssl.dylib openssl-x86_x64/libssl.dylib -output openssl-mac/libssl.dylib
cd openssl-mac
file libcrypto.a libssl.a
file libcrypto.dylib libssl.dylib
install_name_tool -id '@rpath/libcrypto.dylib' libcrypto.dylib
install_name_tool -id '@rpath/libssl.dylib' libssl.dylib
otool -D libcrypto.dylib /* to verify */
otool -D libssl.dylib /* to verify */
sudo cp *.dylib /usr/local/lib/
cd ..

```
