#include "stm32g4xx.h"
#include "EPD_HW.h"
//graphic buffer
uint8_t GraphicRAM_BW[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8]={};
uint8_t GraphicRAM_R[EPD_WEIGHT_MAX*EPD_HEIGHT_MAX/8]={};
typedef enum COLOR
{
	BLACK=0,
	WHITE=1,
	RED=2,
	EMP=3
}UI_COLOR_TYPE;
//write a pixel of buffer
void UI_DrawPixel(uint16_t x,uint16_t y,UI_COLOR_TYPE color)
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
		
		//What are you fucking doing?
		
	}
	return ;
}
//write graphic data to EPD
void UI_UpdateGRAM()
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
	
	return ;
}
//update the whole screen
void UI_Update_All()
{
	UI_UpdateGRAM();
	
	EPD_XYSEPositionSet(0,(EPD_Inf.WEIGHT_MAX-1),0,(EPD_Inf.HEIGHT_MAX/8-1));
	EPD_XYCntSet(0,0);
	
	EPD_Update_All();
}
//update a part of the screen
void UI_Update_Part(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye)
{
	UI_UpdateGRAM();
	
	EPD_XYSEPositionSet(xs,ys,xe,ye);
	EPD_XYCntSet(xs,ys);
	
	EPD_Update_Part();
	
}
//only clear the buffer
void UI_Init()
{
	for(uint32_t c1=0;c1<EPD_Inf.WEIGHT_MAX*EPD_Inf.HEIGHT_MAX/8;c1++)
	{
			GraphicRAM_BW[c1]=0xFF;
			GraphicRAM_R[c1]=0x00;
	}
	return ;
}



//======
//I believe you can understand my code here....It is very simple.
void UI_DrawBox_FILLED(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE color)
{
	for(uint16_t cnt=sx;cnt<=ex;cnt++)
	{
		for(uint16_t cnt2=sy;cnt2<=ey;cnt2++)
	{
		UI_DrawPixel(cnt,cnt2,color);
	}
	}
	return ;
}
void UI_DrawBox_EMPTY(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,UI_COLOR_TYPE coloroutside,UI_COLOR_TYPE colorinside,uint16_t edgewide)
{
	UI_DrawBox_FILLED(sx,sy,ex,sy+edgewide,coloroutside);
	UI_DrawBox_FILLED(sx,sy,sx+edgewide,ey,coloroutside);
	UI_DrawBox_FILLED(ex-edgewide,sy,ex,ey,coloroutside);
	UI_DrawBox_FILLED(sx,ey-edgewide,ex,ey,coloroutside);
	UI_DrawBox_FILLED(sx+edgewide,sy+edgewide,ex-edgewide,ey-edgewide,colorinside);
	return ;
}

