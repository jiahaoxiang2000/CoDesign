/******************************************************************************
 * Copyright (C) 2002 - 2021 Xilinx, Inc.  All rights reserved.
 * Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/*****************************************************************************/
/**
 * @file xgpio_example.c
 *
 * This file contains a design example using the AXI GPIO driver (XGpio) and
 * hardware device.  It only uses channel 1 of a GPIO device and assumes that
 * the bit 0 of the GPIO is connected to the LED on the HW board.
 *
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.00a rmm  03/13/02 First release
 * 1.00a rpm  08/04/03 Removed second example and invalid macro calls
 * 2.00a jhl  12/15/03 Added support for dual channels
 * 2.00a sv   04/20/05 Minor changes to comply to Doxygen and coding guidelines
 * 3.00a ktn  11/20/09 Minor changes as per coding guidelines.
 * 4.1   lks  11/18/15 Updated to use canonical xparameters and
 *		      clean up of the comments and code for CR 900381
 * 4.3   sk   09/29/16 Modified the example to make it work when LED_bits are
 *                     configured as an output. CR# 958644
 *       ms   01/23/17 Added xil_printf statement in main function to
 *                     ensure that "Successfully ran" and "Failed" strings
 *                     are available in all examples. This is a fix for
 *                     CR-965028.
 * 4.5  sne   06/12/19 Fixed IAR compiler warning.
 * 4.10 gm    07/11/23 Added SDT support.
 *
 * </pre>
 ******************************************************************************/

/***************************** Include Files *********************************/

#include "PmodGPIO.h"
#include "oled.h"
#include "sleep.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xparameters.h"
#include <stdint.h>

/************************** Constant Definitions *****************************/

#define LED 0x01 /* Assumes bit 0 of GPIO is connected to an LED  */
#define SEGMENT 0xff
/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef SDT
#define GPIO_EXAMPLE_DEVICE_ID XPAR_GPIO_0_DEVICE_ID
#else
#define XGPIO_AXI_BASEADDRESS XPAR_XGPIO_1_BASEADDR
#endif

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY 10000000

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#ifdef PRE_2_00A_APPLICATION

/*
 * The following macros are provided to allow an application to compile that
 * uses an older version of the driver (pre 2.00a) which did not have a channel
 * parameter. Note that the channel parameter is fixed as channel 1.
 */
#define XGpio_SetDataDirection(InstancePtr, DirectionMask)                     \
  XGpio_SetDataDirection(InstancePtr, LED_CHANNEL, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr)                                        \
  XGpio_DiscreteRead(InstancePtr, LED_CHANNEL)

#define XGpio_DiscreteWrite(InstancePtr, Mask)                                 \
  XGpio_DiscreteWrite(InstancePtr, LED_CHANNEL, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask)                                   \
  XGpio_DiscreteSet(InstancePtr, LED_CHANNEL, Mask)

#endif

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio Gpio;         /* The Instance of the GPIO Driver */
XGpio Gpio_segment; // segment
XGpio Gpio_Ja;      // segment
PmodGPIO JaDevice;  // JA pmod

/*****************************************************************************/
/**
 *
 * The purpose of this function is to illustrate how to use the GPIO
 * driver to turn on and off an LED.
 *
 *
 * @return	XST_FAILURE to indicate that the GPIO Initialization had
 *		failed.
 *
 * @note		This function will not return if the test is running.
 *
 ******************************************************************************/

int main(void) {
  int Status;
  volatile int Delay;
  //******************segment********************
  // Initialize the 7 segment
  XGpio_Initialize(&Gpio_segment, XPAR_XGPIO_0_BASEADDR);
  // set output mode for GPIO
  XGpio_SetDataDirection(&Gpio_segment, 1, ~SEGMENT);
  // Set the SEGMENT to High for close segment
  XGpio_DiscreteWrite(&Gpio_segment, 1, SEGMENT);
  xil_printf("Close 7 segment display\r\n");
  //******************segment********************

  //******************JA********************
  // pin 1 for  IIC SCL, pin 2 for IIC SDA
  GPIO_begin(&JaDevice, XPAR_PMODGPIO_0_BASEADDR, 0x00);
  OLED_Init();
  OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
  OLED_Refresh();
  OLED_Clear();

  //******************JA********************

  /* Initialize the GPIO driver */
  Status = XGpio_Initialize(&Gpio, XGPIO_AXI_BASEADDRESS);
  if (Status != XST_SUCCESS) {
    xil_printf("Gpio Initialization Failed\r\n");
    return XST_FAILURE;
  }

  /* Set the direction for all signals as inputs except the LED output */
  XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ~LED);
  /* Loop forever blinking the LED */

  while (1) {
    /* Set the LED to High */
    XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, LED);
    /* Wait a small amount of time so the LED is visible */
    sleep(1);
    /* Clear the LED bit */
    XGpio_DiscreteClear(&Gpio, LED_CHANNEL, LED);
    /* Wait a small amount of time so the LED is visible */
    sleep(1);

    OLED_ShowString(0, 0, "P:0123456789ABCDEF", 12);
    OLED_ShowString(0, 15, "FEDCBA9876543210", 12);
    OLED_ShowString(0, 30, "C:6C842BCE41C7E351", 12);
    OLED_ShowString(0, 45, "4068A3FB8BB42936", 12);
    OLED_Refresh();
  }
}
