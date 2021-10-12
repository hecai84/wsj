/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:45
 * @LastEditTime: 2021-10-12 09:50:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wsj\src\oled.c
 */
#include "oled.h"
#define SDA P0_5
#define SCL P0_4
#define LCD_EN P3_0
#define LCD_RES P1_6

#define uint unsigned int
#define uchar unsigned char

uchar Contrast_level = 0x8F;
//bit log = 0;

#define Write_Address 0x78 /*slave addresses with write*/
#define Read_Address 0x79  /*slave addresses with read*/

#define Start_column 0x00
#define Start_page 0x00

u8 isDisplay;

void Set_Page_Address(unsigned char add);
void Set_Column_Address(unsigned char add);
void Initial(void);
void Display_Picture();

void SentByte(unsigned char Byte);
void Check_Ack(void); //Acknowledge
static void Stop(void);
static void Start(void);
void Send_ACK(void);

static void somenop()
{
    ;
    ;
}

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
u8 code picBat[5][36] = {
    0x00, 0x00, 0xF0, 0x30, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x30, 0xF0, 0x00, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x3F,
    0x00, 0x00, 0xF0, 0x30, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x30, 0xF0, 0x00, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3F, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
    0x3E, 0x3E, 0x3E, 0x3F,
    0x00, 0x00, 0xF0, 0x30, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x30, 0xF0, 0x00, 0x00, 0xFF, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F,
    0x00, 0x00, 0xF0, 0x30, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x30, 0xF0, 0x00, 0x00, 0xFF, 0xF8,
    0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFF, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F,
    0x00, 0x00, 0xF0, 0xF0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF0, 0xF0, 0x00, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F};

u8 code picDot_s[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, /*".",0*/
};

u8 code picShan_s[] = {0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0x9C, 0x8E, 0x80, 0x01, 0x71, 0x39, 0x1F, 0x0F, 0x07, 0x03, 0x00};

u8 code picA_s[] = {

    0x00, 0x80, 0xF8, 0x7E, 0x7E, 0xF8, 0x80, 0x00, 0x38, 0x3F, 0x07, 0x04, 0x04, 0x07, 0x3F, 0x38 /*"A",0*/};

u8 code picNum_s[10][16] = {
    0xF8, 0xFC, 0x06, 0x02, 0x06, 0xFC, 0xF8, 0x00, 0x0F, 0x1F, 0x30, 0x20, 0x30, 0x1F, 0x0F, 0x00, /*"0",0*/

    0x00, 0x00, 0x04, 0x04, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, /*"1",1*/

    0x00, 0x18, 0x1C, 0x06, 0x02, 0xC6, 0xFC, 0x38, 0x00, 0x30, 0x38, 0x2E, 0x27, 0x21, 0x20, 0x20, /*"2",2*/

    0x00, 0x18, 0x1C, 0x86, 0x82, 0xC6, 0x7C, 0x38, 0x00, 0x0C, 0x1C, 0x30, 0x20, 0x31, 0x1F, 0x0E, /*"3",3*/

    0x00, 0x00, 0x80, 0xE0, 0x78, 0xFE, 0xFE, 0x00, 0x00, 0x06, 0x07, 0x05, 0x04, 0x3F, 0x3F, 0x04, /*"4",4*/

    0x00, 0xFE, 0xFE, 0x62, 0x22, 0x62, 0xC2, 0x82, 0x00, 0x08, 0x18, 0x30, 0x20, 0x30, 0x1F, 0x0F, /*"5",5*/

    0x00, 0xF8, 0xFC, 0xC6, 0x42, 0xC6, 0x9C, 0x18, 0x00, 0x0F, 0x1F, 0x30, 0x20, 0x30, 0x1F, 0x0F, /*"6",6*/

    0x00, 0x02, 0x02, 0x02, 0x82, 0xF2, 0x7E, 0x0E, 0x00, 0x00, 0x00, 0x3C, 0x3F, 0x03, 0x00, 0x00, /*"7",7*/

    0x00, 0x38, 0x7C, 0xC6, 0x82, 0xC6, 0x7C, 0x38, 0x00, 0x0E, 0x1F, 0x31, 0x20, 0x31, 0x1F, 0x0E, /*"8",8*/

    0x00, 0x78, 0xFC, 0x86, 0x02, 0x86, 0xFC, 0xF8, 0x00, 0x0C, 0x1C, 0x31, 0x21, 0x31, 0x1F, 0x0F /*"9",9*/

};

u8 code picDot_b[] = {

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, /*".",0*/

};

