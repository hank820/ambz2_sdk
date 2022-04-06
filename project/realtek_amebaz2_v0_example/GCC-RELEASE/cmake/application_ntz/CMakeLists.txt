cmake_minimum_required(VERSION 3.6)

find_package(Python3 REQUIRED)

project(app_ntz)

enable_language(C ASM)

set(prj_root_tmp "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
set(sdk_root_tmp "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
get_filename_component(prj_root ${prj_root_tmp} REALPATH)
get_filename_component(sdk_root ${sdk_root_tmp} REALPATH)
set(amebaz2_gcctooldir "${sdk_root}/component/soc/realtek/8710c/misc/gcc_utility")
set(amebaz2_tooldir "${sdk_root}/component/soc/realtek/8710c/misc/iar_utility")
set(amebaz2_bootloaderdir "${sdk_root}/component/soc/realtek/8710c/misc/bsp/image")
set(amebaz2_romsymdir "${sdk_root}/component/soc/realtek/8710c/misc/bsp/ROM")
#set(km0_image "${prj_root}/GCC-RELEASE/project_lp/asdk/image")
set(prj_hp "${prj_root}/GCC-RELEASE/project_hp")
set(app_ntz application.ntz)
set(app_ntz_pure application.ntz_pure)
set(list app_ntz_sources)
set(list_inc_path app_ntz_inc)
set(list_c_flags app_ntz_flags)
set(list_code2sram "")
#set(list_code2sram_network code2sram_network)

# set elf2bin and chksum
set(ELF2BIN ${amebaz2_gcctooldir}/elf2bin.linux)
set(CHKSUM ${amebaz2_gcctooldir}/checksum.linux)


set(freertos "freertos_v10.2.0")
set(lwip "lwip_v2.1.2")

set(utility_dir "${prj_hp}/asdk/gnu_utility")
set(prepend_tool "${utility_dir}/prepend_header.sh")
set(pad_tool "${utility_dir}/pad.sh")

#include(config.cmake)
include(../includepath.cmake)
#include(make/make.cmake)

if(DEFINED ameba_matter_root)
	include(${ameba_matter_root}/src/app/chip_data_model.cmake)
	include(${matter_example_path}/chip_main.cmake)
    include(${matter_config_path}/chip.cmake)
else()
    message(FATAL_ERROR "Matter path is not specified, CMake will exit")
endif()

add_dependencies(chip_main chip-gn)

################################ Source file list #############################
#bluetooth - board
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/hci/bt_fwconfig.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/hci/bt_mp_patch.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/hci/bt_normal_patch.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/src/cycle_queue.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/data_uart/data_uart.c
    ${sdk_root}/component/common/file_system/ftl/ftl.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/hci/hci_board.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/hci/hci_uart.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/os/freertos/osif_freertos.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/platform_utils.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/rtk_coex.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/trace_uart.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/src/vendor_cmd/vendor_cmd.c
)

#bluetooth - common
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/client/ancs_client.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/server/bas.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/client/bas_client.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/src/bt_uart_bridge.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/server/dis.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/client/gaps_client.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/src/hci_adapter.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/src/hci_process.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/server/hids.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/client/simple_ble_client.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/server/simple_ble_service.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/board/common/src/trace_task.c
)

#bluetooth - example - ble_central
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/ble_central_app_main.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/ble_central_app_task.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/ble_central_at_cmd.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/ble_central_client_app.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/ble_central_link_mgr.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_central/user_cmd.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/ble/profile/client/gcs_client.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/src/mcu/module/data_uart_cmd/user_cmd_parse.c
)

#bluetooth - example - ble_peripheral
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_peripheral/app_task.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_peripheral/ble_app_main.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_peripheral/ble_peripheral_at_cmd.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_peripheral/peripheral_app.c
)

#bluetooth - example - ble_scatternet
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_scatternet/ble_scatternet_app.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_scatternet/ble_scatternet_app_main.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_scatternet/ble_scatternet_app_task.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_scatternet/ble_scatternet_link_mgr.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/ble_scatternet/ble_scatternet_user_cmd.c
)

#bluetooth - example - bt_config
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_config/bt_config_app_main.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_config/bt_config_app_task.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_config/bt_config_peripheral_app.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_config/bt_config_service.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_config/bt_config_wifi.c
)

