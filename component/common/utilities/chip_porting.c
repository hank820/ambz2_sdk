#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#include "platform_opts.h"
#include "platform/platform_stdlib.h"
#include "errno.h"
#include <sntp/sntp.h>
#include "dct.h"
#include <wifi_conf.h>
#include "chip_porting.h"

#define MICROSECONDS_PER_SECOND    ( 1000000LL )                                   /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND     ( 1000000000LL )                                /**< Nanoseconds per second. */
#define NANOSECONDS_PER_TICK       ( NANOSECONDS_PER_SECOND / configTICK_RATE_HZ ) /**< Nanoseconds per FreeRTOS tick. */

extern int FreeRTOS_errno;
#define errno FreeRTOS_errno

BOOL UTILS_ValidateTimespec( const struct timespec * const pxTimespec )
{
    BOOL xReturn = FALSE;

    if( pxTimespec != NULL )
    {
        /* Verify 0 <= tv_nsec < 1000000000. */
        if( ( pxTimespec->tv_nsec >= 0 ) &&
            ( pxTimespec->tv_nsec < NANOSECONDS_PER_SECOND ) )
        {
            xReturn = TRUE;
        }
    }

    return xReturn;
}

int _nanosleep( const struct timespec * rqtp,
               struct timespec * rmtp )
{
    int iStatus = 0;
    TickType_t xSleepTime = 0;

    /* Silence warnings about unused parameters. */
    ( void ) rmtp;

    /* Check rqtp. */
    if( UTILS_ValidateTimespec( rqtp ) == FALSE )
    {
        errno = EINVAL;
        iStatus = -1;
    }

    if( iStatus == 0 )
    {
        /* Convert rqtp to ticks and delay. */
        if( UTILS_TimespecToTicks( rqtp, &xSleepTime ) == 0 )
        {
            vTaskDelay( xSleepTime );
        }
    }

    return iStatus;
}

int __clock_gettime(struct timespec * tp)
{
	unsigned int update_tick = 0;
	long update_sec = 0, update_usec = 0, current_sec = 0, current_usec = 0;
	unsigned int current_tick = xTaskGetTickCount();

	sntp_get_lasttime(&update_sec, &update_usec, &update_tick);
	//if(update_tick) {
		long tick_diff_sec, tick_diff_ms;

		tick_diff_sec = (current_tick - update_tick) / configTICK_RATE_HZ;
		tick_diff_ms = (current_tick - update_tick) % configTICK_RATE_HZ / portTICK_RATE_MS;
		update_sec += tick_diff_sec;
		update_usec += (tick_diff_ms * 1000);
        current_sec = update_sec + update_usec / 1000000;
        current_usec = update_usec % 1000000;
	//}
	//else {
		//current_sec = current_tick / configTICK_RATE_HZ;
	//}
	tp->tv_sec = current_sec;
    tp->tv_nsec = current_usec*1000;
	//sntp_set_lasttime(update_sec,update_usec,update_tick);
    //printf("update_sec %d update_usec %d update_tick %d tvsec %d\r\n",update_sec,update_usec,update_tick,tp->tv_sec);
}

time_t _time( time_t * tloc )
{
#if 0
    /* Read the current FreeRTOS tick count and convert it to seconds. */
    time_t xCurrentTime = ( time_t ) ( xTaskGetTickCount() / configTICK_RATE_HZ );
#else
    time_t xCurrentTime;
    struct timespec tp;

    __clock_gettime(&tp);
    xCurrentTime = tp.tv_sec;
#endif
    /* Set the output parameter if provided. */
    if( tloc != NULL )
    {
        *tloc = xCurrentTime;
    }

    return xCurrentTime;
}

extern void vTaskDelay( const TickType_t xTicksToDelay );
int _vTaskDelay( const TickType_t xTicksToDelay )
{
	vTaskDelay(xTicksToDelay);

	return 0;
}

