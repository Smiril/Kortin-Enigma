bin_PROGRAMS = enigma
SIGNING_IDENTITY="Apple Development: sonar@gmx.com (XXXXXXXXXX)"
CXX = clang
AM_CFLAGS = -Wall -Wextra \
	-ferror-limit=500 \
        -DVERSION="\"$(VERSION)\"" -D_7ZIP_ST \
	-DIOAPI_NO_64 -DUNZ_BUFSIZE=262144 \
	-pipe -Iinclude/ -IOpenCL/ \
	-DWITH_HWMON -DMISSING_CLOCK_GETTIME

LIBS0 = -L/usr/local/lib -L../deps/
INCLUDES0 = -I/usr/local/include -I../deps/
AM_LDFLAGS = -lc++ -lstdc++ -ldl -lm -liconv -framework CoreFoundation -framework CoreGraphics -framework Foundation -framework Metal -framework IOKit 

all-am: sign

enigma_SOURCES = add.h folder.c addx.h folder.h air.h hlfmt.c hlfmt.h backend.c hwmon.c backend.h hwmon.h bitmap.c inc_types.h bitmap.h inc_vendor.h induct.c bitops.c induct.h bitops.h interface.c interface.h keyboard_layout.c keyboard_layout.h locking.c common.h locking.h convert.c convert.h logfile.c logfile.h cpt.c cpt.h loopback.c loopback.h cpu_crc32.c cpu_crc32.h main.c debugfile.c memory.c debugfile.h memory.h dictstat.c modules.h dictstat.h monitor.c monitor.h dispatch.c dispatch.h mpsp.c mpsp.h dynloader.c dynloader.h outfile.c outfile.h emu_general.c pidfile.c emu_general.h pidfile.h emu_inc_rp.c emu_inc_rp.h potfile.c emu_inc_rp_optimized.c potfile.h emu_inc_rp_optimized.h restore.c restore.h event.c rp.c event.h rp.h ext_ADL.c rp_cpu.c ext_ADL.h rp_cpu.h ext_OpenCL.c selftest.c ext_OpenCL.h selftest.h ext_cuda.c shared.c ext_cuda.h shared.h ext_hip.c slow_candidates.c ext_hip.h slow_candidates.h ext_hiprtc.c sort_r.h ext_hiprtc.h status.c ext_iokit.c status.h ext_iokit.h stdout.c stdout.h straight.c ext_metal.h straight.h ext_metal.m terminal.c terminal.h ext_nvapi.c thread.c ext_nvapi.h thread.h ext_nvml.c ext_nvml.h timer.c ext_nvrtc.c timer.h ext_nvrtc.h tuningdb.c ext_sysfs_amdgpu.c tuningdb.h ext_sysfs_amdgpu.h types.h ext_sysfs_cpu.c wordlist.c ext_sysfs_cpu.h wordlist.h user_options.c user_options.h hashcat.c hashcat.h outfile_check.h xxhash.h

sign:
    export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/../bin/codesign_allocate
    xcrun codesign -f -s $(SIGNING_IDENTITY) --preserve-metadata=identifier,entitlements --timestamp=none enigma
    
