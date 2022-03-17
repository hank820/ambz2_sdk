#!/usr/bin/env bash

#   ./build.sh -g -r {MATTER DIR} {BUILD METHOD} {OUTPUT DIR} {APP_NAME} 
#   no trailing '/' for MATTER_DIR and OUTPUT_DIR
#   -g and -r are optional flags, input them as the first two arguments
#       -g: Generate MATTER_OTA_ALL.bin
#       -r: Enable RPC

while getopts :gr opt; do
    case $opt in
        g) export MATTER_GENERATE_OTA_IMAGE=1 ;;
        r) export MATTER_ENABLE_RPC=1 ;;
        :) echo "Missing argument for option -$OPTARG"; exit 1;;
       \?) echo "Unknown option -$OPTARG"; exit 1;;
    esac
done
shift $(( OPTIND - 1 ))

BUILD_FILE_DIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
CMAKE_ROOT=$BUILD_FILE_DIR/cmake
SDK_ROOT=$BUILD_FILE_DIR/../../..
TOOL_DIR=$SDK_ROOT/tools/
LP_IMAGE=$BUILD_FILE_DIR/project_lp/asdk/image
HP_IMAGE=$BUILD_FILE_DIR/project_hp/asdk/image

echo $BUILD_FILE_DIR
echo $CMAKE_ROOT
echo $SDK_ROOT

echo Extracting Toolchain...;

if [ ! -f $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2 ]; then
    cat $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2* > $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2;
fi

if [ ! -d $TOOL_DIR/arm-none-eabi-gcc/asdk ]; then
    mkdir $TOOL_DIR/arm-none-eabi-gcc/asdk; 
fi

if [ ! -d $TOOL_DIR/arm-none-eabi-gcc/asdk/linux ]; then
    tar -jxf $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2 -C $TOOL_DIR/arm-none-eabi-gcc/; 
    mv $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0/linux $TOOL_DIR/arm-none-eabi-gcc/asdk/linux ; 
    rmdir $TOOL_DIR/arm-none-eabi-gcc/asdk-10.3.0; 
fi

echo Toolchain Extraction Done!
echo Toolchain is found at $TOOL_DIR/arm-none-eabi-gcc/asdk/linux

## AMEBAZ2 build
export MATTER_PLATFORM_8710C=1

## AMEBA_MATTER to be exported or manually keyed in.
if [ ! -z ${AMEBA_MATTER} ]; then
    echo "Matter SDK is located at: ${AMEBA_MATTER}"
elif [ -d "$1" ]; then
    echo "Matter SDK is located at: "$1""
    export AMEBA_MATTER="$1"
else
    echo "Error: Unknown path for Matter SDK."
    exit
fi

export MATTER_CONFIG_PATH=${AMEBA_MATTER}/config/ameba

APP_SELECT="$4"
echo "********** You are building $APP_SELECT **********"

if [ $APP_SELECT == "all-clusters-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "lighting-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/lighting-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "pigweed-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/pigweed-app/ameba
elif [ $APP_SELECT == "ota-provider-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-provider-app/ameba
elif [ $APP_SELECT == "ota-requestor-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-requestor-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
else
    echo "$APP_SELECT not found, building all-clusters-app instead";
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
fi
echo "MATTER_EXAMPLE_PATH at: ${MATTER_EXAMPLE_PATH}"

## Check output directory
if [ ! -z "$3" ]; then
    export MATTER_OUTPUT="$3"
    mkdir -p "$MATTER_OUTPUT"
fi
cd "$MATTER_OUTPUT"

function exe_cmake()
{
	if [ $APP_SELECT == "all-clusters-app" ]; then
	    exe_cmake_all
	elif [ $APP_SELECT == "lighting-app" ]; then
	    exe_cmake_light
	elif [ $APP_SELECT == "pigweed-app" ]; then
	    exe_cmake_pigweed
	elif [ $APP_SELECT == "ota-requestor-app" ]; then
	    exe_cmake_otar
	elif [ $APP_SELECT == "ota-provider-app" ]; then
	    exe_cmake_otap
	else
	    exe_cmake_all
	fi
}

function exe_cmake_all()
{
	echo "Build all"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_ALL_CLUSTERS_APP=ON
}

function exe_cmake_light()
{
	echo "Build light"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_LIGHTING_APP=ON
}

function exe_cmake_pigweed()
{
	echo "Build pigweed"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_PIGWEED_APP=ON
}

function exe_cmake_otar()
{
	echo "Build OTA-R"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_REQUESTOR_APP=ON
}

function exe_cmake_otap()
{
	echo "Build OTA-P"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_PROVIDER_APP=ON
}

## Decide meta build method
if [[ "$2" == "ninja" || "$2" == "Ninja" ]]; then
	BUILD_METHOD="Ninja"
	exe_cmake
	#ninja
else
	BUILD_METHOD="Unix Makefiles"
    exe_cmake
	#make
fi

: '

## Copy bootloaders
if [ ! -d "$MATTER_OUTPUT/asdk/bootloader" ]; then
        mkdir -p $MATTER_OUTPUT/asdk/bootloader
fi

if [ -a "$LP_IMAGE/km0_boot_all.bin" ]; then
    cp $LP_IMAGE/km0_boot_all.bin $MATTER_OUTPUT/asdk/bootloader/km0_boot_all.bin
else
    echo "Error: km0_boot_all.bin can not be found."
fi

if [ -a "$HP_IMAGE/km4_boot_all.bin" ]; then
    cp $HP_IMAGE/km4_boot_all.bin $MATTER_OUTPUT/asdk/bootloader/km4_boot_all.bin
else
    echo "Error: km4_boot_all.bin can not be found."
fi

'
