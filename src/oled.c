/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:45
 * @LastEditTime: 2021-05-06 00:11:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\oled.c
 */
#include "oled.h"
#define SDA P0_5
#define SCL P0_4


#define uint unsigned int
#define uchar unsigned char

uchar Contrast_level = 0x8F;
bit log = 0;

#define Write_Address 0x78 /*slave addresses with write*/
#define Read_Address 0x79  /*slave addresses with read*/

#define Start_column 0x00
#define Start_page 0x00

u8 curDisplay;

void Write_number(uchar code *n, uchar k, uchar station_dot);
void Set_Page_Address(unsigned char add);
void Set_Column_Address(unsigned char add);
void Initial(void);
void Display_Picture();

void SentByte(unsigned char Byte);
void Check_Ack(void); //Acknowledge
static void Stop(void);
static void Start(void);
void Send_ACK(void);
unsigned char ReceiveByte(void);


static void somenop()  
{;;}  



// u8 code picBat[5][36]={0xF8,0x18,0x18,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x18,0x18,0xF8,0xFF,0x00,0x00,0x00,
// 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
// 0xC0,0xC0,0xC0,0xFF,
// 0xF8,0x18,0x18,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x18,0x18,0xF8,0xFF,0x00,0x00,0x00,
// 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xC0,0xC0,0xDE,0xDE,0xDE,0xDE,0xDE,
// 0xDE,0xC0,0xC0,0xFF,
// 0xF8,0x18,0x18,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x18,0x18,0xF8,0xFF,0x00,0x00,0xC0,
// 0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0xFF,0xFF,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,0xDF,
// 0xDF,0xC0,0xC0,0xFF,
// 0xF8,0x18,0x18,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x18,0x18,0xF8,0xFF,0x00,0x00,0xFC,
// 0xFC,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0xFF,0xFF,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,0xDF,
// 0xDF,0xC0,0xC0,0xFF,
// 0xF8,0x18,0x18,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0x18,0x18,0xF8,0xFF,0x00,0x00,0xFF,
// 0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,0xDF,
// 0xDF,0xC0,0xC0,0xFF};
u8 code picBat[5][36]={
0x00,0x00,0xF0,0x30,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x30,0xF0,0x00,0x00,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x3F,0x30,0x30,0x30,0x30,0x30,
0x30,0x30,0x30,0x3F,
0x00,0x00,0xF0,0x30,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x30,0xF0,0x00,0x00,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x3F,0x3E,0x3E,0x3E,0x3E,0x3E,
0x3E,0x3E,0x3E,0x3F,
0x00,0x00,0xF0,0x30,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x30,0xF0,0x00,0x00,0xFF,0xC0,
0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F,
0x00,0x00,0xF0,0x30,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x30,0xF0,0x00,0x00,0xFF,0xF8,
0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xFF,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F,
0x00,0x00,0xF0,0xF0,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xF0,0xF0,0x00,0x00,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F};

u8 code picDot_s[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00};

u8 code picA_s[]={
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,/*"A",0*/};

u8 code picNum_s[10][16]={0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,/*"0",1*/
0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,/*"1"*/
0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,/*"2"*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,/*"3"*/
0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,/*"4"*/
0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,/*"5"*/
0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,/*"6"*/
0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,/*"7"*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,/*"8"*/
0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00/*"9"*/};

u8 code picDot_b[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x00,0x00,0x00,
0x00,0x00,0x00,0x00};

// u8 code picV_b[]={0x20,0x60,0xE0,0x20,0x00,0x00,0x00,0x00,0xA0,0x60,0x20,0x00,0x00,0x00,0x07,0x7E,
// 0xE0,0x00,0x80,0x78,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1E,0x03,0x00,
// 0x00,0x00,0x00,0x00};

u8 code picV_b[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x79,0x80,
0x00,0x80,0x79,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1C,0x03,0x00,0x00,
0x00,0x00,0x00,0x00};/*"V",0*/


