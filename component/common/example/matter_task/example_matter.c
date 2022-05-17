#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include "basic_types.h"
#include "platform_opts.h"
#include "section_config.h"
#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT && CONFIG_FAST_DHCP
#include "wifi/wifi_conf.h"
#include "wlan_fast_connect/example_wlan_fast_connect.h"
extern uint32_t offer_ip;
#endif

#if defined(CONFIG_EXAMPLE_MATTER) && CONFIG_EXAMPLE_MATTER

extern void ChipTest(void);

static void example_matter_task_thread(void *pvParameters)
{
	vTaskDelay(5000);

#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT && CONFIG_FAST_DHCP
	int autoreconnect_count = 0;

	/*Wait for internet connection if fast connect has data*/
	while((wifi_is_ready_to_transceive(RTW_STA_INTERFACE) != RTW_SUCCESS) && (offer_ip)){
		printf("\n\r[WiFi] exist network: wait for Wi-Fi connection");
		autoreconnect_count++;
		vTaskDelay(AUTO_RECONNECT_INTERVAL*1000);
		if(autoreconnect_count > (AUTO_RECONNECT_COUNT + 2))
			break;
	}
#endif

	ChipTest();

	vTaskDelete(NULL);
	return;
}

void example_matter_task(void)
{
	if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
}

#endif // #if (CONFIG_EXAMPLE_MATTER)
