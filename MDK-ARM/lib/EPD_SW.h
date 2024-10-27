#ifndef __EPD_SW_H
#define __EPD_SW_H
#include "stm32g4xx.h"
#include "EPD_HW.h"


typedef enum COLOR
{
	BLACK=0,
	WHITE=1,
	RED=2
}UI_COLOR_TYPE;

extern uint8_t GraphicRAM_BW[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8];
extern uint8_t GraphicRAM_R[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8];

void UI_Init();
void UI_UpdateGRAM();
void UI_Update_All();
void UI_Update_Part(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye);

void UI_DrawPixel(uint16_t x,uint16_t y,UI_COLOR_TYPE color);

void UI_DrawBox_FILLED(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE color);
void UI_DrawBox_EMPTY(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE coloro,UI_COLOR_TYPE colorin,uint16_t edgewide);


#endif
