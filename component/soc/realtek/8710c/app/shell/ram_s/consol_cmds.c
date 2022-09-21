/**************************************************************************//**
 * @file     consol_cmds.c
 * @brief    Some commands implementation for the shell command. It provides
 *           some basic memory write and dump commands.
 * @version  V1.00
 * @date     2016-05-30
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#include <arm_cmse.h>   /* Use CMSE intrinsics */
#include "cmsis.h"
#include "shell.h"
#include "diag.h"
#include "utility.h"
#include <string.h>
#include <stdlib.h>

//extern uint32_t strtoul (const char *nptr, char **endptr, int base);

int32_t cmd_dump_byte(uint32_t argc, uint8_t *argv[]);
int32_t cmd_dump_helfword(uint32_t argc, uint8_t *argv[]);
int32_t cmd_dump_word(uint32_t argc, uint8_t *argv[]);
int32_t cmd_write_byte(uint32_t argc, uint8_t *argv[]);
int32_t cmd_write_word(uint32_t argc, uint8_t *argv[]);

int32_t cmd_dump_byte(uint32_t argc, uint8_t *argv[])
{
    uint32_t src;
    uint32_t len;

    if(argc<1) {    
        dbg_printf ("Wrong argument number!\r\n");
        return FALSE;
    }
    
    src = strtoul((const char*)(argv[0]), (char **)NULL, 16);       

    if(!argv[1]) {
        len = 16;
    } else {
        len = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    }

    dump_bytes((uint8_t *)src,(uint8_t)len);

    return _TRUE ;
}

int32_t cmd_dump_helfword(uint32_t argc, uint8_t  *argv[])
{
    uint32_t src;
    uint32_t len;
    uint32_t i;

    if(argc<1) {
        dbg_printf ("Wrong argument number!\r\n");
        return _FALSE;
    }
    
    if(argv[0]) {
        src= strtoul((const char*)(argv[0]), (char **)NULL, 16);
    } else {   
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;      
    }
                
    if(!argv[1]) {
        len = 1;
    } else {
        len = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    }
    
    while ( (src) & 0x01) {
        src++;
    }

    for(i = 0; i < len; i+=4, src+=16) {   
        dbg_printf("%08X:  %04X    %04X    %04X    %04X    %04X    %04X    %04X    %04X\r\n",
        src, *(uint16_t *)(src), *(uint16_t *)(src+2), 
        *(uint16_t *)(src+4), *(uint16_t *)(src+6),
        *(uint16_t *)(src+8), *(uint16_t *)(src+10),
        *(uint16_t *)(src+12), *(uint16_t *)(src+14));
    }
    return _TRUE;

}

int32_t cmd_dump_word(uint32_t argc, uint8_t  *argv[])
{
    uint32_t src;
    uint32_t len;
    uint32_t i;

    if(argc<1) {    
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;
    }
    
    if(argv[0]) {
        src= strtoul((const char*)(argv[0]), (char **)NULL, 16);
    } else {    
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;      
    }
                
    if(!argv[1]) {
        len = 1;
    } else {
        len = strtoul((const char*)(argv[1]), (char **)NULL, 10);
    }
    
    while ( (src) & 0x03) {
        src++;
    }

    dbg_printf ("\r\n");
    for(i = 0; i < len; i+=4, src+=16) {   
        dbg_printf("%08X:    %08X", src, *(uint32_t *)(src));
        dbg_printf("    %08X", *(uint32_t *)(src+4));
        dbg_printf("    %08X", *(uint32_t *)(src+8));
        dbg_printf("    %08X\r\n", *(uint32_t *)(src+12));
    }
    return _TRUE;
}

int32_t cmd_write_byte(uint32_t argc, uint8_t  *argv[])
{
    uint32_t src;
    uint8_t value,i;

    src = strtoul((const char*)(argv[0]), (char **)NULL, 16);       

    for(i=0;i<argc-1;i++,src++) {
        value= strtoul((const char*)(argv[i+1]), (char **)NULL, 16);    
        dbg_printf("0x%08X = 0x%02X\r\n", src, value);
        *(volatile uint8_t *)(src) = value;
    }

    return 0;
}

int32_t cmd_write_word(uint32_t argc, uint8_t  *argv[])
{
    uint32_t src;
    uint32_t value,i;
    
    src = strtoul((const char*)(argv[0]), (char **)NULL, 16);       
    while ( (src) & 0x03) {
        src++;
    }

    for(i = 0; i < (argc - 1); i++,src+=4) {
        value= strtoul((const char*)(argv[i+1]), (char **)NULL, 16);
        dbg_printf("0x%08X = 0x%08X\r\n", src, value);
        *(volatile uint32_t *)(src) = value;
    }
    
    return 0;
}

#if !defined(CONFIG_BUILD_BOOT) && defined(CONFIG_BUILD_SECURE)

SECTION_NS_ENTRY_FUNC
int32_t NS_ENTRY cmd_dump_byte_s(uint32_t argc, uint8_t *argv[])
{
    return cmd_dump_byte(argc, argv);
}

SECTION_NS_ENTRY_FUNC
int32_t NS_ENTRY cmd_dump_helfword_s(uint32_t argc, uint8_t *argv[])
{
    return cmd_dump_helfword(argc, argv);
}

SECTION_NS_ENTRY_FUNC
int32_t NS_ENTRY cmd_dump_word_s(uint32_t argc, uint8_t *argv[])
{
    return cmd_dump_word(argc, argv);
}

SECTION_NS_ENTRY_FUNC
int32_t NS_ENTRY cmd_write_byte_s(uint32_t argc, uint8_t *argv[])
{
    return cmd_write_byte(argc, argv);
}

SECTION_NS_ENTRY_FUNC
int32_t NS_ENTRY cmd_write_word_s(uint32_t argc, uint8_t *argv[])
{
    return cmd_write_word(argc, argv);
}
#endif  //#if !defined(CONFIG_BUILD_BOOT)
