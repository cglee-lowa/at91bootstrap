/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "common.h"
#include "hardware.h"
#include "board.h"
#include "usart.h"
#include "debug.h"
#include "slowclk.h"
#include "dataflash.h"
#include "nandflash.h"
#include "sdcard.h"
#include "flash.h"
#include "string.h"
#include "onewire_info.h"

#ifndef CONFIG_UPLOAD_3RD_STAGE
#if defined(CONFIG_LOAD_LINUX) || defined(CONFIG_LOAD_ANDROID)
extern int load_kernel(struct image_info *img_info);
#endif

//! Firmware loader definitions
#if defined(CONFIG_LOAD_LINUX) || defined(CONFIG_LOAD_ANDROID)
#define load_image load_kernel
#else
#if defined (CONFIG_DATAFLASH)
#define load_image load_dataflash
#elif defined(CONFIG_NANDFLASH)
#define load_image load_nandflash
#elif defined(CONFIG_SDCARD)
#define load_image load_sdcard
#else
#error "No booting media_str specified!"
#endif
#endif
#else
int load_nothing (struct image_info* unused)
{
  //NOTHING TO DO
  uasrt_puts("NOTHING is LOADED\n");
  return 0;
}
#define load_image load_nothing
#endif /*CONFIG_UPLOAD_3RD_STAGE*/

//typedef int (*load_function)(struct image_info *img_info);

void (*sdcard_set_of_name)(char *) = NULL;

static void display_banner (void)
{
#ifndef CONFIG_UPLOAD_3RD_STAGE
  char *version = "AT91Bootstrap";
#else
  char *version = "AT91Bootstrap - 3rd stage uploaded through DEBUG PROBE";
#endif

	char *ver_num = " "AT91BOOTSTRAP_VERSION" ("COMPILE_TIME")";

#if defined( CONFIG_CPU_CLK_498MHZ)
	const char* const clocks_msg = " CLOCKS : Core:498MHz, Bus:166MHz\n";
#elif defined (CONFIG_CPU_CLK_400MHZ)
	const char* const clocks_msg = " CLOCKS : Core:400MHz, Bus:132MHz\n";
#elif defined (CONFIG_CPU_CLK_528MHZ)
	const char* const clocks_msg = " CLOCKS : Core:528MHz, Bus:133MHz\n";
#else
#error NO Clock defined !!
	const char* const clocks_msg = "UNKNOWN";
#endif
	usart_puts("\n");
	usart_puts("\n");
	usart_puts(version);
	usart_puts(ver_num);
	usart_puts(clocks_msg);
	usart_puts("\n");
	usart_puts("\n");
}

int main(void)
{
	struct image_info image;
	char *media_str = NULL;
	int ret;

	char filename[FILENAME_BUF_LEN];
	char of_filename[FILENAME_BUF_LEN];

	memset(&image, 0, sizeof(image));
	memset(filename, 0, FILENAME_BUF_LEN);
	memset(of_filename, 0, FILENAME_BUF_LEN);

	image.dest = (unsigned char *)JUMP_ADDR;
#ifdef CONFIG_OF_LIBFDT
	image.of = 1;
	image.of_dest = (unsigned char *)OF_ADDRESS;
#endif

#ifndef CONFIG_UPLOAD_3RD_STAGE

#ifdef CONFIG_NANDFLASH
	media_str = "NAND: ";
	image.offset = IMG_ADDRESS;
#if !defined(CONFIG_LOAD_LINUX) && !defined(CONFIG_LOAD_ANDROID)
	image.length = IMG_SIZE;
#endif
#ifdef CONFIG_OF_LIBFDT
	image.of_offset = OF_OFFSET;
#endif
#endif

#ifdef CONFIG_DATAFLASH
	media_str = "SF: ";
	image.offset = IMG_ADDRESS;
#if !defined(CONFIG_LOAD_LINUX) && !defined(CONFIG_LOAD_ANDROID)
	image.length = IMG_SIZE;
#endif
#ifdef CONFIG_OF_LIBFDT
	image.of_offset = OF_OFFSET;
#endif
#endif

#ifdef CONFIG_SDCARD
	media_str = "SD/MMC: ";
	image.filename = filename;
	strcpy(image.filename, IMAGE_NAME);
#ifdef CONFIG_OF_LIBFDT
	image.of_filename = of_filename;
#endif
#endif
 
#endif /*CONFIG_UPLOAD_3RD_STAGE*/
  
#ifdef CONFIG_HW_INIT
	hw_init();
#endif

	display_banner();

#ifdef CONFIG_LOAD_ONE_WIRE
	/* Load one wire information */
	load_1wire_info();
#endif
	
	ret = load_image(&image);

	if (media_str)
		usart_puts(media_str);

	if (ret == 0){
		usart_puts("Done to load image\n");
	}
	if (ret == -1) {
		usart_puts("Failed to load image\n");
		while(1);
	}
	if (ret == -2) {
		usart_puts("Success to recovery\n");
		while (1);
	}

#ifdef CONFIG_SCLK
	slowclk_switch_osc32();
#endif

	return JUMP_ADDR;
}
//****************************************************
//Will just display a running symbol
void displayWaitDbg(void)
{
  register unsigned int dbgdscr asm("r0");
  register unsigned int cpsr asm("r1");

  dbg_log(2,"DBGDSCR:%b ; CPSR:0x%b\n", dbgdscr, cpsr);
  usart_puts("Entering HALT Debug Mode, Waiting for the DEBUGGER ...\n");
}
//****************************************************
//Failure message
void displayFailedMsg()
{
  usart_puts("CPU not HALTED !\n");
}
//****************************************************
