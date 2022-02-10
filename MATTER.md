# Matter (previously CHIP) on AmebaZ2

## How to support Matter on Ameba SDK

https://github.com/hank820/ambz2_sdk/blob/matter/MATTER_PORTING.md

## Get Ameba SDK & Matter SDK

    Test on Ubuntu 20.04

To check out this repository:

    git clone -b matter --recurse-submodules https://github.com/hank820/ambz2_sdk.git

If you already have a checkout, run the following command to sync submodules recursively:

	git submodule update --init --recursive


## Set Matter Build Environment 

    cd third_party/connectedhomeip

    source scripts/bootstrap.sh

    source scripts/activate.sh

    > Find more details to setup linux build environment
    > https://github.com/project-chip/connectedhomeip/blob/master/docs/guides/BUILDING.md


## Make CHIP library by gn and Make lib_main.a
### all-cluster-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh
	
    make lib_all

### lighting-app

    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    source ToolPath.sh

    make light

### CHIP core (generate by GN/ninja in connectedhomeip. Config by [lib_chip.mk](https://github.com/hank820/ambz2_sdk/blob/matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip.mk))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC
	
    > libCHIP.a

### CHIP application (generate by [lib_chip_main.mk](https://github.com/hank820/ambz2_sdk/blob/matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/lib_chip_main.mk)))

    output : ambz2_sdk/component/soc/realtek/8710c/misc/bsp/lib/common/GCC

    > lib_main.a

## Make Ameba application
    cd ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE

    make is -j4

## Flash Image
    ambz2_sdk/project/realtek_amebaz2_v0_example/GCC-RELEASE/application_is/Debug/bin/flash_is.bin