/*
   module size is 4k, we set max module number as 12;
   if backup enabled, the total module number is 12 + 1*12 = 24, the size is 96k;
   if wear leveling enabled, the total module number is 12 + 2*12 + 3*12 = 36, the size is 288k"
*/
#define DCT_BEGIN_ADDR_MATTER   DCT_BEGIN_ADDR    /*!< DCT begin address of flash, ex: 0x100000 = 1M */
#define MODULE_NUM              40                /*!< max number of module */
#define VARIABLE_NAME_SIZE      32                /*!< max size of the variable name */
#define VARIABLE_VALUE_SIZE     64 + 4          /*!< max size of the variable value */
                                                  /*!< max value number in moudle = 4024 / (32 + 1860+4) = 2 */

#define DCT_BEGIN_ADDR_MATTER2  DCT_BEGIN_ADDR2
#define MODULE_NUM2             30
#define VARIABLE_NAME_SIZE2     32
#define VARIABLE_VALUE_SIZE2    400 + 4

#define ENABLE_BACKUP           0
#define ENABLE_WEAR_LEVELING    0

const char *matter_domain[19] =
{
    "chip-factory",
    "chip-config",
    "chip-counters",
    "chip-fabric-1",
    "chip-fabric-2",
    "chip-fabric-3",
    "chip-fabric-4",
    "chip-fabric-5",
    "chip-acl",
    "chip-groupmsgcounters",
    "chip-attributes",
    "chip-bindingtable",
    "chip-ota",
    "chip-failsafe",
    "chip-sessionresumption",
    "chip-deviceinfoprovider",
    "chip-groupdataprovider",
    "chip-others2",
    "chip-others"
};

/*
 * allocate into predefined domains
 * needs to be more robust, but currently it gets the job done
 * chip-groupdataproviders is put into their respective chip-fabric-x
 */
const char* domainAllocator(const char *domain)
{
    //chip-factory
    if(strcmp(domain, "chip-factory") == 0)
        return matter_domain[0];
    //chip-config
    if(strcmp(domain, "chip-config") == 0)
        return matter_domain[1];
    //chip-counters
    if(strcmp(domain, "chip-counters") == 0)
        return matter_domain[2];
    // chip-acl
    if(strncmp(domain+4, "ac", 2) == 0)
        return matter_domain[8];

    if(domain[0] == 'f')
    {
        // chip-groupdataprovider
        if(strncmp(domain+4, "g", 1) == 0)
            return matter_domain[16];

        // chip-fabrics
        switch(atoi(&domain[2]))
        {
            case 1:
                return matter_domain[3];
                break;
            case 2:
                return matter_domain[4];
                break;
            case 3:
                return matter_domain[5];
                break;
            case 4:
                return matter_domain[6];
                break;
            case 5:
                return matter_domain[7];
                break;
        }
    }
    // chip-groupmsgcounters
    if((strcmp(domain, "g/gdc") == 0) || (strcmp(domain, "g/gcc") == 0))
        return matter_domain[9];
    // chip-attributes
    if(strncmp(domain, "g/a", 3) == 0)
        return matter_domain[10];
    // chip-bindingtable
    if(strncmp(domain, "g/bt", 4) == 0)
        return matter_domain[11];
    // chip-ota
    if(strncmp(domain, "g/o", 3) == 0)
        return matter_domain[12];
    // chip-failsafe
    if(strncmp(domain, "g/fs", 4) == 0)
        return matter_domain[13];
    // chip-sessionresumption
    if((strncmp(domain, "g/s", 3) == 0) && strcmp(domain, "g/sri") != 0)
        return matter_domain[14];
    // chip-deviceinfoprovider
    if(strncmp(domain, "g/userlbl", 9) == 0)
        return matter_domain[15];
    // chip-others2
    // Store KV pairs that can't fit in chip-others (>64bytes)
    if((strcmp(domain, "wifi-pass") == 0) || (strcmp(domain, "g/sri") == 0))
        return matter_domain[17];
    // chip-others
    // store FabricTable, FailSafeContextKey, GroupFabricList, FabricIndexInfo, IMEventNumber in chip-others
    return matter_domain[18];
}

