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
 * this list of conditions and the disclaiimer below.
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
#include "usart.h"
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>

#define MAX_BUFFER	128

static char dbg_buf[MAX_BUFFER];

static char* BIN_TO_HEX[]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};

static inline short fill_char(char *buf, char val)
{
	*buf = val;

	return 1;
}

static inline short fill_string(char *buf, char *p)
{
	short num = 0;

	while (*p != 0) {
		*buf++ = *p++;
		num++;
	}

	return num;
}

static inline short fill_hex_int(char *buf, unsigned int data)
{
	short num = 0;

	if ((data >> 4) > 0) {
		num += fill_hex_int(buf, data >> 4);
		buf += num;
	}

	if ((data & 0xF) < 10)
		fill_char(buf, (data & 0xF) + '0');
	else
		fill_char(buf, (data & 0xF) - 10 + 'a');
	num++;

	return num;
}
static inline short fill_bin_int (char *buf, unsigned int data)
{
	short num = 0;
	int idxBit = 31;
	do
	{
		*buf++ = data & (1<<idxBit)  ?  '1' : '0';
		num++;

		//add decoration for each byte
		if ((idxBit&0xF8) == idxBit)
		{
			*buf++=':';
			num++;
		}
	} while (idxBit-- > 0);

	return num;
}
int dbg_printf(const char *fmt_str, ...)
{
	va_list ap;

	char *p = dbg_buf;

	short num = 0;

	va_start(ap, fmt_str);
	while (*fmt_str != 0) {
		if (*fmt_str != '%')
			*p++ = *fmt_str++;
		else if (*(fmt_str + 1) == '%') {
			*p++ = '%';
			fmt_str += 2;
		} else {
			fmt_str++;	/* skip % */
			switch (*fmt_str) {
			case 'b':// Add binary dump support
				*p++='0';
				*p++='b';
				num=fill_bin_int(p, va_arg(ap, unsigned int));
				break;
			case 'd':
			case 'i':
			case 'u':
			case 'x':
				*p++ = '0';
				*p++ = 'x';
				num = fill_hex_int(p, va_arg(ap, unsigned int));

				break;
			case 's':
				num = fill_string(p, va_arg(ap, char *));

				break;
			case 'c':
				num =
				    fill_char(p, (char)va_arg(ap, signed long));

				break;
			default:
				return -1;
			}

			fmt_str++;
			p += num;
		}
	}
	va_end(ap);

	/* Terminate the result string */
	*p = '\0';

	usart_puts(dbg_buf);

	return 0;
}
//***********************************************************************
int dbg_dump_buffer(const char level, const char* prefix, unsigned char* buffer, unsigned int len)
{
	unsigned int pos = 0;
	if (level > BOOTSTRAP_DEBUG_LEVEL)
			return 0;
	usart_puts(prefix);
	usart_puts("\n\t");
	if (!len)
	{
		usart_puts("EMPTY");
	}
	do
	{
		usart_puts("[");
		usart_puts(BIN_TO_HEX[*buffer >> 4]);
		usart_puts(BIN_TO_HEX[*buffer & 0x0F]);
		usart_puts("],");
		buffer++;
		pos++;
		if (pos == 0x10 )
		{
			usart_puts("\n\t");
			pos = 0;
		}
	}
	while (--len);
	usart_puts("\n");
	return 0;
}
//***********************************************************************

