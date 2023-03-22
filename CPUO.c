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
#include <stdlib.h>
#include <sleep.h>




#define  c (*(volatile uint8_t *)(0xFFFF0000))
#define image130  (*(volatile unsigned long *)(0xFFFF0001))
#define newImage128 (*(volatile unsigned long*)(0xFFFF00FF))

//static void check(int i,int j, uint8_t image[130][130], uint8_t newImage[128][128])
//{
//    printf ("%d, %d, %d \n "
//            "%d, %d, %d \n "
//            "%d, %d, %d \n ", image[i-1][j-1], image[i-1][j], image[i-1][j+1], image[i][j-1], image[i][j], image[i][j+1], image[i+1][j-1], image[i+1][j], image[i+1][j+1]);
//    printf ("value %d \n",newImage[i-1][j-1]);
//}

int main()
{

    init_platform();

    Xil_SetTlbAttributes(0xFFFF0000, 0x14de2);
    	//XScuTimer_Start(TimerInstancePtr);
    	int i, j;
    	uint8_t image[130][130];
    	uint8_t newImage[128][128];
    	c=0;
    	image130=(unsigned long)image;
    	newImage128=(unsigned long) newImage;

    	
    	printf ("getting the image\n");


    	for (i = 1; i <= 128; ++i) {
    		for (j = 1; j <=128; ++j) {
    			image[i][j]=rand() %256;
    		}
    	}
    	for (i=0; i<=129; ++i){
  	   		image[i][0]=image[i][1];
       		image[i][129]=image[i][128];
       }
    	for (j=0; j<=129; ++j){
    		image[0][j]= image[1][j];
    		image[129][j]=image[128][j];
    	}

    

    	printf ("starting calculating first half on cpu0\n");

    	
    	c=1;

    	for (i = 1; i <= 64; ++i) {
 	      	for (j = 1; j <=128; ++j)
 	    	{
 	    		newImage[i-1][j-1]= ((-1 * image[i-1][j-1]) + (-1 * image[i-1][j]) + ( -1 * image[i-1][j+1]) + ( -1 * image[i][j-1]) + ( 9 * image[i][j]) + ( -1 * image[i][j+1]) + ( -1 * image[i+1][j-1]) + ( -1 * image[i+1][j]) + ( -1 * image[i+1][j+1]));
       		}
    	}


    	while (c!=2 );
        
    	printf ("the filter is applied to the image \n");

    	//check(54, 5,image,newImage);


    cleanup_platform();
    return 0;
}
