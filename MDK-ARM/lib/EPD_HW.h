#ifndef __EPD_HW_H
#define __EPD_HW_H
#include "stm32g4xx.h"

#define EPD_HEIGHT_MAX 152
#define EPD_WEIGHT_MAX 296



typedef enum
{
    COMMAND=0,DATA=1
} DataType;
struct EPD_InfDef
{
	
	
	uint16_t WEIGHT_MAX ;
	uint16_t HEIGHT_MAX ;
	
	uint8_t IsSelect;
	DataType StateDC ;
	uint8_t IsSleep;
	uint8_t NeedUpdate ;
	uint8_t ErrorFalg;
} extern EPD_Inf;

void EPD_HReset();
uint8_t EPD_IsBusy();
void EPD_Sleep();
void EPD_SendByte(uint8_t data,DataType datatype);
void EPD_Display_Clear(void);
void EPD_Update();
void EPD_Init2(void);
void EPD_Init();
void EPD_XYCntSet(uint16_t x,uint16_t y);
#endif