#bluetooth - example - bt_matter_adapter
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter/bt_matter_adapter_app_main.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter/bt_matter_adapter_app_task.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter/bt_matter_adapter_peripheral_app.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter/bt_matter_adapter_service.c
    ${sdk_root}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter/bt_matter_adapter_wifi.c
)

#bluetooth - example entry
list(
    APPEND app_ntz_sources
    ${sdk_root}/component/common/bluetooth/realtek/sdk/bt_example_entry.c
)

#cmsis
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/soc/realtek/8710c/cmsis/rtl8710c/source/ram_s/app_start.c
    ${sdk_root}/component/soc/realtek/8710c/cmsis/rtl8710c/source/ram/mpu_config.c
    ${sdk_root}/component/soc/realtek/8710c/cmsis/rtl8710c/source/ram/sys_irq.c
)

#libc api wrapper
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/soc/realtek/8710c/misc/utilities/source/ram/libc_wrap.c
)

#console
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/api/at_cmd/atcmd_bt.c
    ${sdk_root}/component/common/api/at_cmd/atcmd_lwip.c
    ${sdk_root}/component/common/api/at_cmd/atcmd_mp.c
    ${sdk_root}/component/common/api/at_cmd/atcmd_mp_ext2.c
    ${sdk_root}/component/common/api/at_cmd/atcmd_sys.c
    ${sdk_root}/component/common/api/at_cmd/atcmd_wifi.c
    ${sdk_root}/component/soc/realtek/8710c/app/shell/cmd_shell.c
    ${sdk_root}/component/soc/realtek/8710c/app/shell/ram_s/consol_cmds.c
    ${sdk_root}/component/common/api/at_cmd/log_service.c
    ${sdk_root}/component/soc/realtek/8710c/misc/driver/rtl_console.c
)

#network - api
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/api/lwip_netconf.c
)

#network - api - wifi
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/api/wifi/wifi_conf.c
    ${sdk_root}/component/common/api/wifi/wifi_ind.c
    ${sdk_root}/component/common/api/wifi/wifi_promisc.c
    ${sdk_root}/component/common/api/wifi/wifi_simple_config.c
    ${sdk_root}/component/common/api/wifi/wifi_util.c
)

#network - api - wifi - rtw_wpa_supplicant
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/api/wifi/rtw_wpa_supplicant/src/crypto/tls_polarssl.c
    ${sdk_root}/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c
    ${sdk_root}/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_p2p_config.c
    ${sdk_root}/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_wps_config.c
)

#network - app
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/soc/realtek/8710c/misc/platform/ota_8710c.c
    ${sdk_root}/component/common/api/network/src/ping_test.c
    ${sdk_root}/component/common/utilities/ssl_client.c
    ${sdk_root}/component/common/utilities/ssl_client_ext.c
    ${sdk_root}/component/common/utilities/tcptest.c
    ${sdk_root}/component/common/api/network/src/wlan_network.c
)

#utilities
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/utilities/chip_porting.c
    ${sdk_root}/component/common/utilities/cJSON.c
    ${sdk_root}/component/common/utilities/http_client.c
    ${sdk_root}/component/common/utilities/xml.c
)

#network - app - mqtt
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/application/mqtt/MQTTClient/MQTTClient.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTConnectClient.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTConnectServer.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTDeserializePublish.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTFormat.c
    ${sdk_root}/component/common/application/mqtt/MQTTClient/MQTTFreertos.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTPacket.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTSerializePublish.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTSubscribeClient.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTSubscribeServer.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeClient.c
    ${sdk_root}/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeServer.c
)

#network - coap
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/coap/sn_coap_ameba_port.c
    ${sdk_root}/component/common/network/coap/sn_coap_builder.c
    ${sdk_root}/component/common/network/coap/sn_coap_header_check.c
    ${sdk_root}/component/common/network/coap/sn_coap_parser.c
    ${sdk_root}/component/common/network/coap/sn_coap_protocol.c
)

#network - http
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/httpc/httpc_tls.c
    ${sdk_root}/component/common/network/httpd/httpd_tls.c
)

#network
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/dhcp/dhcps.c
    ${sdk_root}/component/common/network/sntp/sntp.c
)

