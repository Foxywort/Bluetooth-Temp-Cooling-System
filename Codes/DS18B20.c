#include <REGX52.H>
#include "OneWire.h"

//DS18B20指令
#define DS18B20_SKIP_ROM			0xCC
#define DS18B20_CONVERT_T			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE

/**
  * @brief  DS18B20开始温度变换
  * @param  无
  * @retval 无
  */
void DS18B20_ConvertT(void)
{
	unsigned char ea = EA;  // 备份全局中断使能
	EA = 0;                 // 关闭中断以保护单总线时序
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
	EA = ea;                // 恢复原中断状态（转换过程可在后台进行）
}

/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval 温度数值
  */
float DS18B20_ReadT(void)
{
	unsigned char TLSB,TMSB;
	int Temp;
	float T;
	{
		unsigned char ea = EA;  // 备份并关闭中断，保证读取窗口不被打断
		EA = 0;
		OneWire_Init();
		OneWire_SendByte(DS18B20_SKIP_ROM);
		OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
		TLSB=OneWire_ReceiveByte();
		TMSB=OneWire_ReceiveByte();
		EA = ea;              // 读取完成，恢复中断
	}
	Temp=(TMSB<<8)|TLSB;
	T=Temp/16.0;
	return T;
}
