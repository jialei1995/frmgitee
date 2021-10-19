#ifndef __SIMIIC_H_
#define	__SIMIIC_H_

#include "main.h"


#define	IIC1SCL_Pin  LL_GPIO_PIN_11
#define	IIC1SCL_Port GPIOA
#define	IIC1SDA_Pin  LL_GPIO_PIN_12
#define	IIC1SDA_Port GPIOA

#define	IIC2SCL_Pin  LL_GPIO_PIN_6
#define	IIC2SCL_Port GPIOB
#define	IIC2SDA_Pin  LL_GPIO_PIN_7
#define	IIC2SDA_Port GPIOB

#define	IIC3SCL_Pin  LL_GPIO_PIN_10
#define	IIC3SCL_Port GPIOB
#define	IIC3SDA_Pin  LL_GPIO_PIN_11
#define	IIC3SDA_Port GPIOB



typedef enum
{
	SIMIIC_1     = 0,
	SIMIIC_2   ,
	SIMIIC_3   ,
	SIMIIC_NULL  
} SIMIIC_name_t;
typedef struct
{
	GPIO_TypeDef * SCL_PORT;
	uint32_t SCL_PIN;
	GPIO_TypeDef * SDA_PORT;
	uint32_t SDA_PIN;
}IIC_GPIO_ST;

static IIC_GPIO_ST IIC_GPIO_INFO[]=
{
	{IIC1SCL_Port,IIC1SCL_Pin,IIC1SDA_Port,IIC1SDA_Pin},
	{IIC2SCL_Port,IIC2SCL_Pin,IIC2SDA_Port,IIC2SDA_Pin},
	{IIC3SCL_Port,IIC3SCL_Pin,IIC3SDA_Port,IIC3SDA_Pin}
};

#define  I2C_SCL_1(x) 	GPIO_WritePin(IIC_GPIO_INFO[x].SCL_PORT, IIC_GPIO_INFO[x].SCL_PIN, GPIO_PIN_SET)
#define  I2C_SCL_0(x) 	GPIO_WritePin(IIC_GPIO_INFO[x].SCL_PORT, IIC_GPIO_INFO[x].SCL_PIN, GPIO_PIN_RESET)
#define  I2C_SDA_1(x) 	GPIO_WritePin(IIC_GPIO_INFO[x].SDA_PORT, IIC_GPIO_INFO[x].SDA_PIN, GPIO_PIN_SET)
#define  I2C_SDA_0(x) 	GPIO_WritePin(IIC_GPIO_INFO[x].SDA_PORT, IIC_GPIO_INFO[x].SDA_PIN, GPIO_PIN_RESET)
#define  I2C_READ_SDA(x) 	GPIO_ReadPin(IIC_GPIO_INFO[x].SDA_PORT, IIC_GPIO_INFO[x].SDA_PIN)


void SH30x_I2C_Start(SIMIIC_name_t IICNum);


void SH30x_I2C_Stop(SIMIIC_name_t IICNum);


void SH30x_I2C_SendByte(SIMIIC_name_t IICNum , uint8_t _ucByte);

uint8_t SH30x_I2C_ReadByte(SIMIIC_name_t IICNum);


uint8_t SH30x_I2C_WaitAck(SIMIIC_name_t IICNum);

void SH30x_I2C_Ack(SIMIIC_name_t IICNum);


void SH30x_I2C_NAck(SIMIIC_name_t IICNum);

void SH30x_IICPort_Init(SIMIIC_name_t IICNum);

#endif /* __ADC_H */