/*
 * Assigns DCT region to an allocated domain
 * retval:
 *      1 => DCT1 region
 *      2 => DCT2 region
 */
uint8_t allocateRegion(const char *allocatedDomain)
{
    if (strncmp(allocatedDomain, "chip-fabric", 11) == 0)
        return 2;
    if (strcmp(allocatedDomain, "chip-others2") == 0)
        return 2;
    else
        return 1;
}

int32_t initPref(void)
{
    int32_t ret;
    ret = dct_init(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init failed with error: %d\n", ret);
    else
        printf("dct_init success\n");

    ret = dct_init2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init2 failed with error: %d\n", ret);
    else
        printf("dct_init2 success\n");

    return ret;
}

int32_t deinitPref(void)
{
    int32_t ret;
    ret = dct_format(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format failed with error: %d\n", ret);
    else
        printf("dct_format success\n");

    ret = dct_format2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format2 failed with error: %d\n", ret);
    else
        printf("dct_format2 success\n");

    return ret;
}

int32_t registerPref(const char * ns)
{
    int32_t ret;
    ret = dct_register_module(ns);
    if (ret != 0)
        printf("dct_register_module %s failed with error: %d\n",ns, ret);
    else
        printf("dct_register_module %s success\n",ns);

    return ret;
}

int32_t registerPref2(const char * ns)
{
    int32_t ret;
    ret = dct_register_module2(ns);
    if (ret != 0)
        printf("dct_register_module2 %s failed with error: %d\n",ns, ret);
    else
        printf("dct_register_module2 %s success\n",ns);

    return ret;
}


int32_t clearPref(const char * ns)
{
    int32_t ret;
    ret = dct_unregister_module(ns);
    if (ret != 0)
        printf("dct_unregister_module %s failed with error: %d\n",ns, ret);
    else
        printf("dct_unregister_module %s success\n",ns);

    return ret;
}

int32_t deleteKey(const char *domain, const char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
	{
	    ret = dct_open_module(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_delete_variable(&handle, key);
	    if(ret == DCT_ERR_NOT_FIND || ret == DCT_SUCCESS)
	        ret = DCT_SUCCESS;
	    else
	        printf("%s : dct_delete_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, allocatedDomain);
	    if (ret != DCT_SUCCESS){
	        printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_delete_variable(&handle, key);
	    if(ret == DCT_ERR_NOT_FIND || ret == DCT_SUCCESS)
	        ret = DCT_SUCCESS;
	    else
	        printf("%s : dct_delete_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    dct_close_module2(&handle);
	    //dct_unregister_module2(key);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

bool checkExist(const char *domain, const char *key)
{
	dct_handle_t handle;
	int32_t ret = -1;
	uint16_t len = 0;
	uint8_t found = 0;
	uint8_t *str = malloc(sizeof(uint8_t) * VARIABLE_VALUE_SIZE-4);
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
	{
		ret = dct_open_module(&handle, allocatedDomain);
		if (ret != DCT_SUCCESS){
			printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
			goto exit;
		}

		if(found == 0)
		{
			len = sizeof(uint32_t);
			ret = dct_get_variable_new(&handle, key, (char *)str, &len);
			if(ret == DCT_SUCCESS)
			{
				printf("checkExist key=%s found.\n", key);
				found = 1;
			}
		}

		if(found == 0)
		{
			len = sizeof(uint64_t);
			ret = dct_get_variable_new(&handle, key, (char *)str, &len);
			if(ret == DCT_SUCCESS)
			{
				printf("checkExist key=%s found.\n", key);
				found = 1;
			}
		}

		dct_close_module(&handle);
	}
	else
	{
		ret = dct_open_module2(&handle, allocatedDomain);
		if (ret != DCT_SUCCESS){
			printf("%s : dct_open_module2(%s) failed with error : %d\n" ,__FUNCTION__, allocatedDomain, ret);
			goto exit;
		}

		len = VARIABLE_VALUE_SIZE-4;
		ret = dct_get_variable_new2(&handle, key, str, &len);
		if(ret == DCT_SUCCESS)
		{
			printf("checkExist key=%s found.\n", key);
			found = 1;
		}

		dct_close_module2(&handle);
	}

	if(found == 0)
		printf("checkExist key=%s not found. ret=%d\n",key ,ret);

exit:
	free(str);
	return found;
}

int32_t setPref_new(const char *domain, const char *key, uint8_t *value, size_t byteCount)
{
    dct_handle_t handle;
    int32_t ret = -1;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
	    ret = dct_open_module(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_set_variable_new(&handle, key, (char *)value, (uint16_t)byteCount);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_set_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_set_variable_new2(&handle, key, (char *)value, (uint16_t)byteCount);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_set_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bool_new(const char *domain, const char *key, uint8_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint8_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module(&handle);
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint8_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module2(&handle);
    }

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u32_new(const char *domain, const char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint32_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module(&handle);
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint32_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module2(&handle);
    }
exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u64_new(const char *domain, const char *key, uint64_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint64_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module(&handle);
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        len = sizeof(uint64_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        if (DCT_SUCCESS != ret)
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module2(&handle);
    }

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_str_new(const char *domain, const char *key, char * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
	{
	    ret = dct_open_module(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed with error: %d\n",__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_get_variable_new(&handle, key, buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable(%s) failed with error: %d\n",__FUNCTION__, key, ret);

	    *outLen = _bufSize;

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_get_variable_new2(&handle, key, buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    *outLen = _bufSize;

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bin_new(const char *domain, const char *key, uint8_t * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
	{
	    ret = dct_open_module(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_get_variable_new(&handle, key, (char *)buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    *outLen = _bufSize;

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, allocatedDomain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
	        goto exit;
	    }

	    ret = dct_get_variable_new2(&handle, key, (char *)buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

	    *outLen = _bufSize;

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

/************************** Matter WiFi Related **************************/
uint32_t apNum = 0; // no of total AP scanned
static rtw_scan_result_t matter_userdata[65] = {0};
static char *matter_ssid;

chip_connmgr_callback chip_connmgr_callback_func = NULL;
void *chip_connmgr_callback_data = NULL;
void chip_connmgr_set_callback_func(chip_connmgr_callback p, void *data)
{
    chip_connmgr_callback_func = p;
    chip_connmgr_callback_data = data;
}

void print_matter_scan_result( rtw_scan_result_t* record )
{
	RTW_API_INFO("%s\t ", ( record->bss_type == RTW_BSS_TYPE_ADHOC ) ? "Adhoc" : "Infra");
	RTW_API_INFO(MAC_FMT, MAC_ARG(record->BSSID.octet));
	RTW_API_INFO(" %d\t ", record->signal_strength);
	RTW_API_INFO(" %d\t  ", record->channel);
	RTW_API_INFO(" %d\t  ", record->wps_type);
	RTW_API_INFO("%s\t\t ", ( record->security == RTW_SECURITY_OPEN ) ? "Open" :
								 ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
								 ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
								 ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
								 ( record->security == RTW_SECURITY_WPA_MIXED_PSK ) ? "WPA Mixed" :
								 ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
								 ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
								 ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_TKIP_PSK) ? "WPA/WPA2 TKIP" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_AES_PSK) ? "WPA/WPA2 AES" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED_PSK) ? "WPA/WPA2 Mixed" :
								 ( record->security == RTW_SECURITY_WPA_TKIP_ENTERPRISE ) ? "WPA TKIP Enterprise" :
								 ( record->security == RTW_SECURITY_WPA_AES_ENTERPRISE ) ? "WPA AES Enterprise" :
								 ( record->security == RTW_SECURITY_WPA_MIXED_ENTERPRISE ) ? "WPA Mixed Enterprise" :
								 ( record->security == RTW_SECURITY_WPA2_TKIP_ENTERPRISE ) ? "WPA2 TKIP Enterprise" :
								 ( record->security == RTW_SECURITY_WPA2_AES_ENTERPRISE ) ? "WPA2 AES Enterprise" :
								 ( record->security == RTW_SECURITY_WPA2_MIXED_ENTERPRISE ) ? "WPA2 Mixed Enterprise" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE ) ? "WPA/WPA2 TKIP Enterprise" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE ) ? "WPA/WPA2 AES Enterprise" :
								 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE ) ? "WPA/WPA2 Mixed Enterprise" :
								 "Unknown");

	RTW_API_INFO(" %s ", record->SSID.val);
	RTW_API_INFO("\r\n");
}

static rtw_result_t matter_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
	if (malloced_scan_result->scan_complete != RTW_TRUE)
    {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

		RTW_API_INFO("%d\t ", ++apNum);
		print_matter_scan_result(record);

		if(malloced_scan_result->user_data)
			memcpy((void *)((char *)malloced_scan_result->user_data+(apNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
	}
    else
    {
        if (chip_connmgr_callback_func && chip_connmgr_callback_data)
        {
            // inform matter
            chip_connmgr_callback_func(chip_connmgr_callback_data);
        }
        else
        {
            printf("chip_connmgr_callback_func is NULL\r\n");
		    apNum = 0;
            return RTW_ERROR;
        }
	}
	return RTW_SUCCESS;
}

static rtw_result_t matter_scan_with_ssid_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
	if (malloced_scan_result->scan_complete != RTW_TRUE)
    {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

		if((malloced_scan_result->user_data) && (!strcmp(matter_ssid, record->SSID.val)))
        {
            RTW_API_INFO("%d\t ", ++apNum);
			memcpy((void *)((char *)malloced_scan_result->user_data+(apNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
            print_matter_scan_result(record);
        }
	}
    else
    {
        if (chip_connmgr_callback_func && chip_connmgr_callback_data)
        {
            // inform matter
            chip_connmgr_callback_func(chip_connmgr_callback_data);
            vPortFree(matter_ssid);
        }
        else
        {
            printf("chip_connmgr_callback_func is NULL\r\n");
		    apNum = 0;
            vPortFree(matter_ssid);
            return RTW_ERROR;
        }
	}
	return RTW_SUCCESS;
}

void matter_scan_networks(void)
{
	volatile int ret = RTW_SUCCESS;
    apNum = 0; // reset counter at the start of scan
	if((ret = wifi_scan_networks(matter_scan_result_handler, matter_userdata)) != RTW_SUCCESS)
    {
		printf("ERROR: wifi scan failed\n\r");
	}
}

void matter_scan_networks_with_ssid(const unsigned char *ssid, size_t length)
{
	volatile int ret = RTW_SUCCESS;
    apNum = 0; // reset counter at the start of scan
    matter_ssid = (char*) pvPortMalloc(length+1);
    memset(matter_ssid, 0, length+1);
    memcpy(matter_ssid, ssid, length);
    matter_ssid[length] = '\0';
	if((ret = wifi_scan_networks(matter_scan_with_ssid_result_handler, matter_userdata)) != RTW_SUCCESS)
    {
		printf("ERROR: wifi scan failed\n\r");
	}
}

void matter_get_scan_results(rtw_scan_result_t *result_buf, uint8_t scanned_num)
{
    memcpy(result_buf, matter_userdata, sizeof(rtw_scan_result_t) * scanned_num);
}

#ifdef __cplusplus
}
#endif