#network - lwip - api
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/api_lib.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/api_msg.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/err.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/netbuf.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/netdb.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/netifapi.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/sockets.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/api/tcpip.c
)

#network - lwip - core
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/def.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/dns.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/inet_chksum.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/init.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ip.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/mem.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/memp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/netif.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/pbuf.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/raw.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/stats.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/sys.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/tcp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/tcp_in.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/tcp_out.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/timeouts.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/udp.c
)

#network - lwip - core - ipv4
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/autoip.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/dhcp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/etharp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/icmp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/igmp.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/ip4.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/ip4_addr.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv4/ip4_frag.c
)

#network - lwip - core - ipv6
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/dhcp6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/ethip6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/icmp6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/inet6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/ip6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/ip6_addr.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/ip6_frag.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/mld6.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/core/ipv6/nd6.c
)

#network - lwip - netif
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/src/netif/ethernet.c
)

#network - lwip - port
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/port/realtek/freertos/ethernetif.c
    ${sdk_root}/component/common/drivers/wlan/realtek/src/osdep/lwip_intf.c
    ${sdk_root}/component/common/network/lwip/lwip_v2.1.2/port/realtek/freertos/sys_arch.c
)

#network - mdns
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/mDNS/mDNSPlatform.c
)

set(chip_mbedtls_dir "${ameba_matter_root}/third_party/mbedtls/repo/library")

#network - ssl - mbedtls
#merge 2.4.0/net_sockets.c because MBEDTLS_NET_C
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/ssl/mbedtls-matter/net_sockets.c
    ${chip_mbedtls_dir}/aes.c
    ${chip_mbedtls_dir}/aesni.c
    ${chip_mbedtls_dir}/arc4.c
    ${chip_mbedtls_dir}/aria.c
    ${chip_mbedtls_dir}/asn1parse.c
    ${chip_mbedtls_dir}/asn1write.c
    ${chip_mbedtls_dir}/base64.c
    ${chip_mbedtls_dir}/bignum.c
    ${chip_mbedtls_dir}/blowfish.c
    ${chip_mbedtls_dir}/camellia.c
    ${chip_mbedtls_dir}/ccm.c
    ${chip_mbedtls_dir}/certs.c
    ${chip_mbedtls_dir}/chacha20.c
    ${chip_mbedtls_dir}/chachapoly.c
    ${chip_mbedtls_dir}/cipher.c
    ${chip_mbedtls_dir}/cipher_wrap.c
    ${chip_mbedtls_dir}/cmac.c
    ${chip_mbedtls_dir}/ctr_drbg.c
    ${chip_mbedtls_dir}/debug.c
    ${chip_mbedtls_dir}/des.c
    ${chip_mbedtls_dir}/dhm.c
    ${chip_mbedtls_dir}/ecdh.c
    ${chip_mbedtls_dir}/ecdsa.c
    ${chip_mbedtls_dir}/ecjpake.c
    ${chip_mbedtls_dir}/ecp.c
    ${chip_mbedtls_dir}/ecp_curves.c
    ${chip_mbedtls_dir}/entropy.c
    ${chip_mbedtls_dir}/entropy_poll.c
    ${chip_mbedtls_dir}/error.c
    ${chip_mbedtls_dir}/gcm.c
    ${chip_mbedtls_dir}/havege.c
    ${chip_mbedtls_dir}/hkdf.c
    ${chip_mbedtls_dir}/hmac_drbg.c
    ${chip_mbedtls_dir}/md2.c
    ${chip_mbedtls_dir}/md4.c
    ${chip_mbedtls_dir}/md5.c
    ${chip_mbedtls_dir}/md.c
    #${chip_mbedtls_dir}/md_wrap.c
    ${chip_mbedtls_dir}/memory_buffer_alloc.c
    #${chip_mbedtls_dir}/net_sockets.c
    ${chip_mbedtls_dir}/nist_kw.c
    ${chip_mbedtls_dir}/oid.c
    ${chip_mbedtls_dir}/padlock.c
    ${chip_mbedtls_dir}/pem.c
    ${chip_mbedtls_dir}/pk.c
    ${chip_mbedtls_dir}/pkcs11.c
    ${chip_mbedtls_dir}/pkcs12.c
    ${chip_mbedtls_dir}/pkcs5.c
    ${chip_mbedtls_dir}/pkparse.c
    ${chip_mbedtls_dir}/pk_wrap.c
    ${chip_mbedtls_dir}/pkwrite.c
    ${chip_mbedtls_dir}/platform.c
    ${chip_mbedtls_dir}/platform_util.c
    ${chip_mbedtls_dir}/poly1305.c
    ${chip_mbedtls_dir}/ripemd160.c
    ${chip_mbedtls_dir}/rsa.c
    ${chip_mbedtls_dir}/rsa_internal.c
    ${chip_mbedtls_dir}/sha1.c
    ${chip_mbedtls_dir}/sha256.c
    ${chip_mbedtls_dir}/sha512.c
    ${chip_mbedtls_dir}/ssl_cache.c
    ${chip_mbedtls_dir}/ssl_ciphersuites.c
    ${chip_mbedtls_dir}/ssl_cli.c
    ${chip_mbedtls_dir}/ssl_cookie.c
    ${chip_mbedtls_dir}/ssl_msg.c
    ${chip_mbedtls_dir}/ssl_srv.c
    ${chip_mbedtls_dir}/ssl_ticket.c
    ${chip_mbedtls_dir}/ssl_tls.c
    ${chip_mbedtls_dir}/threading.c
    ${chip_mbedtls_dir}/timing.c
    ${chip_mbedtls_dir}/version.c
    ${chip_mbedtls_dir}/version_features.c
    ${chip_mbedtls_dir}/x509.c
    ${chip_mbedtls_dir}/x509_create.c
    ${chip_mbedtls_dir}/x509_crl.c
    ${chip_mbedtls_dir}/x509_crt.c
    ${chip_mbedtls_dir}/x509_csr.c
    ${chip_mbedtls_dir}/x509write_crt.c
    ${chip_mbedtls_dir}/x509write_csr.c
    ${chip_mbedtls_dir}/xtea.c
)

