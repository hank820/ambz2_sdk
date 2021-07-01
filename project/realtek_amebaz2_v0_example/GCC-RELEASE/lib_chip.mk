
SHELL = /bin/bash

# Initialize tool chain
# -------------------------------------------------------------------
BASEDIR := $(shell pwd)
AMEBAZ2_TOOLDIR	= $(BASEDIR)/../../../component/soc/realtek/8710c/misc/iar_utility
CHIPDIR = $(BASEDIR)/../../../../connectedhomeip
OUTPUT_DIR = $(CHIPDIR)/examples/all-clusters-app/ambd/build/chip

OS := $(shell uname)

CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

OS := $(shell uname)

# Initialize target name and target object files
# -------------------------------------------------------------------

OBJ_DIR=$(TARGET)/Debug/obj
BIN_DIR=$(TARGET)/Debug/bin
INFO_DIR=$(TARGET)/Debug/info

# Include folder list
# -------------------------------------------------------------------

INCLUDES =
INCLUDES += -I$(BASEDIR)/../inc

INCLUDES += -I$(BASEDIR)/../../../component/common/api
INCLUDES += -I$(BASEDIR)/../../../component/common/api/at_cmd
INCLUDES += -I$(BASEDIR)/../../../component/common/api/platform
INCLUDES += -I$(BASEDIR)/../../../component/common/api/wifi
INCLUDES += -I$(BASEDIR)/../../../component/common/api/wifi/rtw_wpa_supplicant/src
INCLUDES += -I$(BASEDIR)/../../../component/common/api/wifi/rtw_wpa_supplicant/src/crypto
INCLUDES += -I$(BASEDIR)/../../../component/common/api/network/include
INCLUDES += -I$(BASEDIR)/../../../component/common/application
INCLUDES += -I$(BASEDIR)/../../../component/common/application/mqtt/MQTTClient
INCLUDES += -I$(BASEDIR)/../../../component/common/example
INCLUDES += -I$(BASEDIR)/../../../component/common/file_system
INCLUDES += -I$(BASEDIR)/../../../component/common/file_system/dct
INCLUDES += -I$(BASEDIR)/../../../component/common/file_system/fatfs
INCLUDES += -I$(BASEDIR)/../../../component/common/file_system/fatfs/r0.10c/include
INCLUDES += -I$(BASEDIR)/../../../component/common/file_system/ftl
INCLUDES += -I$(BASEDIR)/../../../component/common/utilities
INCLUDES += -I$(BASEDIR)/../../../component/common/mbed/hal
INCLUDES += -I$(BASEDIR)/../../../component/common/mbed/hal_ext
INCLUDES += -I$(BASEDIR)/../../../component/common/mbed/targets/hal/rtl8710c
INCLUDES += -I$(BASEDIR)/../../../component/common/network
INCLUDES += -I$(BASEDIR)/../../../component/common/network/coap/include
INCLUDES += -I$(BASEDIR)/../../../component/common/network/http2/nghttp2-1.31.0/includes
INCLUDES += -I$(BASEDIR)/../../../component/common/network/lwip/lwip_v2.1.2/src/include
INCLUDES += -I$(BASEDIR)/../../../component/common/network/lwip/lwip_v2.1.2/src/include/lwip
INCLUDES += -I$(BASEDIR)/../../../component/common/network/lwip/lwip_v2.1.2/port/realtek
INCLUDES += -I$(BASEDIR)/../../../component/common/network/lwip/lwip_v2.1.2/port/realtek/freertos
#INCLUDES += -I$(BASEDIR)/../../../component/common/network/ssl/mbedtls-2.4.0/include
#INCLUDES += -I$(BASEDIR)/../../../component/common/network/ssl/ssl_ram_map/rom
INCLUDES += -I$(BASEDIR)/../../../component/common/drivers/wlan/realtek/include
INCLUDES += -I$(BASEDIR)/../../../component/common/drivers/wlan/realtek/src/osdep
INCLUDES += -I$(BASEDIR)/../../../component/common/drivers/wlan/realtek/src/core/option
INCLUDES += -I$(BASEDIR)/../../../component/common/test

INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/cmsis/rtl8710c/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/cmsis/rtl8710c/lib/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/fwlib/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/fwlib/lib/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/cmsis/cmsis-core/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/app/rtl_printf/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/app/shell
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/app/stdio_port
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/misc/utilities/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/mbed-drivers/include
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/misc/platform
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/misc/driver
INCLUDES += -I$(BASEDIR)/../../../component/soc/realtek/8710c/misc/os

INCLUDES += -I$(BASEDIR)/../../../component/os/freertos
INCLUDES += -I$(BASEDIR)/../../../component/os/freertos/freertos_v10.0.1/Source/include
INCLUDES += -I$(BASEDIR)/../../../component/os/freertos/freertos_v10.0.1/Source/portable/GCC/ARM_RTL8710C
INCLUDES += -I$(BASEDIR)/../../../component/os/os_dep/include