u8 code picNum_b[10][36]={0x00,0x00,0x80,0xC0,0x60,0x20,0x20,0x60,0xC0,0x80,0x00,0x00,0x00,0xFE,0xFF,0x01,
0x00,0x00,0x00,0x00,0x01,0xFF,0xFE,0x00,0x00,0x01,0x07,0x0E,0x18,0x10,0x10,0x18,
0x0E,0x07,0x01,0x00,/*"0"*/
0x00,0x00,0x40,0x40,0x40,0xC0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x1F,0x1F,0x10,
0x10,0x10,0x00,0x00,/*"1"*/
0x00,0x80,0x40,0x20,0x20,0x20,0x20,0x60,0xC0,0x80,0x00,0x00,0x00,0x03,0x03,0x00,
0x00,0xC0,0x60,0x30,0x1F,0x07,0x00,0x00,0x00,0x18,0x1C,0x12,0x11,0x10,0x10,0x10,
0x10,0x1E,0x00,0x00,/*"2"*/
0x00,0xC0,0xC0,0x20,0x20,0x20,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x01,0x01,0x00,
0x10,0x10,0x18,0x2F,0xE7,0x80,0x00,0x00,0x00,0x0E,0x0E,0x10,0x10,0x10,0x10,0x18,
0x0F,0x07,0x00,0x00,/*"3"*/
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xE0,0x00,0x00,0x00,0x00,0xC0,0xA0,0x98,
0x84,0x83,0x80,0xFF,0xFF,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x1F,
0x1F,0x10,0x10,0x00,/*"4"*/
0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x3F,0x10,
0x08,0x08,0x08,0x18,0xF0,0xE0,0x00,0x00,0x00,0x07,0x0B,0x10,0x10,0x10,0x10,0x1C,
0x0F,0x03,0x00,0x00,/*"5"*/
0x00,0x00,0x80,0xC0,0x40,0x20,0x20,0x20,0xE0,0xC0,0x00,0x00,0x00,0xFC,0xFF,0x21,
0x10,0x08,0x08,0x08,0x18,0xF0,0xE0,0x00,0x00,0x01,0x07,0x0C,0x18,0x10,0x10,0x10,
0x08,0x0F,0x03,0x00,/*"6"*/
0x00,0x00,0xC0,0x60,0x20,0x20,0x20,0x20,0x20,0xE0,0x60,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0xE0,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,
0x00,0x00,0x00,0x00,/*"7"*/
0x00,0x80,0xC0,0x60,0x20,0x20,0x20,0x20,0x60,0xC0,0x80,0x00,0x00,0x83,0xCF,0x6C,
0x18,0x10,0x30,0x30,0x68,0xCF,0x83,0x00,0x00,0x07,0x0F,0x08,0x10,0x10,0x10,0x10,
0x18,0x0F,0x07,0x00,/*"8"*/
0x00,0x00,0xC0,0xC0,0x20,0x20,0x20,0x20,0xC0,0x80,0x00,0x00,0x00,0x1F,0x3F,0x60,
0x40,0x40,0x40,0x20,0x10,0xFF,0xFE,0x00,0x00,0x00,0x0C,0x1C,0x10,0x10,0x10,0x08,
0x0F,0x03,0x00,0x00/*"9"*/};






void Write_number(uchar code *n, uchar k, uchar station_dot)
{
    uchar i;
    Start();
    SentByte(Write_Address);
    SentByte(0x40);
    for (i = 0; i < 8; i++)
    {
        SentByte(*(n + 16 * k + i));
    }
    Stop();

    Set_Page_Address(Start_page + 1);
    Set_Column_Address(Start_column + station_dot * 8);
    Start();
    SentByte(Write_Address);
    SentByte(0x40);
    for (i = 8; i < 16; i++)
    {
        SentByte(*(n + 16 * k + i));
    }
    Stop();
}


static void Start(void)
{
    SDA = 1;
    SCL = 1;
    somenop();
    SDA = 0;
    somenop();
    SCL = 0;
}

static void Stop(void)
{
    SCL = 0;
    SDA = 0;
    somenop();
    SCL = 1;
    somenop();
    SDA = 1;
}

