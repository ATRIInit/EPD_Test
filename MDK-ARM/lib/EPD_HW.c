#include "stm32g4xx.h"
#include "Delay.h"
#include "spi.h"
#include "gpio.h"

#define PIN_CS GPIOC,GPIO_PIN_10
#define PIN_DC GPIOB,GPIO_PIN_4
#define PIN_RES GPIOB,GPIO_PIN_6
#define PIN_BUSY GPIOA,GPIO_PIN_11

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
} EPD_Inf;



void EPD_HReset()
{
	HAL_GPIO_WritePin(PIN_RES,0);
	Delay_ms(10);
	HAL_GPIO_WritePin(PIN_RES,1);
	Delay_ms(10);
	return ;
}
void EPD_FreeCS()
{
	HAL_GPIO_WritePin(PIN_CS,1);
	return ;
}
uint8_t EPD_IsBusy()
{
	return HAL_GPIO_ReadPin(PIN_BUSY);
}
void EPD_SendByte(uint8_t data,DataType datatype)
{
	uint32_t TimCnt=0;
	if(!EPD_Inf.IsSelect)
		{
			HAL_GPIO_WritePin(PIN_CS,(GPIO_PinState)0);
			Delay_us(2);
		}
	if(EPD_Inf.StateDC!=datatype)
		{
			HAL_GPIO_WritePin(PIN_DC,(GPIO_PinState)datatype);
			EPD_Inf.StateDC=datatype;
			Delay_us(2);
		}
	while(EPD_IsBusy())
		{
			if(TimCnt==0x1312D00/*wait 20s*/)
				{
					EPD_Inf.ErrorFalg = 1;
					break;
				}

			TimCnt++;
			Delay_us(1);
		}

	HAL_SPI_Transmit(&hspi1,(uint8_t*)&data,sizeof(data),200);
	return ;
}
void EPD_Sleep()
{
	EPD_SendByte(0x10,COMMAND);
	EPD_SendByte(0x01,DATA);
	EPD_Inf.IsSleep = 1;
	return ;
}
//===============================================================DRAW
void EPD_XYCntSet(uint16_t x,uint16_t y)
{
	EPD_SendByte(0x4E,COMMAND);//X
	EPD_SendByte(x,DATA);
	EPD_SendByte(0x4F,COMMAND);//Y
	EPD_SendByte(y&0xFF,DATA);
	EPD_SendByte(y>>8,DATA);
}


void EPD_Update()
{
	
	EPD_SendByte(0x22,COMMAND);
	EPD_SendByte(0xF4,DATA);
	EPD_SendByte(0x20,COMMAND);
	return;
}


void EPD_Init()
{


	EPD_HReset();
	//SoftReset
	EPD_SendByte(0x12,COMMAND);
	Delay_ms(10);
	//NoSleep
	EPD_SendByte(0x10,COMMAND);
	EPD_SendByte(0x00,DATA);
	//BorderWavefrom
	EPD_SendByte(0x3C,COMMAND);
	EPD_SendByte(0x05,DATA);

	//Gate Driver Output
	EPD_SendByte(0x01,COMMAND);
	EPD_SendByte((EPD_Inf.WEIGHT_MAX-1)%256,DATA);
	EPD_SendByte((EPD_Inf.WEIGHT_MAX-1)/256,DATA);
	EPD_SendByte(0x0,DATA);
	//XY Dir
	
	EPD_SendByte(0x11,COMMAND);
	EPD_SendByte(0x03,DATA);
	EPD_SendByte(0x44,COMMAND);
	EPD_SendByte(0x00,DATA);
	EPD_SendByte((EPD_Inf.HEIGHT_MAX/8-1),DATA);
	EPD_SendByte(0x45,COMMAND);
	EPD_SendByte(0,DATA);
	EPD_SendByte(0,DATA);
	EPD_SendByte((EPD_Inf.WEIGHT_MAX-1)%256,DATA);
	EPD_SendByte((EPD_Inf.WEIGHT_MAX-1)/256,DATA);
	//XY cnt reset
	EPD_SendByte(0x4E,COMMAND);//X
	EPD_SendByte(0x00,DATA);
	EPD_SendByte(0x4F,COMMAND);//Y
	EPD_SendByte(0x00,DATA);
	EPD_SendByte(0x00,DATA);
	//temperature sense

	EPD_SendByte(0x18,COMMAND);
	EPD_SendByte(0x80,DATA);
	//update ser
	EPD_SendByte(0x21,COMMAND);
	EPD_SendByte(0x00,DATA);
	EPD_SendByte(0x80,DATA);
	// Operating sequence set
	
	EPD_SendByte(0x22,COMMAND);
	EPD_SendByte(0x91,DATA);
	

	//EPD_SendByte(0x20,COMMAND);

	//

	return ;
}
void EPD_WR_REG(uint8_t data)
{
	EPD_SendByte(data,COMMAND);
}
void EPD_WR_DATA8(uint8_t data)
{
	EPD_SendByte(data,DATA);
}
void EPD_Init2(void)
{


	EPD_WR_REG(0x12);  //SWRESET

	
	EPD_WR_REG(0x3C); //BorderWavefrom
	EPD_WR_DATA8(0x05);
	
	EPD_WR_REG(0x01); //Driver output control      
	EPD_WR_DATA8((EPD_Inf.WEIGHT_MAX-1)%256);    
	EPD_WR_DATA8((EPD_Inf.WEIGHT_MAX-1)/256);
	EPD_WR_DATA8(0x00); 

	EPD_WR_REG(0x11); //data entry mode       
	EPD_WR_DATA8(0x01);
	EPD_WR_REG(0x44); //set Ram-X address start/end position   
	EPD_WR_DATA8(0x00);    
	EPD_WR_DATA8(EPD_Inf.HEIGHT_MAX/8-1);  
	EPD_WR_REG(0x45); //set Ram-Y address start/end position          
	EPD_WR_DATA8((EPD_Inf.WEIGHT_MAX-1)%256);
	EPD_WR_DATA8((EPD_Inf.WEIGHT_MAX-1)%256); 
  EPD_WR_DATA8(0x00); 
	EPD_WR_DATA8(0x00);
	EPD_WR_REG(0x21); //  Display update control
	EPD_WR_DATA8(0x00);		
  EPD_WR_DATA8(0x80);	
  EPD_WR_REG(0x18); //Read built-in temperature sensor
	EPD_WR_DATA8(0x80);	
	EPD_WR_REG(0x4E);   // set RAM x address count to 0;
	EPD_WR_DATA8(EPD_Inf.HEIGHT_MAX/8-1);    
	EPD_WR_REG(0x4F);   // set RAM y address count to 0X199;    
	EPD_WR_DATA8(0x00);
	EPD_WR_DATA8(0x00);

}
void EPD_Display_Clear(void)
{
  uint16_t i;
	EPD_WR_REG(0x3C);
	EPD_WR_DATA8(0x01);
	EPD_WR_REG(0x24);
	for(i=0;i<5624;i++)
	{
		EPD_WR_DATA8(0xFF);
	}	
	EPD_WR_REG(0x26);
	for(i=0;i<5624;i++)
	{
		EPD_WR_DATA8(0x00);
	}	
}