#network - ssl - ssl_ram_map
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/ssl/ssl_ram_map/rom/rom_ssl_ram_map.c
)

#network - websocket
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/network/websocket/wsclient_tls.c
    ${sdk_root}/component/common/network/websocket/wsserver_tls.c
)

#os
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/os/freertos/cmsis_os.c
    ${sdk_root}/component/os/os_dep/device_lock.c
    ${sdk_root}/component/os/freertos/freertos_cb.c
    ${sdk_root}/component/os/freertos/freertos_service.c
    ${sdk_root}/component/os/os_dep/osdep_service.c
    ${sdk_root}/component/os/freertos/freertos_pmu.c
)

#os - freertos
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/croutine.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/event_groups.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/list.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/queue.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/stream_buffer.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/tasks.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/timers.c
)

#os - freertos - portable
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/portable/MemMang/heap_5.c
    ${sdk_root}/component/os/freertos/freertos_v10.0.1/Source/portable/GCC/ARM_RTL8710C/port.c
)

#peripheral - api
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/crypto_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/dma_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/efuse_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/gpio_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/gpio_irq_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/i2c_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/pinmap.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/pinmap_common.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/port_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/pwmout_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/rtc_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/serial_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/spdio_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/spi_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/sys_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/timer_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/us_ticker.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/us_ticker_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/wait_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/wdt_api.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/reset_reason_api.c
)

#peripheral - hal
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_s/hal_efuse.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram/hal_gdma.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram/hal_gpio.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_i2c.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram/hal_misc.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_s/hal_pinmux_nsc.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_pwm.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_sdio_dev.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_ssi.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram/hal_uart.c
)

#file_system - fatfs
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/file_system/fatfs/fatfs_ext/src/ff_driver.c
    ${sdk_root}/component/common/file_system/fatfs/r0.10c/src/diskio.c
    ${sdk_root}/component/common/file_system/fatfs/r0.10c/src/ff.c
    ${sdk_root}/component/common/file_system/fatfs/r0.10c/src/option/ccsbcs.c
    ${sdk_root}/component/common/file_system/fatfs/disk_if/src/flash_fatfs.c
)

