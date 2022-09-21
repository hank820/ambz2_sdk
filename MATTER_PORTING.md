# How to support Matter on AmebaD SDK

1. Add lwip_v2.1.2 [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/ee92575a388828695efa9667d5216da4b9212c4d)

	Just copy full folder into your project

2. Build libCHIP.a [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/6f0c5282fe4d33e4ce6c43d6aa7523de9c844987)

	2.1 Add       component/common/utilities/chip_porting.c
	
	2.2 Add       component/common/utilities/chip_porting.h
	
	2.3	Add       project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk
	
	<b>2.4 Delete component/soc/realtek/8710c/misc/utilities/include/ctype.h</b>
	
	2.5	Modified  component/common/api/network/include/lwipopts.h
	
		=> enable LWIP_IPV6
			
	2.6	Modified  component/soc/realtek/8710c/app/rtl_printf/include/diag.h
	
		=> change u32 to uint32_t
			
	2.7	Modified  component/soc/realtek/8710c/cmsis/rtl8710c/include/basic_types.h
	
		=> comment out "typedef unsigned char bool;"
			
		=> Add #if CHIP_PROJECT ... #endif, fix conflict data type in Matter/Ameba SDK
			
	2.8	Modified  component/soc/realtek/8710c/misc/utilities/include/utility.h
	
		=> change u32 to uint32_t
		
	2.9	Modified  project/realtek_amebaz2_v0_example/GCC-RELEASE/Makefile
	
		=> Add lib_chip.mk to build libCHIP.a
	
3. Fix compile error when make is [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/ed407068eb59545e6b854afa8435b5343b270117) 

	3.1 Modified component/common/api/at_cmd/atcmd_mp_ext2.c
	
	Modified component/common/file_system/ftl/ftl.c
		
	Modified component/common/file_system/ftl/ftl.h
		
	Modified component/common/mbed/targets/hal/rtl8710c/rtc_api.c
		
	Modified component/common/utilities/chip_porting.c
		
	Modified component/soc/realtek/8710c/misc/platform/ota_8710c.c
		
	Modified component/soc/realtek/8710c/misc/platform/ota_8710c.h
		
		=> change bool to BOOL
			
	3.2 Modified component/common/api/at_cmd/atcmd_wifi.c
	
	Modified component/common/api/lwip_netconf.c
		
	Modified component/common/api/lwip_netconf.h
		
		=> Merge "#if LWIP_VERSION_MAJOR >= 2 && LWIP_VERSION_MINOR >= 1" related parts. 
			
	3.3 Modified component/common/api/network/include/lwipopts.h
	
		=> Add define for IPv6
			
		=> #define LWIP_IPV6_MLD                   1
			
		=> #define LWIP_IPV6_AUTOCONFIG            1
			
		=> #define LWIP_ICMP6                      1
			
		=> #define LWIP_IPV6_DHCP6                 1
			
	3.4 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/application.is.mk
	
		=> Compare and merge manually
			
		=> Add Matter path
			
		=> Add chip_porting.c into Makefile
			
		=> Add CPP part
			
		=> Change lwip2.0.2 to lwip2.1.2
			
		=> Change mbedtls2.4.0 to Matter version
			

4. Build lib_main.a [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/618864e50749a154a8fadb8e8cd9ea6d6deb0b32)

	4.1 Add      project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk
	
	4.2 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/Makefile
	
		=> Add lib_chip_main.mk

