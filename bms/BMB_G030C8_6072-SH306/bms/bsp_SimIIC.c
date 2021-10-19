#include "main.h"

void SH30x_I2C_Start(SIMIIC_name_t IICNum)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
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

	/* �ȷ����ֽڵĸ�λbit7 */
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
		_ucByte <<= 1;	/* ����һ��bit */
		Soft_Delay(2);
		I2C_SCL_0(IICNum);
		Soft_Delay(1);
		if (i == 7)
		{
			 I2C_SDA_1(IICNum); // �ͷ�����
		}
	}
}

uint8_t SH30x_I2C_ReadByte(SIMIIC_name_t IICNum)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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

	I2C_SDA_1(IICNum);	/* CPU�ͷ�SDA���� */
	Soft_Delay(2);
	I2C_SCL_1(IICNum);	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	Soft_Delay(1);
	re = I2C_READ_SDA(IICNum);	/* CPU��ȡSDA����״̬ */
	Soft_Delay(1);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
	return re;
}

void SH30x_I2C_Ack(SIMIIC_name_t IICNum)
{
	I2C_SDA_0(IICNum);	/* CPU����SDA = 0 */
	Soft_Delay(1);
	I2C_SCL_1(IICNum);	/* CPU����1��ʱ�� */
	Soft_Delay(2);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
	I2C_SDA_1(IICNum);	/* CPU�ͷ�SDA���� */
}


void SH30x_I2C_NAck(SIMIIC_name_t IICNum)
{
	I2C_SDA_1(IICNum);	/* CPU����SDA = 1 */
	Soft_Delay(1);
	I2C_SCL_1(IICNum);	/* CPU����1��ʱ�� */
	Soft_Delay(2);
	I2C_SCL_0(IICNum);
	Soft_Delay(2);
}

void SH30x_IICPort_Init(SIMIIC_name_t IICNum)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	//һ��ͨ
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






