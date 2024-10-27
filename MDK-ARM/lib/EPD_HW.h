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

void EPD_HReset(void);
uint8_t EPD_IsBusy(void);
void EPD_Sleep(void);
void EPD_SendByte(uint8_t data,DataType datatype);
void EPD_Display_Clear(void);
void EPD_Update_All(void);
void EPD_Update_Part(void);
void EPD_Update_Part(void);
void EPD_XYSEPositionSet(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye);
void EPD_Init2(void);
void EPD_Init(void);
void EPD_XYCntSet(uint16_t x,uint16_t y);
#endif
