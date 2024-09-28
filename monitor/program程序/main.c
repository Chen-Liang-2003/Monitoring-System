#include <reg52.h>
#include <intrins.h> 

#define uchar unsigned char		// 以后unsigned char就可以用uchar代替
#define uint  unsigned int		// 以后unsigned int 就可以用uint 代替

sbit ADC_CS   = P1^2; 				// ADC0832的CS引脚
sbit ADC_CLK = P1^1; 				// ADC0832的CLK引脚
sbit ADC_DAT  = P1^0; 				// ADC0832的DI/DO引脚

sbit ADC_CS2   = P1^5; 				// ADC0832的CS引脚
sbit ADC_CLK2 = P1^4; 				// ADC0832的CLK引脚
sbit ADC_DAT2  = P1^3; 				// ADC0832的DI/DO引脚
//LCD定义
#define LCD_DB P0 
sbit LCD_RS=P2^0;   
sbit LCD_RW=P2^1;  
sbit LCD_E=P2^2;  
sbit FMQ=P3^0;  
sbit K1=P3^5;  
sbit K2=P3^6;
bit up=1;
bit flag=1;
uint set=20;
uchar numR,numG,numB,num;		
/*-------------------------------------
			   函数申明
---------------------------------------*/
void LCD_init(void);                          //初始化函数 
void LCD_write_command(uchar command);        //写指令函数 
void LCD_write_data(uchar dat);               //写数据函数  
void LCD_disp_char(uchar x,uchar y,uchar dat);//在某个屏幕位置上显示一个字符,X（0-15),y(1-2)  
void LCD_disp_str(uchar x,uchar y,uchar *str); //LCD1602显示字符串函数 
void delay_n10us(uint n);                     //延时函数
/*-------------------------------------
			   LCD1602子程序
---------------------------------------*/
void LCD_init(void) 
{  
	delay_n10us(10);  
	LCD_write_command(0x38);//设置8位格式，2行，5x7 
	delay_n10us(10);  LCD_write_command(0x0c);//开显示，关光标，不闪烁 
	delay_n10us(10);  LCD_write_command(0x06);//设定输入方式，增量不移位 
	delay_n10us(10);  LCD_write_command(0x01);//清除屏幕显示  
	delay_n10us(100);       //延时清屏，延时函数，延时约n个10us 
}

void LCD_write_command(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=0;         //指令 
	LCD_RW=0;         //写入 
	LCD_E=1;          //允许 
	LCD_DB=dat;  delay_n10us(10);  //实践证明，我的LCD1602上，用for循环1次就能完成普通写指令。 
	LCD_E=0;  delay_n10us(10);  //实践证明，我的LCD1602上，用for循环1次就能完成普通写指令。 
}

void LCD_write_data(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=1;          //数据 
	LCD_RW=0;		   //写入
	LCD_E=1;           //允许 
	LCD_DB=dat; delay_n10us(10);
	 LCD_E=0;  delay_n10us(10); 
}

void LCD_disp_char(uchar x,uchar y,uchar dat) 
{    
	uchar address;   
	if(y==1)           
	address=0x80+x;  
	else           
	address=0xc0+x;    
	LCD_write_command(address);   
	LCD_write_data(dat); 
}

void LCD_disp_str(uchar x,uchar y,uchar *str) 
{    
	uchar address;   
	if(y==1)           
	address=0x80+x;   
	else           
	address=0xc0+x;    
	LCD_write_command(address);   
	while(*str!='\0')   
		{       
		LCD_write_data(*str);
		str++;
		}
}

void delay_n10us(uint n)  //延时 
{                 
	uint i;                    
	for(i=n;i>0;i--)            
	 {         
	 nop_();_nop_();_nop_();_nop_();_nop_();_nop_();    
	 } 
}






/*********************************************************/
// ADC0832的时钟脉冲
/*********************************************************/
void WavePlus()
{
	_nop_();
	ADC_CLK = 1;
	_nop_();
	ADC_CLK = 0;
}



