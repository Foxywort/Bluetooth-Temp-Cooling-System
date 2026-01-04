#include <REGX52.H>
#include "UART.h"


// 蓝牙芯片的RXD连接 51单片机 的P3.1(TXD)
// 蓝牙芯片的TXD连接 51单片机 的P3.0(RXD)
#define FOSC       12000000UL   
#define UART_BAUD  9600         
#define UART_SMOD  0            


#if UART_SMOD==0
  #define TH1_RELOAD (256 - (FOSC/12/32/UART_BAUD))
#else
  #define TH1_RELOAD (256 - (FOSC/12/16/UART_BAUD))
#endif

void UART_Init(void)
{
    // 串口模式1，允许接收
    SCON = 0x50;        // SM0=0, SM1=1, REN=1

    // 设置 SMOD（波特率倍速）
    if (UART_SMOD) {
        PCON |= 0x80;   // SMOD = 1
    } else {
        PCON &= ~0x80;  // SMOD = 0
    }

    // Timer1 方式2（8位自动重装）作为波特率发生器
    TMOD = (TMOD & 0x0F) | 0x20; // 保留 Timer0，设置 Timer1 为方式2

    TH1 = (unsigned char)TH1_RELOAD;
    TL1 = (unsigned char)TH1_RELOAD;

    TR1 = 1;            // 启动 Timer1
}

void UART_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while(!TI);
    TI = 0;
}

void UART_SendString(char *String)
{
    unsigned int i = 0;
    while(String[i] != '\0')
    {
        UART_SendByte((unsigned char)String[i]);
        i++;
    }
}