// u8 code picV_b[]={0x20,0x60,0xE0,0x20,0x00,0x00,0x00,0x00,0xA0,0x60,0x20,0x00,0x00,0x00,0x07,0x7E,
// 0xE0,0x00,0x80,0x78,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1E,0x03,0x00,
// 0x00,0x00,0x00,0x00};

u8 code picV_b[] = {
    0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x07, 0x3F, 0xF8,
    0xC0, 0xC0, 0xF8, 0x3F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x0F, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, /*"V",0*/
};                          /*"V",0*/

u8 code picNum_b[10][36] = {

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"0",0*/

    0x00, 0x00, 0x00, 0x40, 0x40, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
    0x00, 0x00, 0x00, 0x00, /*"1",1*/

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00,
    0x80, 0xE0, 0x70, 0x1C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x08, 0x0C, 0x0F, 0x0B, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x00, 0x00, /*"2",2*/

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00,
    0x10, 0x10, 0x10, 0x38, 0xEF, 0xC7, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"3",3*/

    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8,
    0x9E, 0x87, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F,
    0x00, 0x00, 0x00, 0x00, /*"4",4*/

    0x00, 0xE0, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x9F, 0x0C,
    0x04, 0x04, 0x04, 0x0C, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"5",5*/

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0C,
    0x04, 0x04, 0x04, 0x0C, 0xF9, 0xF1, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"6",6*/

    0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xE0, 0x7C, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0F, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, /*"7",7*/

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0xC7, 0xEF, 0x38,
    0x10, 0x10, 0x10, 0x38, 0xEF, 0xC7, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"8",8*/

    0x00, 0x80, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x60,
    0x40, 0x40, 0x40, 0x60, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x08, 0x08, 0x08, 0x0C,
    0x07, 0x03, 0x00, 0x00, /*"9",9*/

};

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
    u8 count = 0;
    u8 ack = 1;
    SDA = 1;
    SCL = 1;
    somenop();
    while (ack == 1 && count < 255)
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

void write_i(unsigned char ins)
{
    Start();
    SentByte(Write_Address);
    SentByte(0x00);
    SentByte(ins);
    Stop();
}
void write_d(unsigned char dat) 
{ 
    Start();
    SentByte(Write_Address);
    SentByte(0x40); 
    SentByte(dat); 
    Stop(); 
}

// Set page address
void Set_Page_Address(unsigned char add)
{
    add = 0xb0 | add;
    write_i(add);
    somenop();
    return;
}

void Set_Column_Address(unsigned char add)
{
    add += 28;
    write_i((0x10 | (add >> 4)));

    write_i((0x0f & add));
    return;
}

void LcdPowerOff()
{
    LCD_EN=1;
    SDA=1;
    SCL=1;

    P1M1 = P1M1| 0x40;
    P1M0 = P1M0| 0x40;
    LCD_RES=1;
}
void LcdPowerOn()
{
    P1M1 = P1M1 & 0xBF;
    P1M0 = P1M0 & 0xBF;    
    LCD_EN=0;
    LCD_RES=0;
    Delay_ms(50);
    LCD_RES=1;   
}

void Initial(void)
{
    //Delay_ms(4000);
    P0M1 = P0M1 | 0x30;
    P0M0 = P0M0 | 0x30;

    write_i(0xae); //--turn off oled panel

    write_i(0xd5); //--set display clock divide ratio/oscillator frequency

    write_i(0xf0); //--set divide ratio

    write_i(0xa8); //--set multiplex ratio(1 to 64)

    write_i(0x27); //--1/40 duty

    write_i(0xd3); //-set display offset

    write_i(0x00); //-not offset
    
    write_i(0x40); //--set start line address

    write_i(0x8d); //--set Charge Pump enable/disable

    write_i(0x14); //--set(0x10) disable

    write_i(0x20);
    write_i(0x02);

    write_i(0xa1); //--set segment re-map

    write_i(0xC8); //--Set COM Output Scan Direction

    write_i(0xda); //--set com pins hardware configuration

    write_i(0x12);

    write_i(0xAD);
    write_i(0x30);

    write_i(0x81); //--set contrast control register

    write_i(Contrast_level);

    write_i(0xd9); //--set pre-charge period

    write_i(0x22);

    write_i(0xdb); //--set vcomh

    write_i(0x20);

    write_i(0xa4);
    write_i(0xa6);
    write_i(0x0c);
    write_i(0x11);
    
    clear();
    write_i(0xaf); //--turn on oled panel
    isDisplay = 1;
}

