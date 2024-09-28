#include <reg52.h>
#include <intrins.h> 

#define uchar unsigned char		// �Ժ�unsigned char�Ϳ�����uchar����
#define uint  unsigned int		// �Ժ�unsigned int �Ϳ�����uint ����

sbit ADC_CS   = P1^2; 				// ADC0832��CS����
sbit ADC_CLK = P1^1; 				// ADC0832��CLK����
sbit ADC_DAT  = P1^0; 				// ADC0832��DI/DO����

sbit ADC_CS2   = P1^5; 				// ADC0832��CS����
sbit ADC_CLK2 = P1^4; 				// ADC0832��CLK����
sbit ADC_DAT2  = P1^3; 				// ADC0832��DI/DO����
//LCD����
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
			   ��������
---------------------------------------*/
void LCD_init(void);                          //��ʼ������ 
void LCD_write_command(uchar command);        //дָ��� 
void LCD_write_data(uchar dat);               //д���ݺ���  
void LCD_disp_char(uchar x,uchar y,uchar dat);//��ĳ����Ļλ������ʾһ���ַ�,X��0-15),y(1-2)  
void LCD_disp_str(uchar x,uchar y,uchar *str); //LCD1602��ʾ�ַ������� 
void delay_n10us(uint n);                     //��ʱ����
/*-------------------------------------
			   LCD1602�ӳ���
---------------------------------------*/
void LCD_init(void) 
{  
	delay_n10us(10);  
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7 
	delay_n10us(10);  LCD_write_command(0x0c);//����ʾ���ع�꣬����˸ 
	delay_n10us(10);  LCD_write_command(0x06);//�趨���뷽ʽ����������λ 
	delay_n10us(10);  LCD_write_command(0x01);//�����Ļ��ʾ  
	delay_n10us(100);       //��ʱ��������ʱ��������ʱԼn��10us 
}

void LCD_write_command(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=0;         //ָ�� 
	LCD_RW=0;         //д�� 
	LCD_E=1;          //���� 
	LCD_DB=dat;  delay_n10us(10);  //ʵ��֤�����ҵ�LCD1602�ϣ���forѭ��1�ξ��������ͨдָ� 
	LCD_E=0;  delay_n10us(10);  //ʵ��֤�����ҵ�LCD1602�ϣ���forѭ��1�ξ��������ͨдָ� 
}

void LCD_write_data(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=1;          //���� 
	LCD_RW=0;		   //д��
	LCD_E=1;           //���� 
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

void delay_n10us(uint n)  //��ʱ 
{                 
	uint i;                    
	for(i=n;i>0;i--)            
	 {         
	 nop_();_nop_();_nop_();_nop_();_nop_();_nop_();    
	 } 
}






/*********************************************************/
// ADC0832��ʱ������
/*********************************************************/
void WavePlus()
{
	_nop_();
	ADC_CLK = 1;
	_nop_();
	ADC_CLK = 0;
}



/*********************************************************/
// ��ȡָ��ͨ����A/Dת�����
/*********************************************************/
uchar Get_ADC0832(uchar chanel)
{ 
	uchar i;
	uchar dat1=0;
	uchar dat2=0;
	
	ADC_CLK = 0;				// ��ƽ��ʼ��
	ADC_DAT = 1;
	_nop_();
	ADC_CS = 0;
	WavePlus();					// ��ʼ�ź� 
	ADC_DAT = 1;
	WavePlus();					// ͨ��ѡ��ĵ�һλ
	ADC_DAT = chanel;      
	WavePlus();					// ͨ��ѡ��ĵڶ�λ
	ADC_DAT = 1;
	
	for(i=0;i<8;i++)		// ��һ�ζ�ȡ
	{
		dat1<<=1;
		WavePlus();
		if(ADC_DAT)
			dat1=dat1|0x01;
		else
			dat1=dat1|0x00;
	}
	
	for(i=0;i<8;i++)		// �ڶ��ζ�ȡ
	{
		dat2>>= 1;
		if(ADC_DAT)
			dat2=dat2|0x80;
		else
			dat2=dat2|0x00;
		WavePlus();
	}
	
	_nop_();						// �����˴δ���
	ADC_DAT = 1;
	ADC_CLK = 1;
	ADC_CS  = 1;   

	if(dat1==dat2)			// ���زɼ����
		return dat1;
	else
		return 0;
} 








/*********************************************************/
// ADC0832��ʱ������
/*********************************************************/
void WavePlus2()
{
	_nop_();
	ADC_CLK2 = 1;
	_nop_();
	ADC_CLK2 = 0;
}



/*********************************************************/
// ��ȡָ��ͨ����A/Dת�����
/*********************************************************/
uchar Get_ADC08322(uchar chanel)
{ 
	uchar i;
	uchar dat1=0;
	uchar dat2=0;
	
	ADC_CLK2 = 0;				// ��ƽ��ʼ��
	ADC_DAT2 = 1;
	_nop_();
	ADC_CS2 = 0;
	WavePlus2();					// ��ʼ�ź� 
	ADC_DAT2 = 1;
	WavePlus2();					// ͨ��ѡ��ĵ�һλ
	ADC_DAT2 = chanel;      
	WavePlus2();					// ͨ��ѡ��ĵڶ�λ
	ADC_DAT2 = 1;
	
	for(i=0;i<8;i++)		// ��һ�ζ�ȡ
	{
		dat1<<=1;
		WavePlus2();
		if(ADC_DAT2)
			dat1=dat1|0x01;
		else
			dat1=dat1|0x00;
	}
	
	for(i=0;i<8;i++)		// �ڶ��ζ�ȡ
	{
		dat2>>= 1;
		if(ADC_DAT2)
			dat2=dat2|0x80;
		else
			dat2=dat2|0x00;
		WavePlus2();
	}
	
	_nop_();						// �����˴δ���
	ADC_DAT2 = 1;
	ADC_CLK2 = 1;
	ADC_CS2  = 1;   

	if(dat1==dat2)			// ���زɼ����
		return dat1;
	else
		return 0;
} 




void LCD() 
{
	LCD_disp_char(1,1,numR/100+'0'); //��ʾ 
	LCD_disp_char(2,1,numR/10%10+'0'); //��ʾ 
	LCD_disp_char(3,1,numR%10+'0'); //��ʾ
	
	LCD_disp_char(6,1,numG/100+'0'); //��ʾ 
	LCD_disp_char(7,1,numG/10%10+'0'); //��ʾ 
	LCD_disp_char(8,1,numG%10+'0'); //��ʾ
	
	LCD_disp_char(11,1,numB/100+'0'); //��ʾ 
	LCD_disp_char(12,1,numB/10%10+'0'); //��ʾ 
	LCD_disp_char(13,1,numB%10+'0'); //��ʾ
	

	LCD_disp_char(8,2,num/10+'0'); //��ʾ 
	LCD_disp_char(9,2,num%10+'0'); //��ʾ 
	
	
	LCD_disp_char(14,2,set/10+'0'); //��ʾ 
	LCD_disp_char(15,2,set%10+'0'); //��ʾ 
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
������
********************************************************/
void main(void)
{
	LCD_init();//LCD��ʼ��              
	LCD_disp_str(0,1,"R000 G000 B000 "); //��ʾ 
	LCD_disp_str(0,2,"      n:00 s:00"); //��ʾ 
	
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
				LCD_disp_str(0,2,"White"); //��ʾ 
				if(num<99)
				{
					num++;
				}
			}
		}
		else
		{
			up=1;
			LCD_disp_str(0,2,"     "); //��ʾ 
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




