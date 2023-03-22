/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <sleep.h>
#include "xil_cache.h"
#include "xil_exception.h"
#define  c (*(volatile uint8_t *)(0xFFFF0000))
#define image130 (*(volatile unsigned long *)(0xFFFF0001))
#define newImage128 (*(volatile unsigned long*)(0xFFFF00FF))

int main()
{
    init_platform();
    Xil_SetTlbAttributes(0xFFFF0000, 0x14de2);
	int i, j;


	while(c!=1);
	printf ("starting calculating second half on cpu1\n");
	uint8_t (*image)[130]=image130;
	uint8_t (*newImage)[128]=newImage128;
				for (i = 65; i <= 128; ++i)
				{
					for (j = 1; j <=128; ++j)
					{
						newImage[i-1][j-1]= ((-1 * image[i-1][j-1]) + (-1 * image[i-1][j]) + ( -1 * image[i-1][j+1]) + ( -1 * image[i][j-1]) + ( 9 * image[i][j]) + ( -1 * image[i][j+1]) + ( -1 * image[i+1][j-1]) + ( -1 * image[i+1][j]) + ( -1 * image[i+1][j+1]));
					}
				}
		c=2;


    cleanup_platform();
    return 0;
}