void Check_Ack(void) //Acknowledge
{
    u8 count=0;
    u8 ack = 1;
    SDA = 1;
    SCL = 1;
    somenop();
    while (ack == 1 && count<255)
    {
        ack = SDA;
        count++;
    }
    somenop();
    SCL = 0;
    return;
}

void SentByte(unsigned char Byte)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        SCL = 0;
        if ((Byte & 0x80) == 0x80)
            SDA = 1;
        else
            SDA = 0;
        SCL = 1;
        somenop();
        Byte = Byte << 1;
    }
    SCL = 0;
    Check_Ack();
}

unsigned char ReceiveByte(void)
{
    uchar i, rudata = 0;
    SCL = 0;
    SDA = 1;
    for (i = 0; i < 8; i++)
    {
        SCL = 1;
        somenop();
        if (SDA == 1)
            rudata |= 0x01;
        else
            rudata |= 0x00;
        rudata = rudata << 1;
        SCL = 0;
        somenop();
    }
    Send_ACK();
    return rudata;
}

void Send_ACK(void)
{
    SCL = 0;
    SDA = 0;
    somenop();
    SCL = 1;
    SCL = 0;
}

// Set page address
void Set_Page_Address(unsigned char add)
{
    Start();
    SentByte(Write_Address);
    SentByte(0x80);
    add = 0xb0 | add;
    SentByte(add);
    somenop();
    return;
}

void Set_Column_Address(unsigned char add)
{
    add += 28;
    Start();
    SentByte(Write_Address);
    SentByte(0x80);
    SentByte((0x10 | (add >> 4)));
    SentByte(0x80);
    SentByte((0x0f & add));
    Stop();
    return;
}



void Initial(void)
{

    Delay_ms(4000);

    Start();
    SentByte(Write_Address);
    SentByte(0x80);
    SentByte(0xae); //--turn off oled panel
    SentByte(0x80);

    SentByte(0xd5); //--set display clock divide ratio/oscillator frequency
    SentByte(0x80);
    SentByte(0xf0); //--set divide ratio
    SentByte(0x80);

    SentByte(0xa8); //--set multiplex ratio(1 to 64)
    SentByte(0x80);
    SentByte(0x27); //--1/40 duty
    SentByte(0x80);

    SentByte(0xd3); //-set display offset
    SentByte(0x80);
    SentByte(0x00); //-not offset
    SentByte(0x80);

    SentByte(0xad); //--Internal IREF Setting		***
    SentByte(0x80);
    SentByte(0x30); //
    SentByte(0x80);

    SentByte(0x8d); //--set Charge Pump enable/disable
    SentByte(0x80);
    SentByte(0x14); //--set(0x10) disable
    SentByte(0x80);

    SentByte(0x40); //--set start line address
    SentByte(0x80);

    SentByte(0xa6); //--set normal display
    SentByte(0x80);

    SentByte(0xa4); //Disable Entire Display On
    SentByte(0x80);

    SentByte(0xa1); //--set segment re-map
    SentByte(0x80);

    SentByte(0xC8); //--Set COM Output Scan Direction
    SentByte(0x80);

    SentByte(0xda); //--set com pins hardware configuration
    SentByte(0x80);
    SentByte(0x12);
    SentByte(0x80);

    SentByte(0x81); //--set contrast control register
    SentByte(0x80);
    SentByte(Contrast_level);
    SentByte(0x80);

    SentByte(0xd9); //--set pre-charge period
    SentByte(0x80);
    SentByte(0x22);
    SentByte(0x80);

    SentByte(0xdb); //--set vcomh
    SentByte(0x80);
    SentByte(0x20);

    SentByte(0x80);

    SentByte(0xaf); //--turn on oled panel
    curDisplay=1;
    Stop();
}

void DisplayOff()
{
    
    Start();
    SentByte(Write_Address);
    SentByte(0x8d); //--set Charge Pump enable/disable
    SentByte(0x80);
    SentByte(0x10); //--set(0x10) disable
    SentByte(0x80);
    SentByte(0xae);
    Stop();
    curDisplay=0;
    Delay_ms(100);
}
void DisplayOn()
{
    Start();
    SentByte(Write_Address);
    SentByte(0x8d); //--set Charge Pump enable/disable
    SentByte(0x80);
    SentByte(0x14); //--set(0x10) disable
    SentByte(0x80);
    SentByte(0xaf);
    Stop();
    curDisplay=1;
    Delay_ms(100);
}


