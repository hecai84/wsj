/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:45
 * @LastEditTime: 2021-04-30 01:55:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\oled.c
 */
#define SDA P0_5
#define SCL P0_4
#include "oled.h"


#define uint unsigned int
#define uchar unsigned char

uchar Contrast_level = 0x8F;
bit log = 0;

#define Write_Address 0x78 /*slave addresses with write*/
#define Read_Address 0x79  /*slave addresses with read*/

#define Start_column 0x00
#define Start_page 0x00

void Write_number(uchar code *n, uchar k, uchar station_dot);
void adj_Contrast(void);
void Set_Page_Address(unsigned char add);
void Set_Column_Address(unsigned char add);
void Set_Contrast_Control_Register(unsigned char mod);
void Initial(void);
void Display_Chess(unsigned char value);
void Display_Chinese(unsigned char ft[]);
void Display_Chinese_Column(unsigned char ft[]);
void Display_Picture(unsigned char pic[]);

void SentByte(unsigned char Byte);
void Check_Ack(void); //Acknowledge
static void Stop(void);
static void Start(void);
void Send_ACK(void);
unsigned char ReceiveByte(void);

static void somenop()  
{;;}  

xdata u8 num[]={

0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,/*"0",0*/

0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,/*"1",1*/

0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,/*"2",2*/

0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,/*"3",3*/

0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,/*"4",4*/

0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,/*"5",5*/

0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,/*"6",6*/

0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,/*"7",7*/

0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,/*"8",8*/
};


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
    unsigned char ack = 1;
    SDA = 1;
    SCL = 1;
    somenop();
    while (ack == 1)
    {
        ack = SDA;
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

void Set_Contrast_Control_Register(unsigned char mod)
{
    Start();
    SentByte(Write_Address);
    SentByte(0x80);
    SentByte(0x81);
    SentByte(0x80);
    SentByte(mod);
    Stop();
    return;
}

void Initial(void)
{

    Delay_ms(2000);

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

    Stop();
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

void Display_Chess(unsigned char value)
{
    unsigned char i, j, k;
    for (i = 0; i < 0x05; i++)
    {
        Set_Page_Address(i);

        Set_Column_Address(0x00);

        Start();
        SentByte(Write_Address);
        SentByte(0x40);
        for (j = 0; j < 0x09; j++)
        {
            for (k = 0; k < 0x04; k++)
                SentByte(value);
            for (k = 0; k < 0x04; k++)
                SentByte(~value);
        }
        Stop();
    }
    return;
}

void Display_Chinese(unsigned char ft[])
{
    unsigned char i, j, k, b, c = 0;
    unsigned int num = 0;

    for (b = 0; b < 1; b++)
    {
        for (i = 0; i < 1; i++)
        {
            Set_Page_Address(c);
            Set_Column_Address(0x00);
            num = i * 0x10 + b * 256;
            Start();
            SentByte(Write_Address);
            SentByte(0x40);
            for (j = 0; j < 0x04; j++)
            {
                for (k = 0; k < 0x10; k++)
                {
                    SentByte(ft[num + k]);
                }
                num += 0x20;
            }
            c++;
            Stop();
        }
    }
    return;
}

void Display_num()
{
    unsigned char i, j,k,d  = 0;
    clear();
    for (i = 0; i < 1; i++)
    {
        for(d=0;d<2;d++)
        {
            Set_Page_Address(i*2+d);
            Set_Column_Address(0x00);
            Start();
            SentByte(Write_Address);
            SentByte(0x40);
            for (j = 0; j < 8; j++)
            {
                for(k=0;k<8;k++)
                    SentByte(num[i*0x80+j*16+d*8+k]);
            }
        }
        Stop();
    }
    return; 
}


void Display_Picture()
{
    unsigned char i, j, num = 0;
    clear();
    for (i = 0; i < 0x05; i++)
    {
        Set_Page_Address(i);
        Set_Column_Address(0x00);
        Start();
        SentByte(Write_Address);
        SentByte(0x40);
        for (j = 0; j < 72; j++)
        {
            //SentByte(pic[i * 72 + j]);
        }
        Stop();
    }
    return;
}

void move_test(void)
{
    unsigned char i;

    for (i = 0x0; i <= 20; i++)
    {
        Start();
        SentByte(Write_Address);
        SentByte(0x80);
        SentByte(i + 0x40); //--set Inverse Display
        SentByte(0x00);
        Stop();
        Delay_ms(2000);
    }
    Delay_ms(35000);

    for (i = 21; i <= 40; i++)
    {
        Start();
        SentByte(Write_Address);
        SentByte(0x80);
        SentByte(i + 0x40); //--set Inverse Display
        SentByte(0x00);
        Stop();
        Delay_ms(2000);
    }

    Delay_ms(65000);

    Start();
    SentByte(Write_Address);
    SentByte(0x80);
    SentByte(0x40); //--set Inverse Display
    SentByte(0x00);
    Stop();
}

void Display_test()
{
    clear();
    Display_num();
}