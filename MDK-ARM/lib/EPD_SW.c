#include "stm32g4xx.h"
#include "EPD_HW.h"
uint8_t GraphicRAM_BW[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8]={};
uint8_t GraphicRAM_R[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8]={};
typedef enum COLOR
{
	BLACK=0,
	WHITE=1,
	RED=2,
	EMP=3
}UI_COLOR_TYPE;
void UIDrawPixel(uint16_t x,uint16_t y,UI_COLOR_TYPE color)
{
	switch(color)
	{
		case WHITE:
		GraphicRAM_BW[x*EPD_Inf.HEIGHT_MAX/8+y/8]|=(1<<(7-y%8));//set bit
		GraphicRAM_R[x*EPD_Inf.HEIGHT_MAX/8+y/8]&=~(1<<(7-y%8));//reset bit
		break;
		case BLACK:
		GraphicRAM_BW[x*EPD_Inf.HEIGHT_MAX/8+y/8]&=~(1<<(7-y%8));//reset bit
		GraphicRAM_R[x*EPD_Inf.HEIGHT_MAX/8+y/8]&=~(1<<(7-y%8));//reset bit
		break;
		case RED:
		GraphicRAM_BW[x*EPD_Inf.HEIGHT_MAX/8+y/8]&=~(1<<(7-y%8));//reset bit
		GraphicRAM_R[x*EPD_Inf.HEIGHT_MAX/8+y/8]|=(1<<(7-y%8));//set bit
		break;
		case EMP:
		break;
		defaullt:
		//What are you fucking doing?
		break;
	}
	return ;
}

void UIUpdateGRAM()
{
	EPD_SendByte(0x24,COMMAND);
	for(int c1=EPD_Inf.WEIGHT_MAX-1;c1>=0;c1--)
	{
		for(int c2=0;c2<EPD_Inf.HEIGHT_MAX/8;c2++)
		{
			EPD_SendByte(GraphicRAM_BW[c1*EPD_Inf.HEIGHT_MAX/8+c2],DATA);
		}
	}
	EPD_SendByte(0x26,COMMAND);
	for(int c1=EPD_Inf.WEIGHT_MAX-1;c1>=0;c1--)
	{
		for(int c2=0;c2<EPD_Inf.HEIGHT_MAX/8;c2++)
		{
			EPD_SendByte(GraphicRAM_R[c1*EPD_Inf.HEIGHT_MAX/8+c2],DATA);
		}
	}
	EPD_Update();
	return ;
}
void UI_Init()
{
	for(uint32_t c1=0;c1<EPD_Inf.WEIGHT_MAX*EPD_Inf.HEIGHT_MAX/8;c1++)
	{
			GraphicRAM_BW[c1]=0xFF;
			GraphicRAM_R[c1]=0x00;
	}
	return ;
}



//===
void UIDrawBox_FILLED(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE color)
{
	for(uint16_t cnt=sx;cnt<=ex;cnt++)
	{
		for(uint16_t cnt2=sy;cnt2<=ey;cnt2++)
	{
		UIDrawPixel(cnt,cnt2,color);
	}
	}
	return ;
}
void UIDrawBox_EMPTY(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE coloro,UI_COLOR_TYPE colorin,uint16_t edgewide)
{
	UIDrawBox_FILLED(sx,sy,ex,sy+edgewide,coloro);
	UIDrawBox_FILLED(sx,sy,sx+edgewide,ey,coloro);
	UIDrawBox_FILLED(ex-edgewide,sy,ex,ey,coloro);
	UIDrawBox_FILLED(sx,ey-edgewide,ex,ey,coloro);
	UIDrawBox_FILLED(sx+edgewide,sy+edgewide,ex-edgewide,ey-edgewide,colorin);
	return ;
}