void clear(void)
{
    unsigned char i, j;
    for (i = 0; i < 0x08; i++)
    {
        Set_Page_Address(i);

        Set_Column_Address(0x00);

        Start();
        SentByte(Write_Address);
        SentByte(0x40);
        for (j = 0; j < 128; j++)
        {

            SentByte(0);
        }
        Stop();
    }
    return;
}


static u8 twinkBat=0;
void DisplayBat(u8 bat)
{
    unsigned char i, j = 0;
    if(twinkBat==4)
        twinkBat=0;
    else
        twinkBat++;
    for (i = 0; i < 0x03; i++)
    {
        Set_Page_Address(i);
        Set_Column_Address(60);
        Start();
        SentByte(Write_Address);
        SentByte(0x40);
        if(bat==0 && twinkBat<2)
        {
            for (j = 0; j < 12; j++)
            {
                SentByte(0);
            }
        }else if(bat==255)
        {
            for (j = 0; j < 12; j++)
            {
                SentByte(picBat[twinkBat][i * 12 + j]);
            }
        }
        else
        {
            for (j = 0; j < 12; j++)
            {
                SentByte(picBat[bat][i * 12 + j]);
            }
        }
        Stop();
    }
}

/**
 * @description: 
 * @param {u8} num  传入电压×10
 * @param {u8} *
 * @return {*}
 */
void DisplayChar_b(u8 num)
{
    u8 i, j,n,offset = 0;
    for (i = 0; i < 0x03; i++)
    {
        Set_Page_Address(i);
        Set_Column_Address(0);
        Start();
        SentByte(Write_Address);
        SentByte(0x40);

        offset=i * 12;
        //显示10位
        n=num/100;
        if(n!=0)
        {
            for (j = 0; j < 12; j++)
            {
                SentByte(picNum_b[n][offset + j]);
            }
        }
        //显示个位
        n=(num/10)%10;
        for (j = 0; j < 12; j++)
        {
            SentByte(picNum_b[n][offset + j]);
        }
        //显示小数点
        for (j = 0; j < 12; j++)
        {
            SentByte(picDot_b[offset + j]);
        }
        //显示小数点后1位
        n=num%10;
        for (j = 0; j < 12; j++)
        {
            SentByte(picNum_b[n][offset + j]);
        }
        
        //显示V
        for (j = 0; j < 12; j++)
        {
            SentByte(picV_b[offset + j]);
        }

        n=num/100;
        if(n==0)
        {
            for (j = 0; j < 12; j++)
            {
                SentByte(0);
            }
        }
        Stop();
    }
}

/**
 * @description: 
 * @param {u8} num  传入电流×100
 * @param {u8} *
 * @return {*}
 */
void DisplayChar_s(u16 num)
{
    u8 i, j,n,offset = 0;
    if(num<10)
        num=0;
    for (i = 0; i < 2; i++)
    {
        Set_Page_Address(i+3);
        Set_Column_Address(0);
        Start();
        SentByte(Write_Address);
        SentByte(0x40);

        offset=i * 8;
        //显示个位
        n=num/100;

        for (j = 0; j < 8; j++)
        {
            SentByte(picNum_s[n][offset + j]);
        }
        //显示小数点
        for (j = 0; j < 8; j++)
        {
            SentByte(picDot_s[offset + j]);
        }
        //显示小数点后1位
        n=(num/10)%10;
        for (j = 0; j < 8; j++)
        {
            SentByte(picNum_s[n][offset + j]);
        }
        
        //显示小数点后2位
        n=num%10;
        for (j = 0; j < 8; j++)
        {
            SentByte(picNum_s[n][offset + j]);
        }
        
        //显示A
        for (j = 0; j < 8; j++)
        {
            SentByte(picA_s[offset + j]);
        }
        Stop();
    }
}