# CHIP Include folder list
# -------------------------------------------------------------------
INCLUDES += -I$(CHIPDIR)/config/ambd
INCLUDES += -I$(CHIPDIR)/src/include/platform/AMBD
INCLUDES += -I$(CHIPDIR)/src/include
INCLUDES += -I$(CHIPDIR)/src/lib
INCLUDES += -I$(CHIPDIR)/src
INCLUDES += -I$(CHIPDIR)/src/system
INCLUDES += -I$(CHIPDIR)/src/app
INCLUDES += -I$(CHIPDIR)/third_party/mbedtls/repo/include
INCLUDES += -I$(CHIPDIR)/third_party/mbedtls/repo/include/mbedtls
INCLUDES += -I$(CHIPDIR)/third_party/nlassert/repo/include
INCLUDES += -I$(CHIPDIR)/third_party/nlio/repo/include
INCLUDES += -I$(CHIPDIR)/third_party/nlunit-test/repo/src

# Compile options
# -------------------------------------------------------------------

CFLAGS =
CFLAGS += -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=soft -D__thumb2__ -g -gdwarf-3 -Os
CFLAGS += -D__ARM_ARCH_8M_MAIN__=1 -gdwarf-3 -fstack-usage -fdata-sections -ffunction-sections 
CFLAGS += -fdiagnostics-color=always -Wall -Wpointer-arith -Wno-write-strings 
CFLAGS += -Wno-maybe-uninitialized --save-temps -c -MMD
CFLAGS += -DCONFIG_PLATFORM_8710C -DCONFIG_BUILD_RAM=1
CFLAGS += -DCONFIG_BUILD_LIB=1
CFLAGS += -DV8M_STKOVF
#for time64 
ifdef SYSTEM_TIME64_MAKE_OPTION
CFLAGS += -DCONFIG_SYSTEM_TIME64=1
CFLAGS += -include time64.h
else
CFLAGS += -DCONFIG_SYSTEM_TIME64=0
endif

# CHIP options
# -------------------------------------------------------------------
CFLAGS += -DCHIP_PROJECT=1
CFLAGS += -DCHIP_DEVICE_LAYER_TARGET=AMBD
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls/mbedtls_config.h\"

CFLAGS += -DLWIP_IPV6_ND=0
CFLAGS += -DLWIP_IPV6_SCOPES=0
CFLAGS += -DLWIP_PBUF_FROM_CUSTOM_POOLS=0
CFLAGS += -DLWIP_IPV6_ROUTE_TABLE_SUPPORT=1

CFLAGS += -DCHIP_DEVICE_LAYER_NONE=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_NET_IF=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_BSD_IFADDRS=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_SOCKET_EXTENSIONS=0

CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_LWIP=1
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_SOCKETS=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_NETWORK_FRAMEWORK=0

CXXFLAGS += -DFD_SETSIZE=10

CXXFLAGS += -Wno-sign-compare
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-deprecated-declarations
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wno-format

CXXFLAGS += -std=gnu++11
CXXFLAGS += -std=c++14
CXXFLAGS += -fno-rtti

CHIP_CFLAGS = $(CFLAGS)
CHIP_CFLAGS += $(INCLUDES)

CHIP_CXXFLAGS += $(CFLAGS)
CHIP_CXXFLAGS += $(CXXFLAGS)
CHIP_CXXFLAGS += $(INCLUDES)

#*****************************************************************************#
#                        RULES TO GENERATE TARGETS                            #
#*****************************************************************************#

# Define the Rules to build the core targets
all: GENERATE_NINJA

GENERATE_NINJA:
	echo "INSTALL CHIP..."
	echo $(BASEDIR)
	mkdir -p $(OUTPUT_DIR)
	echo                                   > $(OUTPUT_DIR)/args.gn
	echo "import(\"//args.gni\")"          >> $(OUTPUT_DIR)/args.gn
	echo target_cflags_c  = [$(foreach word,$(CHIP_CFLAGS),\"$(word)\",)] | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'  >> $(OUTPUT_DIR)/args.gn
	echo target_cflags_cc = [$(foreach word,$(CHIP_CXXFLAGS),\"$(word)\",)] | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'   >> $(OUTPUT_DIR)/args.gn
	echo ambd_ar = \"arm-none-eabi-ar\"    >> $(OUTPUT_DIR)/args.gn
	echo ambd_cc = \"arm-none-eabi-gcc\"   >> $(OUTPUT_DIR)/args.gn
	echo ambd_cxx = \"arm-none-eabi-c++\"  >> $(OUTPUT_DIR)/args.gn
	echo ambd_cpu = \"ambd\"               >> $(OUTPUT_DIR)/args.gn
	cd $(CHIPDIR) && PW_ENVSETUP_QUIET=1 . scripts/activate.sh
	mkdir -p $(CHIPDIR)/config/ambd/components/chip
	cd $(CHIPDIR)/config/ambd/components/chip && gn gen --check --fail-on-unused-args $(CHIPDIR)/examples/all-clusters-app/ambd/build/chip
	cd $(CHIPDIR)/config/ambd/components/chip ; ninja -C $(CHIPDIR)/examples/all-clusters-app/ambd/build/chip
	cp -f $(OUTPUT_DIR)/lib/* $(BASEDIR)/../../../component/soc/realtek/8710c/misc/bsp/lib/common/GCC
#*****************************************************************************#
#              CLEAN GENERATED FILES                                          #
#*****************************************************************************#
.PHONY: clean
clean:
	echo "RM $(OUTPUT_DIR)"
	rm -rf $(OUTPUT_DIR)

