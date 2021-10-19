#include "main.h"

void SH30x_I2C_Start(SIMIIC_name_t IICNum)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1(IICNum);
	Soft_Delay(1);
	I2C_SCL_1(IICNum);
	Soft_Delay(4);
	I2C_SDA_0(IICNum);
	Soft_Delay(4);
	I2C_SCL_0(IICNum);
	Soft_Delay(1);
}


void SH30x_I2C_Stop(SIMIIC_name_t IICNum)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0(IICNum);
	Soft_Delay(2);
	I2C_SCL_1(IICNum);
	Soft_Delay(1);
	I2C_SDA_1(IICNum);
	Soft_Delay(2);
}


void SH30x_I2C_SendByte(SIMIIC_name_t IICNum , uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1(IICNum);
		}
		else
		{
			I2C_SDA_0(IICNum);
		}
		Soft_Delay(1);
		I2C_SCL_1(IICNum);
		_ucByte <<= 1;	/* 左移一个bit */
		Soft_Delay(2);
		I2C_SCL_0(IICNum);
		Soft_Delay(1);
		if (i == 7)
		{
			 I2C_SDA_1(IICNum); // 释放总线
		}
	}
}

uint8_t SH30x_I2C_ReadByte(SIMIIC_name_t IICNum)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		Soft_Delay(2);
		I2C_SCL_1(IICNum);
		Soft_Delay(1);
		value <<= 1;
		if (I2C_READ_SDA(IICNum))
		{
			value++;
		}
		Soft_Delay(1);
		I2C_SCL_0(IICNum);
	}
	return value;
}


uint8_t SH30x_I2C_WaitAck(SIMIIC_name_t IICNum)
{
	uint8_t re;

	I2C_SDA_1(IICNum);	/* CPU释放SDA总线 */
	Soft_Delay(2);
	I2C_SCL_1(IICNum);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Soft_Delay(1);
	re = I2C_READ_SDA(IICNum);	/* CPU读取SDA口线状态 */
	Soft_Delay(1);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
	return re;
}

void SH30x_I2C_Ack(SIMIIC_name_t IICNum)
{
	I2C_SDA_0(IICNum);	/* CPU驱动SDA = 0 */
	Soft_Delay(1);
	I2C_SCL_1(IICNum);	/* CPU产生1个时钟 */
	Soft_Delay(2);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
	I2C_SDA_1(IICNum);	/* CPU释放SDA总线 */
}


void SH30x_I2C_NAck(SIMIIC_name_t IICNum)
{
	I2C_SDA_1(IICNum);	/* CPU驱动SDA = 1 */
	Soft_Delay(1);
	I2C_SCL_1(IICNum);	/* CPU产生1个时钟 */
	Soft_Delay(2);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
}

void SH30x_IICPort_Init(SIMIIC_name_t IICNum)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	//一线通
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;

	GPIO_InitStruct.Pin = IIC_GPIO_INFO[IICNum].SCL_PIN;
	LL_GPIO_Init(IIC_GPIO_INFO[IICNum].SCL_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = IIC_GPIO_INFO[IICNum].SDA_PIN;
	LL_GPIO_Init(IIC_GPIO_INFO[IICNum].SDA_PORT, &GPIO_InitStruct);
	SH30x_I2C_Stop(IICNum);
	
}