#utilities - example
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/example/bcast/example_bcast.c
    ${sdk_root}/component/common/example/cJSON/example_cJSON.c
    ${sdk_root}/component/common/example/coap/example_coap.c
    ${sdk_root}/component/common/example/dct/example_dct.c
    ${sdk_root}/component/common/example/example_entry.c
    ${sdk_root}/component/common/example/get_beacon_frame/example_get_beacon_frame.c
    ${sdk_root}/component/common/example/high_load_memory_use/example_high_load_memory_use.c
    ${sdk_root}/component/common/example/http_client/example_http_client.c
    ${sdk_root}/component/common/example/http_download/example_http_download.c
    ${sdk_root}/component/common/example/httpc/example_httpc.c
    ${sdk_root}/component/common/example/httpd/example_httpd.c
    ${sdk_root}/component/common/example/mdns/example_mdns.c
    ${sdk_root}/component/common/example/mcast/example_mcast.c
    ${sdk_root}/component/common/example/mqtt/example_mqtt.c
    ${sdk_root}/component/common/example/nonblock_connect/example_nonblock_connect.c
    ${sdk_root}/component/common/example/ota_http/example_ota_http.c
    ${sdk_root}/component/common/example/rarp/example_rarp.c
    ${sdk_root}/component/common/example/sntp_showtime/example_sntp_showtime.c
    ${sdk_root}/component/common/example/socket_select/example_socket_select.c
    ${sdk_root}/component/common/example/socket_tcp_trx/example_socket_tcp_trx_1.c
    ${sdk_root}/component/common/example/socket_tcp_trx/example_socket_tcp_trx_2.c
    ${sdk_root}/component/common/example/ssl_download/example_ssl_download.c
    ${sdk_root}/component/common/example/ssl_server/example_ssl_server.c
    ${sdk_root}/component/common/example/tcp_keepalive/example_tcp_keepalive.c
    ${sdk_root}/component/common/example/uart_atcmd/example_uart_atcmd.c
    ${sdk_root}/component/common/example/wifi_mac_monitor/example_wifi_mac_monitor.c
    ${sdk_root}/component/common/example/wifi_roaming/example_wifi_roaming.c
    ${sdk_root}/component/common/example/wlan_fast_connect/example_wlan_fast_connect.c
    ${sdk_root}/component/common/example/wlan_scenario/example_wlan_scenario.c
    ${sdk_root}/component/common/example/websocket_client/example_wsclient.c
    ${sdk_root}/component/common/example/xml/example_xml.c
    ${sdk_root}/component/common/example/fatfs/example_fatfs.c
    ${sdk_root}/component/common/example/tickless_wifi_roaming/example_tickless_wifi_roaming.c
    ${sdk_root}/component/common/example/matter_task/example_matter.c
)

#user
list(
    APPEND app_ntz_sources

    ${prj_root}/src/main.c
)

#@SRAM
list(
    APPEND app_ntz_sources

    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/flash_api.c
    ${sdk_root}/component/soc/realtek/8710c/misc/driver/flash_api_ext.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_flash.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_spic.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/power_mode_api.c
)

list(
    APPEND list_code2sram

    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/flash_api.c
    ${sdk_root}/component/soc/realtek/8710c/misc/driver/flash_api_ext.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_flash.c
    ${sdk_root}/component/soc/realtek/8710c/fwlib/source/ram_ns/hal_spic.c
    ${sdk_root}/component/common/mbed/targets/hal/rtl8710c/power_mode_api.c
)

############################### END Source File List #############################33

#copy_ld_img2(ld/rlx8721d_img2_is.ld) copy_ld_img3
set( ld_script ${prj_root}/GCC-RELEASE/rtl8710c_ram.ld )

add_executable(
	${app_ntz}
	${app_ntz_sources}
)

target_include_directories(
	${app_ntz}
	PUBLIC

	${inc_path}
	${app_ntz_inc}
)

list(
	APPEND app_ntz_flags
	CONFIG_PLATFORM_8710C
	CONFIG_USE_MBEDTLS_ROM_ALG
	CONFIG_FUNCION_O0_OPTIMIZE
	DM_ODM_SUPPORT_TYPE=32
	MBEDTLS_CONFIG_FILE=<mbedtls_config.h>
	LWIP_IPV6_ROUTE_TABLE_SUPPORT=1
)

