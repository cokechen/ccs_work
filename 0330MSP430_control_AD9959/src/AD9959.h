/*
 * AD9959.h
 *
 *  Created on: 2019年3月30日
 *      Author: seven
 */

#ifndef SRC_AD9959_H_
#define SRC_AD9959_H_

#include<MSP430G2553.h>
//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器
//AD9959管脚宏定义
#define CS			P1DIR|=BIT1
#define SCLK		P2DIR|=BIT1
#define UPDATE		P2DIR|=BIT0
#define PS0			P1DIR|=BIT2
//#define PS1			P1DIR|=2
//#define PS2			P2DIR|=10
//#define PS3			PCout|=0
#define SDIO0		P1DIR|=BIT3
//#define SDIO1		P1DIR|=4
//#define SDIO2		P1DIR|=3
//#define SDIO3		P1DIR|=8
#define AD9959_PWR	P1DIR|=BIT4
#define Reset		P1DIR|=BIT5
//AD9959管脚输出操作
#define AD9959_PWR_0 	P1OUT &= ~BIT4
#define CS_0 		    P1OUT &= ~BIT1
#define SCLK_0 			P2OUT &= ~BIT1
#define UPDATE_0 		P2OUT &= ~BIT0
#define PS0_0 			P1OUT &= ~BIT2
#define SDIO0_0 		P1OUT &= ~BIT3
#define Reset_0			P1OUT &= ~BIT5
#define AD9959_PWR_1 	P1OUT |= BIT4
#define CS_1 			P1OUT |= BIT1
#define SCLK_1 			P2OUT |= BIT1
#define UPDATE_1 		P2OUT |= BIT0
#define PS0_1 			P1OUT |= BIT2
#define SDIO0_1 		P1OUT |= BIT3
#define Reset_1			P1OUT |= BIT5
//#define delay_clk       __delay_cycles(1000000)

void delay1 (unsigned long length);
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(unsigned char RegisterAddress, unsigned int NumberofRegisters, unsigned char *RegisterData,unsigned int temp);
void Init_AD9959(void);
void Write_frequence(unsigned char Channel,unsigned int Freq);
void Write_Amplitude(unsigned char Channel, unsigned short int Ampli);
void Write_Phase(unsigned char Channel,unsigned short int Phase);



#endif /* SRC_AD9959_H_ */
