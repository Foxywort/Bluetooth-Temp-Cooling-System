#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"
#include "UART.h"
#include "Fan.h"


float T;
char Sign;

void main()
{
	DS18B20_ConvertT(); // 上电先转换一次温度，防止第一次读数据错误
	Delay(1000);		// 等待转换完成
	LCD_Init();
	LCD_ShowString(1, 1, "Temperature:");
	UART_Init();
	UART_SendString("Temperature:\r\n");
	// 初始化风扇模块
	Fan_Init();
	Fan_SetLevel(0);
	while (1)
	{
		DS18B20_ConvertT();	 // 转换温度
		T = DS18B20_ReadT(); // 读取温度
		if (T < 0)			 // 如果温度小于0
		{
			LCD_ShowChar(2, 1, '-');
			Sign = '-';
			T = -T;
		}
		else // 如果温度大于等于0
		{
			LCD_ShowChar(2, 1, '+');
			Sign = '+';
		}
		LCD_ShowNum(2, 2, T, 3);								  // 显示温度整数部分
		LCD_ShowChar(2, 5, '.');								  // 显示小数点
		LCD_ShowNum(2, 6, (unsigned long)(T * 10000) % 10000, 4); // 显示温度小数部分
		// 显示单位：°C（HD44780度符号代码0xDF）
		LCD_ShowChar(2, 10, (char)0xDF);
		LCD_ShowChar(2, 11, 'C');

		UART_SendByte(Sign);
		{
			unsigned int intPart = (unsigned int)T;
			UART_SendByte(intPart / 100 % 10 + '0');
			UART_SendByte(intPart / 10 % 10 + '0');
			UART_SendByte(intPart % 10 + '0');
		}
		UART_SendByte('.');
		{
			unsigned int frac = (unsigned long)(T * 10000) % 10000;
			UART_SendByte(frac / 1000 % 10 + '0');
			UART_SendByte(frac / 100 % 10 + '0');
			UART_SendByte(frac / 10 % 10 + '0');
			UART_SendByte(frac % 10 + '0');
		}

		UART_SendByte(0xA1); // GBK 编码的高八位
		UART_SendByte(0xE3); // GBK 编码的低八位
		UART_SendByte('C');

		// 根据温度设置风扇档位并输出档位与占空比百分比
		{
			unsigned char level;
			if (T < 25.0)
			{
				level = 0;
			}
			else if (T < 27.0)
			{
				level = 1;
			}
			else if (T < 30.0)
			{
				level = 2;
			}
			else
			{
				level = 3;
			}
			Fan_SetLevel(level);

			UART_SendString(" Level:");
			UART_SendByte(level + '0');
			UART_SendString(" Speed:");
			{
				unsigned int sp = Fan_GetDutyPercent(); // 0~100%
				UART_SendByte(sp / 100 % 10 + '0');
				UART_SendByte(sp / 10 % 10 + '0');
				UART_SendByte(sp % 10 + '0');
			}
			UART_SendByte('%');
		}

		UART_SendByte('\r');
		UART_SendByte('\n');
		Delay(1000); // 每次发送后延迟1秒
	}
}