if(MATTER_ALL_CLUSTERS_APP)
    ADD_DEFINITIONS(-DMATTER_ALL_CLUSTERS_APP=1)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
endif()

if(MATTER_LIGHTING_APP)
    ADD_DEFINITIONS(-DMATTER_LIGHTING_APP=1)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
endif()

if(MATTER_OTA_REQUESTOR_APP)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
endif()

if(MATTER_OTA_PROVIDER_APP)
    ADD_DEFINITIONS(-DMATTER_OTA_PROVIDER_APP=1)
endif()

target_compile_definitions(
	${app_ntz}
	PRIVATE

	${app_ntz_flags}
)

target_link_libraries(
	${app_ntz}
	_hal_pmc
	_btgap
    _soc_is
	_wlan
    _http
    _dct
    _eap
    _p2p
    _websocket
    _wps
    _mdns
	-Wl,--start-group chip_main CHIP -Wl,--end-group
	m
	stdc++
	c
	gcc
	nosys
)

set(TOOLCHAINDIR "${sdk_root}/tools/arm-none-eabi-gcc/asdk")
set(ASDK_TOOLCHAIN "${TOOLCHAINDIR}/linux/newlib")
target_link_options(
	${app_ntz}
	PUBLIC
	"LINKER:SHELL:-L ${amebaz2_romsymdir}"
	"LINKER:SHELL:-L ${ASDK_TOOLCHAIN}/lib"
	"LINKER:SHELL:-L ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebaz2/lib"
	"LINKER:SHELL:-L ${sdk_root}/component/soc/realtek/8710c/misc/bsp/lib/common/GCC"
	"LINKER:SHELL:-L ${sdk_root}/component/soc/realtek/8710c/fwlib/lib/lib"
	"LINKER:SHELL:-T ${ld_script}"
	"LINKER:SHELL:-Map=${app_ntz}.map"
)

if(true)
#RENAME CODE2SRAM
foreach(_file_net ${list_code2sram})
	string(CONCAT _file_obj ${CMAKE_CURRENT_BINARY_DIR} "/CMakeFiles/${app_ntz}.dir" ${_file_net} ".obj")
	add_custom_command(TARGET ${app_ntz} PRE_LINK
        COMMAND ${CMAKE_OBJCOPY} --prefix-alloc-sections=.sram ${_file_obj} ${_file_obj}
	)
endforeach()
endif()

add_custom_command(TARGET ${app_ntz} PRE_LINK
    COMMAND echo "COPY CHIP library ..."
    COMMAND cp -f ${CHIP_OUTPUT}/lib/libCHIP.a ${LIB_ROOT}
)

