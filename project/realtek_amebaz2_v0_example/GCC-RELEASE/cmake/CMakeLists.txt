cmake_minimum_required(VERSION 3.6)

project(flash_bin)

# root of realtek_amebapro2_v0_example
set (prj_root "${CMAKE_CURRENT_SOURCE_DIR}/../..")
# root of SDK
set (sdk_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set (matter_platform_8710c $ENV{MATTER_PLATFORM_8710C})
set (ameba_matter_root $ENV{AMEBA_MATTER})
set (matter_output_path $ENV{MATTER_OUTPUT})
set (matter_example_path $ENV{MATTER_EXAMPLE_PATH})
set (matter_config_path $ENV{MATTER_CONFIG_PATH})
set (matter_enable_rpc $ENV{MATTER_ENABLE_RPC})
set (matter_enable_ota_requestor $ENV{MATTER_ENABLE_OTA_REQUESTOR})
set (matter_generate_ota_image $ENV{MATTER_GENERATE_OTA_IMAGE})

set (BUILD_TZ OFF)

# App selection macros OFF by default, enable through command line argument in build.sh, Eg: -DMATTER_ALL_CLUSTERS_APP=ON
option(MATTER_ALL_CLUSTERS_APP "Build All-Clusters-App" OFF)
option(MATTER_LIGHTING_APP "Build Lighting-App" OFF)
option(MATTER_PIGWEED_APP "Build Pigweed-App" OFF)
option(MATTER_OTA_REQUESTOR_APP "Build OTA-Requestor-App" OFF)
option(MATTER_OTA_PROVIDER_APP "Build OTA-Provider-App" OFF)

message(STATUS "Build TZ ${BUILD_TZ}")

#add_subdirectory (bootloader)
if(BUILD_TZ)
	set(target flash_tz)
	set(fw application_tz/firmware_tz.bin)
	set(bttrace application_tz/APP.trace)
	set(app_dep application.s application.ns)
	add_subdirectory (application_tz)
else()
	set(target flash_ntz)
	set(fw application_ntz/firmware_ntz.bin)
#	set(bttrace application_ntz/APP.trace)
	set(app_dep application_ntz)
	add_subdirectory (application_ntz)
endif()



####################
IF(FALSE)
add_custom_target(
	flash

#	COMMAND cp ${prj_root}/GCC-RELEASE/keycfg.json keycfg.json
#	COMMAND cp ${prj_root}/GCC-RELEASE/partition.json partition.json
#	COMMAND cp ${prj_root}/GCC-RELEASE/amebapro2_partition.json amebapro2_partition.json
#	COMMAND ${prj_root}/GCC-RELEASE/elf2bin.exe keygen keycfg.json

#	COMMAND ${prj_root}/GCC-RELEASE/elf2bin.exe convert amebapro2_partition.json PARTITIONTABLE
	COMMAND cp bootloader/boot.bin .

	COMMAND cp ${fw} ./firmware.bin
	COMMAND ${prj_root}/GCC-RELEASE/elf2bin.exe combine ${target}.bin PTAB=partition.bin,BOOT=boot.bin,FW1=firmware.bin

#	COMMAND cp ${bttrace} ./APP.trace

	DEPENDS bootloader
	DEPENDS ${app_dep}
)
ENDIF()