Find more detail in [application_note](https://github.com/hank820/ambz2_sdk/blob/matter/doc/AN0500%20Realtek%20Ameba-ZII%20application%20note.en.pdf) Chapter4

## Run CHIP task on AmebaZ2 (all-cluster-app/lighting-app example)

	Matter task will auto start after reset the device.




## Test with [chip-tool](https://github.com/hank820/connectedhomeip/tree/master/examples/chip-tool)
Use standalone chip-tool app(linux) to communicate with the device.

In order to send commands to a device, it must be commissioned with the client. chip-tool currently only supports commissioning and remembering one device at a time. The configuration state is stored in `/tmp/chip_tool_config.ini`; deleting this and other `.ini` files in `/tmp` can sometimes resolve issues due to stale configuration.

### Commission a device over BLE

* Run CHIP-TOOL IP commissioning command `./chip-tool pairing ble-wifi ${NODE_ID_TO_ASSIGN} ${SSID} ${PASSWORD} 20202021 3840`
* For example: `./chip-tool pairing ble-wifi 12344321 testssid password 20202021 3840`

### Pair a device over IP

* Connect to AP using after matter task by `ATW0, ATW1, ATWC` commands
* Run CHIP-TOOL IP commissioning command `./chip-tool pairing onnetwork ${NODE_ID_TO_ASSIGN} 20202021`
* For example: `./chip-tool pairing onnetwork 12344321 20202021`

### Command for onoff cluster

Use PA_20 as output, connect a LED to this pin and GND.

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff on 12344321 1`

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff off 12344321 1`


Find more detail in [chip-tool](https://github.com/hank820/connectedhomeip/tree/master/examples/chip-tool)

## Test with [Python Controller](https://github.com/hank820/connectedhomeip/blob/master/docs/guides/python_chip_controller_building.md)
To build the Python Controller (linux), run the following command.

	./scripts/build_python.sh --chip_mdns platform

To launch Python Controller, activate the python environment first.
	
	source out/python_env/bin/activate
	chip-device-ctrl

### Commission a device over BLE
* Power on device and wait matter task run
* Run python controller BLE commissioning command `chip-device-ctrl > connect -ble 3840 20202021 135246`
* Provide network credentials `chip-device-ctrl > zcl NetworkCommissioning AddWiFiNetwork 135246 0 0 ssid=str:TESTSSID credentials=str:TESTPASSWD breadcrumb=0 timeoutMs=1000`
* Connect to AP `chip-device-ctrl > zcl NetworkCommissioning EnableNetwork 135246 0 0 networkID=str:TESTSSID breadcrumb=0 timeoutMs=1000`
* Close the BLE connection `chip-device-ctrl > close-ble`
* Resolve DNS-SD name and update address of the node in the device controller. `chip-device-ctrl > resolve 135246`

* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`

### Pair a device over IP
* Power on device and wait matter task run
* Connect to AP using `ATW0, ATW1, ATWC` commands
* Run python controller IP commissioning command `chip-device-ctrl > connect -ip <IP> 20202021 135246`
* Resolve DNS-SD name and update address of the node in the device controller. `chip-device-ctrl > resolve 135246`
* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`



## Test with [Android Chip-Tool](https://github.com/hank820/connectedhomeip/tree/master/src/android/CHIPTool)

### Install Android Studio
* Download [Android Studio](https://developer.android.com/studio?gclid=Cj0KCQjwkIGKBhCxARIsAINMioLrotRhWXySwLrveax_JTX-qt1H-zjyGBEZk2FtseA-SYmQYmpq9M4aApGsEALw_wcB&gclsrc=aw.ds#downloads) and [Android-NDK-r21e](https://github.com/android/ndk/wiki/Unsupported-Downloads) and extract the files.
* Launch Android Studio: `cd android-studio/bin` and run `./studio.sh`
* Go to **Customize** and **All settings**. Under **Appearance & Behaviour** >> **System Settings** >> **Android SDK**, Check **Android 5.0 (Lollipop)** and uncheck the rest. Default Android SDK location is at `~/Android/Sdk`.

### Preparing for build

Run the following commands

	cd connectedhomeip/
	source script/bootstrap.sh
	export ANDROID_HOME=~/Android/Sdk
	export ANDROID_NDK_HOME={path to android-ndk-r21e}
	
Target CPU
`TARGET_CPU` can have the following values, depending on your smartphone CPU
architecture:

| ABI         | TARGET_CPU |
| ----------- | ---------- |
| armeabi-v7a | arm        |
| arm64-v8a   | arm64      |
| x86         | x86        |
| x86_64      | x64        |

Complete the following steps to prepare the Matter build:

1. Check out the Matter repository.

2. Run bootstrap (**only required first time**)

    ```shell
    source scripts/bootstrap.sh
    ```

3. Choose how you want to build the Android CHIPTool. There are **two** ways:
   from script, or from source within Android Studio.

<a name="building-scripts"></a>

### Building Android CHIPTool from scripts

This is the simplest option. In the command line, run the following command from
the top CHIP directory:

```shell
./scripts/build/build_examples.py --target android-arm64-chip-tool build
```

See the table above for other values of `TARGET_CPU`.

The debug Android package `app-debug.apk` will be generated at
`out/android-$TARGET_CPU-chip-tool/outputs/apk/debug/`, and can be installed
with

```shell
adb install out/android-$TARGET_CPU-chip-tool/outputs/apk/debug/app-debug.apk
```

You can use Android Studio to edit the Android CHIPTool app itself and run it
after build_examples.py, but you will not be able to edit Matter Android code
from `src/controller/java`, or other Matter C++ code within Android Studio.

<a name="building-studio"></a>

### Building Android CHIPTool from Android Studio

This option allows Android Studio to build the core Matter code from source,
which allows us to directly edit core Matter code in-IDE.

1. In the command line, run the following command from the top Matter directory:

    ```shell
    TARGET_CPU=arm64 ./scripts/examples/android_app_ide.sh
    ```

    See the table above for other values of `TARGET_CPU`.

2. Modify the `matterSdkSourceBuild` variable to true, `matterBuildSrcDir` point
   to the appropriate output directory (e.g. `../../../../out/android_arm64`),
   and `matterSourceBuildAbiFilters` to the desired ABIs in
   [src/android/CHIPTool/gradle.properties](https://github.com/project-chip/connectedhomeip/blob/master/src/android/CHIPTool/gradle.properties)

3) Open the project in Android Studio and run **Sync Project with Gradle
   Files**.

4) Use one of the following options to build an Android package:

    - Click **Make Project** in Android Studio.
    - Run the following command in the command line:

        ```shell
        cd src/android/CHIPTool
        ./gradlew build
        ```

The debug Android package `app-debug.apk` will be generated at
`src/android/CHIPTool/app/build/outputs/apk/debug/`, and can be installed with

```shell
adb install src/android/CHIPTool/app/build/outputs/apk/debug/app-debug.apk
```

or

```shell
(cd src/android/CHIPTool && ./gradlew installDebug)
```

### BLE Commissioning
* Power on device and wait matter task run
* Launch Android Chip-Tool app and press **Provision CHIP Device With Wi-Fi**
* Scan the QR Code and enter the network credentials of the AP