add_custom_command(TARGET ${app_ntz} POST_BUILD
    COMMAND mkdir -p application_is/Debug/obj
    COMMAND mkdir -p application_is/Debug/bin
    COMMAND mkdir -p application_is/Debug/info
    COMMAND mkdir -p bootloader/Debug/bin
    COMMAND echo ${app_ntz}
    COMMAND echo $<TARGET_FILE:${app_ntz}>
    #COMMAND ${CMAKE_NM} $<TARGET_FILE:${app_ntz}> | sort > ${app_ntz}.nm.map
    COMMAND cp $<TARGET_FILE:${app_ntz}> ${matter_output_path}/application_ntz/application_is/Debug/bin/${app_ntz}.axf
    #COMMAND cp ${sdk_root}/component/soc/8735b/fwlib/rtl8735b/lib/source/ram/video/voe_bin/voe.bin voe.bin
    COMMAND ${CMAKE_OBJECOPY} -j .bluetooth_trace.text -Obinary ${matter_output_path}/application_ntz/application_is/Debug/bin/${app_ntz}.axf APP.trace
    COMMAND ${CMAKE_OBJECOPY} -R .bluetooth_trace.text ${matter_output_path}/application_ntz/application_is/Debug/bin/${app_ntz}.axf
    COMMAND ${CMAKE_OBJEDUMP} -d $<TARGET_FILE:${app_ntz}> > ${matter_output_path}/application_ntz/application_is/Debug/bin/${app_ntz}.asm

    COMMAND cp ${amebaz2_bootloaderdir}/bootloader.axf bootloader/Debug/bin/bootloader.axf
    #COMMAND rm -rf output && mkdir output
    #COMMAND cp firmware_ntz.bin output
    #COMMAND cp ${app_ntz}.nm.map output
    #COMMAND cp ${app_ntz}.map output
    #COMMAND cp ${app_ntz}.asm output
    #COMMAND cp ${app_ntz}.axf output
    #COMMAND cp ${app} ${app}_pure
    #COMMAND ${CMAKE_STRIP} --strip-unneeded ${app}_pure
    #COMMAND ${CMAKE_OBJCOPY} -j .rom.vector_tbl -j .rom.ns_vector_tbl -j .rom.text -j .ARM.extab -j .ARM.exidx -j .data_of_rom -j .note.gnu.build-id -j .rom.stubs -j .rom.ns4s_stubs -Obinary ${app}_pure rom.bin
    #COMMAND ${CMAKE_OBJCOPY} -j .gnu.sgstubs  -Obinary ${app}_pure nsc.bin
    #COMMAND ${CMAKE_OBJCOPY} -j .ram.func.table -j .ram.img.signature -j .data -j .ram.code_text -j .ram.code_rodata  -Obinary ${app}_pure ram_ld.bin
    #COMMAND ${CMAKE_OBJCOPY} -j .ram.vector -j .ram.func.table -j .ram.img.signature -j .ram.func.table -j .data -j .ram.code_text -j .ram.code_rodata  -Obinary ${app}_pure ram.bin
    #COMMAND rm -rf info
    #COMMAND mkdir info
    #COMMAND mv *.{i,s} info

    #COMMAND ${prj_root}/GCC-RELEASE/elf2bin.exe convert ${prj_root}/GCC-RELEASE/amebapro2_partition.json PARTITIONTABLE

    COMMAND cp ${prj_root}/GCC-RELEASE/keycfg.json keycfg.json
    COMMAND cp ${prj_root}/GCC-RELEASE/partition.json partition.json
    COMMAND cp ${prj_root}/GCC-RELEASE/amebaz2_firmware_is.json amebaz2_firmware_is.json
    COMMAND cp ${prj_root}/GCC-RELEASE/amebaz2_bootloader.json amebaz2_bootloader.json
    COMMAND chmod 0773 ${ELF2BIN} ${CHKSUM}
    COMMAND ${ELF2BIN} keygen keycfg.json
    COMMAND ${ELF2BIN} convert amebaz2_bootloader.json BOOTLOADER secure_bit=0
    COMMAND ${ELF2BIN} convert amebaz2_bootloader.json PARTITIONTABLE secure_bit=0
    COMMAND ${ELF2BIN} convert amebaz2_firmware_is.json FIRMWARE secure_bit=0
    COMMAND ${CHKSUM} ${matter_output_path}/application_ntz/application_is/Debug/bin/firmware_is.bin
    COMMAND ${ELF2BIN} combine ${matter_output_path}/application_ntz/application_is/Debug/bin/flash_is.bin PTAB=partition.bin,BOOT=${matter_output_path}/application_ntz/bootloader/Debug/bin/bootloader.bin,FW1=${matter_output_path}/application_ntz/application_is/Debug/bin/firmware_is.bin
    #COMMAND cp *.a output
    #COMMAND rm -rf output && mkdir output
    #COMMAND cp ${boot} output
    #COMMAND python ../split_binary.py output
    #COMMAND cd .. && tar zcvf build.tar.gz build/output
)


