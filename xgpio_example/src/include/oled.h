#ifndef __OLED_H
#define __OLED_H

#include "PmodGPIO.h"
#include "stdlib.h"

extern PmodGPIO JaDevice; // JA pmod

#define OLED_SCLK_Clr() GPIO_setPin(&JaDevice, 1, 0)
#define OLED_SCLK_Set() GPIO_setPin(&JaDevice, 1, 1) // SCL

#define OLED_SDIN_Clr() GPIO_setPin(&JaDevice, 2, 0) // DIN
#define OLED_SDIN_Set() GPIO_setPin(&JaDevice, 2, 1)

#define OLED_CMD 0
#define OLED_DATA 1
#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x, u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void OLED_DrawCircle(u8 x, u8 y, u8 r);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1);
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1);
void OLED_ScrollDisplay(u8 num, u8 space);
void OLED_WR_BP(u8 x, u8 y);
void OLED_ShowPicture(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);
void OLED_Init(void);

/***
    // use example
    OLED_Init();
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);
    OLED_Refresh();
    OLED_Clear();
    
    OLED_ShowString(0, 0, "Welcome User: Xrh", 12);
    OLED_ShowString(0, 15, "Plaintext :01234567", 12);
    OLED_ShowString(0, 30, "Encryption:f0f65f3b", 12);
    OLED_ShowString(0, 45, "Decryption:01234567", 12);
    OLED_Refresh();
***/
#endif