void DisplayOff()
{
    write_i(0xae);
    write_i(0x8d); //--set Charge Pump enable/disable

    write_i(0x10); //--set(0x10) disable

    LcdPowerOff();
    isDisplay = 0;
    Delay_ms(100);
}
void DisplayOn()
{
    LcdPowerOn();
    // write_i(0x8d); //--set Charge Pump enable/disable

    // write_i(0x14); //--set(0x10) disable
    
    Initial();

    //write_i(0xaf);
    isDisplay = 1;
    Delay_ms(100);
}

void clear(void)
{
    unsigned char i, j;
    for (i = 0; i < 0x05; i++)
    {
        Set_Page_Address(i);

        Set_Column_Address(0x00);

        // Start();
        // SentByte(Write_Address);
        // SentByte(0x40); 
        for (j = 0; j < 72; j++)
        {
            write_d(0);
        }
        //Stop();
    }
    return;
}

static u8 twinkBat = 0;
void DisplayBat(u8 bat)
{
    unsigned char i, j = 0;
    if (twinkBat == 4)
        twinkBat = 0;
    else
        twinkBat++;
    for (i = 0; i < 0x03; i++)
    {
        Set_Page_Address(i);
        Set_Column_Address(55);
        
        // Start();
        // SentByte(Write_Address);
        // SentByte(0x40); 
        if (bat == 0 && twinkBat < 2)
        {
            for (j = 0; j < 12; j++)
            {
                write_d(0);
            }
        }
        else if (bat == 255)
        {
            for (j = 0; j < 12; j++)
            {
                write_d(picBat[twinkBat][i * 12 + j]);
            }
        }
        else
        {
            for (j = 0; j < 12; j++)
            {
                write_d(picBat[bat][i * 12 + j]);
            }
        }
        //Stop();
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
    u8 i, j, n, offset = 0;
    for (i = 0; i < 0x03; i++)
    {
        Set_Page_Address(i);
        Set_Column_Address(0);


        // Start();
        // SentByte(Write_Address);
        // SentByte(0x40); 
        offset = i * 12;
        //显示10位
        n = num / 100;
        if (n != 0)
        {
            for (j = 2; j < 12; j++)
            {
                write_d(picNum_b[n][offset + j]);
            }
        }
        //显示个位
        n = (num / 10) % 10;
        for (j = 0; j < 12; j++)
        {
            write_d(picNum_b[n][offset + j]);
        }
        //显示小数点
        for (j = 0; j < 4; j++)
        {
            write_d(picDot_b[i * 4 + j]);
        }
        //显示小数点后1位
        n = num % 10;
        for (j = 0; j < 12; j++)
        {
            write_d(picNum_b[n][offset + j]);
        }

        //显示V
        for (j = 0; j < 12; j++)
        {
            write_d(picV_b[offset + j]);
        }

        n = num / 100;
        if (n == 0)
        {
            for (j = 0; j < 12; j++)
            {
                write_d(0);
            }
        }
        //Stop();
    }
}

void DisplayShan_s(u8 visable)
{
    u8 i, j = 0;
    for (i = 0; i < 2; i++)
    {
        Set_Page_Address(i + 3);
        Set_Column_Address(58);

        // Start();
        // SentByte(Write_Address);
        // SentByte(0x40); 
        if (visable == 1)
        {
            for (j = 0; j < 8; j++)
            {
                write_d(picShan_s[i * 8 + j]);
            }
        }
        else
        {
            for (j = 0; j < 8; j++)
            {
                write_d(0);
            }
        }
        //Stop();
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
    u8 i, j, n, offset = 0;

    for (i = 0; i < 2; i++)
    {
        Set_Page_Address(i + 3);
        Set_Column_Address(0);
        
        // Start();
        // SentByte(Write_Address);
        // SentByte(0x40); 
        offset = i * 8;
        //显示个位
        n = num / 100;

        for (j = 0; j < 8; j++)
        {
            write_d(picNum_s[n][offset + j]);
        }
        //显示小数点
        for (j = 0; j < 5; j++)
        {
            write_d(picDot_s[i * 5 + j]);
        }
        //显示小数点后1位
        n = (num / 10) % 10;
        for (j = 0; j < 8; j++)
        {
            write_d(picNum_s[n][offset + j]);
        }

        //显示小数点后2位
        n = num % 10;
        for (j = 0; j < 8; j++)
        {
            write_d(picNum_s[n][offset + j]);
        }

        //显示A
        for (j = 0; j < 8; j++)
        {
            write_d(picA_s[offset + j]);
        }
        //Stop();
    }
}