if(false)
add_custom_command(TARGET ${app_ntz} POST_BUILD

    COMMAND echo "========= Image manipulating ========="
	COMMAND cp $<TARGET_FILE:${app_ntz}> target_image2.axf
	COMMAND ${CMAKE_NM} target_image2.axf | sort > target_image2.map
	COMMAND ${CMAKE_OBJEDUMP} -d target_image2.axf > target_image2.asm

	COMMAND rm -rf image && mkdir -p image
	COMMAND cp target_image2.axf image/target_image2.axf
	COMMAND cp target_image2.axf image/target_pure_image2.axf
	COMMAND cp target_image2.map image
	COMMAND cp target_image2.asm image
	COMMAND cp ram_size.txt image
	COMMAND cp obj_list.txt image
	COMMAND find .. -type f -name "text.map" | xargs -i cp {} image/text_image2_ns.map

	COMMAND ${CMAKE_STRIP} image/target_pure_image2.axf
	COMMAND ${CMAKE_NM} image/target_pure_image2.axf | sort > image/target_pure_image2.map

	COMMAND ${CMAKE_OBJCOPY} -j .ram_image2.entry -j .ram_image2.text -j .ram_image2.data -Obinary image/target_pure_image2.axf image/ram_2.bin
	COMMAND ${CMAKE_OBJCOPY} -j .xip_image2.text -Obinary image/target_pure_image2.axf image/xip_image2.bin
	COMMAND ${CMAKE_OBJCOPY} -j .psram_image2.text -j .psram_image2.data -Obinary image/target_pure_image2.axf image/psram_2.bin

	# BT TRACE
	#COMMAND ${CMAKE_OBJCOPY} -j .bluetooth_trace.text -Obinary output/${app_ntz_pure}.axf APP.trace

	COMMAND echo "========== Image Info HEX =========="
	COMMAND ${CMAKE_SIZE} -A --radix=16 image/target_image2.axf
	COMMAND ${CMAKE_SIZE} -t --radix=16 image/target_image2.axf
	COMMAND echo "========== Image Info HEX =========="

	COMMAND echo "========== Image Info DEC =========="
	COMMAND ${CMAKE_SIZE} -A --radix=10 image/target_image2.axf
	COMMAND ${CMAKE_SIZE} -t --radix=10 image/target_image2.axf
	COMMAND echo "========== Image Info DEC =========="

	COMMAND echo "========== Image manipulating start =========="
	COMMAND ${prepend_tool} image/ram_2.bin  __ram_image2_text_start__  image/target_image2.map
	COMMAND ${prepend_tool} image/xip_image2.bin  __flash_text_start__  image/target_image2.map
	COMMAND ${prepend_tool} image/psram_2.bin  __psram_image2_text_start__  image/target_image2.map
	COMMAND cp ${prj_hp}/asdk/code_analyze.py .
	COMMAND chmod 777 code_analyze.py
	COMMAND python3 code_analyze.py

	COMMAND cat image/xip_image2_prepend.bin image/ram_2_prepend.bin image/psram_2_prepend.bin  > image/km4_image2_all.bin

	COMMAND ${pad_tool} image/km4_image2_all.bin
	COMMAND cat ${km0_image}/km0_image2_all.bin image/km4_image2_all.bin > image/km0_km4_image2.bin
	COMMAND echo "Image manipulating ... Done "
	COMMAND echo "========== Image manipulating end =========="


)

if (matter_generate_ota_image)
file(READ ${prj_root}/GCC-RELEASE/matter_ota_settings.json JSON_STRING)
string(JSON VendorID GET ${JSON_STRING} VendorID)
string(JSON ProductID GET ${JSON_STRING} ProductID)
string(JSON Version GET ${JSON_STRING} Version)
string(JSON VersionString GET ${JSON_STRING} VersionString)
string(JSON DigestAlgorithm GET ${JSON_STRING} DigestAlgorithm)

set(ota_imagetool_dir ${ameba_matter_root}/src/app)
add_custom_command(TARGET ${app_ntz} POST_BUILD
    COMMAND python3 ../../python_custom_ecdsa_D_gcc.py ${matter_output_path}/asdk/image/km0_km4_image2.bin ${matter_output_path}/asdk/image/OTA_ALL.bin
    COMMAND echo "========= Generating OTA_ALL.bin Complete =========="
    COMMAND cd ${ota_imagetool_dir}
    COMMAND python3 ./ota_image_tool.py create -v ${VendorID} -p ${ProductID} -vn ${Version} -vs ${VersionString} -da ${DigestAlgorithm} ${matter_output_path}/asdk/image/OTA_ALL.bin ${matter_output_path}/asdk/image/MATTER_OTA_ALL.bin
    COMMAND echo "========= Generating MATTER_OTA_ALL.bin Complete =========="
    COMMAND python3 ./ota_image_tool.py show ${matter_output_path}/asdk/image/MATTER_OTA_ALL.bin
)
endif(matter_generate_ota_image)
endif()
