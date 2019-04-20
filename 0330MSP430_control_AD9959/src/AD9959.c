/*
 * AD9959.c
 *
 *  Created on: 2019年3月30日
 *      Author: seven
 */
#include <MSP430G2553.h>
#include <AD9959.h>


unsigned char CSR_DATA0[1] = {0x10};      // 开 CH0
unsigned char CSR_DATA1[1] = {0x20};      // 开 CH1
unsigned char CSR_DATA2[1] = {0x40};      // 开 CH2
unsigned char CSR_DATA3[1] = {0x80};      // 开 CH3
unsigned char FR2_DATA[2] = {0x00,0x00};//default Value = 0x0000
unsigned char CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302
unsigned char CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360
unsigned char LSRR_DATA[2] = {0x00,0x00};//default Value = 0x----
unsigned char RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
unsigned char FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

unsigned int SinFre[4] = {8000, 5000, 5000, 5000};
unsigned int SinAmp[4] = {1023, 1023, 1023, 1023};
unsigned int SinPhr[4] = {0, 4095, 4095*3, 4095*2};

//AD9959初始化
void Init_AD9959(void)
{
 	unsigned char FR1_DATA[3] = {0xD0,0x00,0x00};//20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;

 	//初始化IO
 	CS;
 	SCLK;
 	UPDATE;
 	PS0;
 	SDIO0;
 	AD9959_PWR;
 	Reset;
 	//

	Intserve();  //IO口初始化
	IntReset();  //AD9959复位

	WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//写功能寄存器1
//  WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
//  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
//  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
//  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
//  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
//  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
//  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
   //写入初始频率
	Write_frequence(3,SinFre[3]);
	Write_frequence(0,SinFre[0]);
	Write_frequence(1,SinFre[1]);
	Write_frequence(2,SinFre[2]);

////	Write_frequence(3,50);
////	Write_frequence(0,50);
////	Write_frequence(1,50);
////	Write_frequence(2,50);

	Write_Phase(3, SinPhr[3]);
	Write_Phase(0, SinPhr[0]);
	Write_Phase(1, SinPhr[1]);
	Write_Phase(2, SinPhr[2]);

	Write_Amplitude(3, SinAmp[3]);
	Write_Amplitude(0, SinAmp[0]);
	Write_Amplitude(1, SinAmp[1]);
	Write_Amplitude(2, SinAmp[2]);
}
/*/延时
void delay1(unsigned long length)
{
	unsigned long i=0;
	unsigned long length1 = length*1200000;
	for(i=0;i<length1;i++);
}*/
//IO口初始化
void Intserve(void)
{
	AD9959_PWR_0;
	CS_1;
	SCLK_0;
	UPDATE_0;
	PS0_0;
	SDIO0_0;
}
//AD9959复位
void IntReset(void)
{
	Reset_0;
	__delay_cycles(100);//delay1(1000);
	Reset_1;
	__delay_cycles(100);//delay1(3000);
	Reset_0;
}
 //AD9959更新数据
void IO_Update(void)
{
	UPDATE_0;
	__delay_cycles(100);//delay1(500);
	UPDATE_1;
	__delay_cycles(100);//delay1(1000);
	UPDATE_0;
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(unsigned char RegisterAddress,unsigned int NumberofRegisters,unsigned char *RegisterData,unsigned int temp)
{
	unsigned char	ControlValue = 0;
	unsigned char	ValueToWrite = 0;
	unsigned int	RegisterIndex = 0;
	unsigned int	i = 0;

	ControlValue = RegisterAddress;
	//写入地址
	SCLK_0;
	CS_0;
	for(i=0; i<8; i++)
	{
		SCLK_0;
		if(0x80 == (ControlValue & 0x80))
			SDIO0_1;
		else
			SDIO0_0;
		__delay_cycles(100);//delay1(300);
		SCLK_1;
		__delay_cycles(100);//delay1(300);
		ControlValue <<= 1;
	}
	SCLK_0;
	//写入数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_0;
			if(0x80 == (ValueToWrite & 0x80))
				SDIO0_1;
			else
				SDIO0_0;
			__delay_cycles(100);//delay1(300);
			SCLK_1;
			__delay_cycles(100);//delay1(300);
			ValueToWrite <<= 1;
		}
		SCLK_0;
	}
	if(temp==1)
		IO_Update();
	CS_1;
}
/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void Write_frequence(unsigned char Channel,unsigned int Freq)
{
	  unsigned char CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  unsigned int Temp;
	  Temp=(unsigned int)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=500000000/(2^32)
	  CFTW0_DATA[3]=(unsigned char)Temp;
	  CFTW0_DATA[2]=(unsigned char)(Temp>>8);
	  CFTW0_DATA[1]=(unsigned char)(Temp>>16);
	  CFTW0_DATA[0]=(unsigned char)(Temp>>24);
	  if(Channel==0)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写入CH0通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
	  }
	  else if(Channel==1)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写入CH1通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率
	  }
	  else if(Channel==2)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写入CH2通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率
	  }
	  else if(Channel==3)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写入CH3通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);//CTW0 address 0x04.输出CH3设定频率
	  }

}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void Write_Amplitude(unsigned char Channel, unsigned short int Ampli)
{
	unsigned short int A_temp;//=0x23ff;
	unsigned char ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout

    A_temp=Ampli|0x1000;
	ACR_DATA[2] = (unsigned char)A_temp;  //低位数据
	ACR_DATA[1] = (unsigned char)(A_temp>>8); //高位数据
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
}
/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void Write_Phase(unsigned char Channel,unsigned short int Phase)
{
	unsigned short int P_temp=0;
	P_temp=(unsigned short int)Phase;
	CPOW0_DATA[1]=(unsigned char)P_temp;
	CPOW0_DATA[0]=(unsigned char)(P_temp>>8);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	}
}