5. Link libCHIP.a & lib_main.a [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/aab45b10c23c59e171c8fb6b6cdd050cca971b2b)

	5.1 Modified component/common/api/at_cmd/atcmd_sys.c
	
		=> Add ATS$ with fATchipapp.
			
	5.2 Modified component/common/utilities/chip_porting.c
	
		=> Comment out not support function
			
	5.3 Modified component/soc/realtek/8710c/cmsis/rtl8710c/source/ram_s/app_start.c
	
		=> Add _fini for support C++ 
			
	5.4 Modified component/soc/realtek/8710c/misc/utilities/source/ram/libc_wrap.c
	
		=> Comment out __dso_handle to fix redefine
			
	5.5 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/application.is.mk
	
		=> Change toolchain path
			
		=> Link lib_main, liBCHIP.a, libstdc++	
			
	5.6 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/rtl8710c_ram.ld
	
		=> Merge manually for C++ support
			
		=> Find "Add This for C++ support"

6. Revise makefile [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/be1bb5bcb8573ae4e04648e078f1555841541ed6)

	6.1 Modified component/common/utilities/chip_porting.c
	
		=> comment out and return TRUE with not support function (initPref, clearPref)
			
	6.2 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/application.is.mk
	
		=> Revise link lib_main, liBCHIP.a, libstdc++
			
	6.3 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk
	
		=> Add "CFLAGS += -DINET_CONFIG_ENABLE_IPV4=1"
			
	6.4 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk
	
		=> Add "CFLAGS += -DCHIP_SYSTEM_CONFIG_POSIX_LOCKING=0"
			
		=> Add "CFLAGS += -DINET_CONFIG_ENABLE_IPV4=1"

7. Add toolchain 9.3.0 [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/e987b418cd8e752451143696b3a6900c6dfc9d70)

	7.1 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/Makefile
	
	Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/toolchain.mk
		
		=> change toolchain version
			
	7.2 Add      tools/arm-none-eabi-gcc/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partaa
	
	Add      tools/arm-none-eabi-gcc/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partab
		
	Add      tools/arm-none-eabi-gcc/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partac
		
	Add      tools/arm-none-eabi-gcc/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partad
		
	Add      tools/arm-none-eabi-gcc/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partae

8. Fix Failed to initialize Udp transport: LwIP Error [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/09dbdc4395475ca2be864a313fddfd37373f0d73)

	8.1 Modified component/common/api/network/include/lwipopts.h
	
		=> Change define
			
		=> #define LWIP_TCPIP_CORE_LOCKING         1
			
		=> #define LWIP_COMPAT_MUTEX_ALLOWED       1
			
		=> #define LWIP_IPV6_ND                    0
			
		=> #define LWIP_IPV6_SCOPES                0
			
		=> #define LWIP_PBUF_FROM_CUSTOM_POOLS     0
			
		=> #define ERRNO                           1
			
		=> #define LWIP_SO_SNDTIMEO                1
			
		=> #define LWIP_SOCKET_SET_ERRNO           1
			
9. Add __libc_init_array to support C++ constructors [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/ed3600b075bcad84996c3a333db7e900f4b4e32a)

	9.1 Modified component/soc/realtek/8710c/cmsis/rtl8710c/source/ram_s/app_start.c
	
		=> Add __libc_init_array(); for C++ Support

10. Fix chip[CR] mbedTLS error: -0xFFFF5180 when start CHIP task [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/1fc71bcc156fad91a635e428839370f4e09c0bf2)

	10.1 Modified project/realtek_amebaz2_v0_example/src/main.c
	
		=> Add mbedtls_platform_set_calloc_free

11. Fix chip[CSL] PacketBuffer: allocation too large [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/4eb9d24c6f3af42d5ded66df837888e8de11ddaf)

	11.1 Modified component/common/api/network/include/lwipopts.h
	
		=> Enlarge PBUF_POOL_BUFSIZE from 500 to 1280
		
12. Add connectedhomeip as submodule. Base0624 [[ref commit]](https://github.com/hank820/ambz2_sdk/commit/0b3c49cd52ac12e8bc408316ec3d603477b22c5b)

	12.1 Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/application.is.mk
	
	Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk
		
	Modified project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk
		
		=> Change path of connectedhomeip SDK
			
	12.2 Add      third_party/connectedhomeip
	
		=> submodule 
			