/*********************************************************/
// 获取指定通道的A/D转换结果
/*********************************************************/
uchar Get_ADC0832(uchar chanel)
{ 
	uchar i;
	uchar dat1=0;
	uchar dat2=0;
	
	ADC_CLK = 0;				// 电平初始化
	ADC_DAT = 1;
	_nop_();
	ADC_CS = 0;
	WavePlus();					// 起始信号 
	ADC_DAT = 1;
	WavePlus();					// 通道选择的第一位
	ADC_DAT = chanel;      
	WavePlus();					// 通道选择的第二位
	ADC_DAT = 1;
	
	for(i=0;i<8;i++)		// 第一次读取
	{
		dat1<<=1;
		WavePlus();
		if(ADC_DAT)
			dat1=dat1|0x01;
		else
			dat1=dat1|0x00;
	}
	
	for(i=0;i<8;i++)		// 第二次读取
	{
		dat2>>= 1;
		if(ADC_DAT)
			dat2=dat2|0x80;
		else
			dat2=dat2|0x00;
		WavePlus();
	}
	
	_nop_();						// 结束此次传输
	ADC_DAT = 1;
	ADC_CLK = 1;
	ADC_CS  = 1;   

	if(dat1==dat2)			// 返回采集结果
		return dat1;
	else
		return 0;
} 








/*********************************************************/
// ADC0832的时钟脉冲
/*********************************************************/
void WavePlus2()
{
	_nop_();
	ADC_CLK2 = 1;
	_nop_();
	ADC_CLK2 = 0;
}



/*********************************************************/
// 获取指定通道的A/D转换结果
/*********************************************************/
uchar Get_ADC08322(uchar chanel)
{ 
	uchar i;
	uchar dat1=0;
	uchar dat2=0;
	
	ADC_CLK2 = 0;				// 电平初始化
	ADC_DAT2 = 1;
	_nop_();
	ADC_CS2 = 0;
	WavePlus2();					// 起始信号 
	ADC_DAT2 = 1;
	WavePlus2();					// 通道选择的第一位
	ADC_DAT2 = chanel;      
	WavePlus2();					// 通道选择的第二位
	ADC_DAT2 = 1;
	
	for(i=0;i<8;i++)		// 第一次读取
	{
		dat1<<=1;
		WavePlus2();
		if(ADC_DAT2)
			dat1=dat1|0x01;
		else
			dat1=dat1|0x00;
	}
	
	for(i=0;i<8;i++)		// 第二次读取
	{
		dat2>>= 1;
		if(ADC_DAT2)
			dat2=dat2|0x80;
		else
			dat2=dat2|0x00;
		WavePlus2();
	}
	
	_nop_();						// 结束此次传输
	ADC_DAT2 = 1;
	ADC_CLK2 = 1;
	ADC_CS2  = 1;   

	if(dat1==dat2)			// 返回采集结果
		return dat1;
	else
		return 0;
} 




void LCD() 
{
	LCD_disp_char(1,1,numR/100+'0'); //显示 
	LCD_disp_char(2,1,numR/10%10+'0'); //显示 
	LCD_disp_char(3,1,numR%10+'0'); //显示
	
	LCD_disp_char(6,1,numG/100+'0'); //显示 
	LCD_disp_char(7,1,numG/10%10+'0'); //显示 
	LCD_disp_char(8,1,numG%10+'0'); //显示
	
	LCD_disp_char(11,1,numB/100+'0'); //显示 
	LCD_disp_char(12,1,numB/10%10+'0'); //显示 
	LCD_disp_char(13,1,numB%10+'0'); //显示
	

	LCD_disp_char(8,2,num/10+'0'); //显示 
	LCD_disp_char(9,2,num%10+'0'); //显示 
	
	
	LCD_disp_char(14,2,set/10+'0'); //显示 
	LCD_disp_char(15,2,set%10+'0'); //显示 
}



void key()
{
	if(K1==0)
	{
		if(flag==1)
		{
			flag=0;
			if(set<99)
			{
				set++;
			}
		}
	}
	else if(K2==0)
	{
		if(flag==1)
		{
			flag=0;
			if(set>0)
			{
				set--;
			}
		}
	}
	else
	{
		flag=1;
	}
}




/********************************************************
主函数
********************************************************/
void main(void)
{
	LCD_init();//LCD初始化              
	LCD_disp_str(0,1,"R000 G000 B000 "); //显示 
	LCD_disp_str(0,2,"      n:00 s:00"); //显示 
	
	while(1)
	{
		numR=Get_ADC0832(0);			
		numG=Get_ADC0832(1);			
		numB=Get_ADC08322(0);						
		LCD();
		key();
		
		if(numR==255&&numG==255&&numB==255)
		{
			if(up==1)
			{
				up=0;
				LCD_disp_str(0,2,"White"); //显示 
				if(num<99)
				{
					num++;
				}
			}
		}
		else
		{
			up=1;
			LCD_disp_str(0,2,"     "); //显示 
		}
		
		if(num>=set)
		{
			FMQ=0;
		}
		else
		{
			FMQ=1;
		}
	}
}




