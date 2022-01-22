/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:45
 * @LastEditTime: 2022-01-19 20:22:24
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
// bit log = 0;

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
void Check_Ack(void); // Acknowledge
static void Stop(void);
static void Start(void);
void Send_ACK(void);

static void somenop()
{
    ;
    ;
}

u8 code picBat[5][16] = {
    // 0x00, 0xFE, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xFE, 0x00, 0x00, 0xFF, 0x00, 0x00,
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x07, 0x06, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    // 0x1E, 0x06, 0x07, 0x00, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\0.bmp",0*/

    // 0x00, 0xFE, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0xFE, 0x00, 0x00, 0xFF, 0x00, 0x00,
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x07, 0x06, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    // 0x1E, 0x06, 0x07, 0x00, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\1.bmp",0*/

    // 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFF, 0x03, 0x03,
    // 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0x00, 0x00, 0x07, 0x06, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    // 0x1E, 0x06, 0x07, 0x00, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\2.bmp",0*/

    // 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFF, 0x1F, 0x1F,
    // 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0xFF, 0x00, 0x00, 0x07, 0x06, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    // 0x1E, 0x06, 0x07, 0x00, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\3.bmp",0*/

    // 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
    // 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x07, 0x07, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    // 0x1F, 0x07, 0x07, 0x00, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\4.bmp",0*/

    0xFE, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xFE, 0x3F, 0x20, 0xE0, 0xE0, 0xE0, 0xE0, 0x20, 0x3F, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\小\0.bmp",0*/

    0xFE, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0xFE, 0x3F, 0x30, 0xF0, 0xF0, 0xF0, 0xF0, 0x30, 0x3F, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\小\1.bmp",0*/

    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x3F, 0x30, 0xF0, 0xF0, 0xF0, 0xF0, 0x30, 0x3F, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\小\2.bmp",0*/
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x3F, 0x23, 0xE3, 0xE3, 0xE3, 0xE3, 0x23, 0x3F, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\小\3.bmp",0*/

    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, /*"C:\Users\hecai\OneDrive\文档\技术资料\芯片资料\0.42寸OLED技术资料\图标\battery\小\4.bmp",0*/

};

u8 code picNum_s[10][16] = {

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"0",0*/

    0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x18, 0x00, /*"1",1*/

    0x00, 0x00, 0x7F, 0x03, 0x06, 0x0C, 0x0C, 0x18, 0x30, 0x30, 0x60, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"2",2*/

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x60, 0x30, 0x1C, 0x30, 0x60, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"3",3*/

    0x00, 0x00, 0x30, 0x30, 0x30, 0x7F, 0x33, 0x36, 0x36, 0x3C, 0x3C, 0x38, 0x38, 0x30, 0x30, 0x00, /*"4",4*/

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x60, 0x60, 0x60, 0x63, 0x37, 0x1F, 0x03, 0x03, 0x03, 0x7F, 0x00, /*"5",5*/

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x63, 0x37, 0x1F, 0x03, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"6",6*/

    0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x18, 0x18, 0x30, 0x30, 0x30, 0x60, 0x60, 0x7F, 0x00, /*"7",7*/

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"8",8*/

    0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x60, 0x7C, 0x76, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, /*"9",9*/
};

u8 code picDot[16] = {
    0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*".",0*/
};

u8 code picV[16] = {
    0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x1E, 0x1E, 0x1E, 0x33, 0x33, 0x33, 0x33, 0x61, 0x61, 0x61, 0x00, /*"V",0*/

};

u8 code picPause[8]={
0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,/*"未命名文件",0*/
};

u8 code picF[8]={
   0x81,0xC1,0xE1,0xF1,0xC1,0xC1,0xFF,0xFF,/*"未命名文件",0*/
};
u8 code picR[8]={
0x81,0x83,0x87,0x8F,0x83,0x83,0xFF,0xFF,/*"未命名文件",0*/
};

u8 code picNum[10][64] = {

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF0, 0x38, 0x1C, 0x1C, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1C, 0x1C, 0x1C, 0x38, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x30, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x30, 0x38, 0x38, 0x38, 0x1C, 0x1E, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, /*"0",0*/

    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0xF8, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, /*"1",1*/

    0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x1C, 0x38, 0x38, 0x70, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x18, 0x38, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07, 0x0E, 0x1E, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x1F, 0x0F, 0x01, 0x00, 0x00, 0x00, /*"2",2*/

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0x1C, 0x1C, 0x0C, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x38, 0xF8, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x0E, 0x07, 0x07, 0x0E, 0x1C, 0x18, 0x38, 0x38, 0x38, 0x1C, 0x1C, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, /*"3",3*/

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0x0E, 0x0C, 0x1C, 0x18, 0x38, 0x30, 0x70, 0x60, 0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x7F, 0x7F, 0x7F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x00, 0x00, 0x00, /*"4",4*/

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0x1C, 0x1C, 0x0C, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x7C, 0xFC, 0x9C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x1C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, /*"5",5*/

    0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0x38, 0x38, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x3C, 0x7C, 0xDC, 0x9C, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x70, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1E, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x38, 0x38, 0x1C, 0x1F, 0x0F, 0x03, 0x00, 0x00, 0x00, /*"6",6*/

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x06, 0x0E, 0x0E, 0x0C, 0x0C, 0x1C, 0x1C, 0x18, 0x18, 0x38, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, /*"7",7*/

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0x3C, 0x1C, 0x0C, 0x0E, 0x0E, 0x0C, 0x1C, 0x18, 0x78, 0xE0, 0xF0, 0x38, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x38, 0xF8, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F, 0x3C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x0E, 0x07, 0x0F, 0x1C, 0x18, 0x38, 0x38, 0x38, 0x38, 0x18, 0x1C, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, /*"8",8*/

    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0x3C, 0x1C, 0x1C, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0x38, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x38, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0E, 0x1C, 0x1C, 0x1C, 0x38, 0x38, 0x3B, 0x3F, 0x3E, 0x3C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x18, 0x1C, 0x0E, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, /*"9",9*/
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

void Check_Ack(void) // Acknowledge
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
    LCD_EN = 1;
    SDA = 1;
    SCL = 1;

    P1M1 = P1M1 | 0x40;
    P1M0 = P1M0 | 0x40;
    LCD_RES = 1;
}
void LcdPowerOn()
{
    P1M1 = P1M1 & 0xBF;
    P1M0 = P1M0 & 0xBF;
    LCD_EN = 0;
    LCD_RES = 0;
    Delay_ms(50);
    LCD_RES = 1;
}

void Initial(void)
{
    // Delay_ms(4000);
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

    // write_i(0xaf);
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
        // Stop();
    }
    return;
}

/**
 * @description: 
 * @param {u8} state 0暂停 1正转  2反转
 * @return {*}
 */
void DisplayPlay(u8 state)
{
    u8 i;
    Set_Page_Address(0);
        Set_Column_Address(52);

    for (i = 0; i < 8; i++)
    {
        if(state==1)
            write_d(picF[i]);
        else if (state==2)
            write_d(picR[i]);
        else
            write_d(picPause[i]);
    }
}

static u8 twinkBat = 0;
void DisplayBat(u8 bat)
{
    u8 i, j;
    if (twinkBat == 4)
        twinkBat = 0;
    else
        twinkBat++;
    for (i = 0; i < 0x02; i++)
    {
        Set_Page_Address(i + 3);
        Set_Column_Address(52);

        if (bat == 0 && twinkBat < 2)
        {
            for (j = 0; j < 8; j++)
            {
                write_d(0);
            }
        }
        else if (bat == 255)
        {
            for (j = 0; j < 8; j++)
            {
                write_d(picBat[twinkBat][i * 8 + j]);
            }
        }
        else
        {
            for (j = 0; j < 8; j++)
            {
                write_d(picBat[bat][i * 8 + j]);
            }
        }
    }
}

#define VOLT_START_POS 0
void DisplayVolt(u8 num)
{
    u8 i, j;
    u8 num1, num2, num3;

    num1 = num / 100;
    num2 = (num / 10) % 10;
    num3 = num % 10;

    j = 0;
    Set_Page_Address(j);
    Set_Column_Address(VOLT_START_POS);
    if (num1 != 0)
    {
        j++;
        for (i = 0; i < 16; i++)
        {
            write_d(picNum_s[num1][i]);
        }
    }

    Set_Page_Address(j++);
    Set_Column_Address(VOLT_START_POS);
    for (i = 0; i < 16; i++)
    {
        write_d(picNum_s[num2][i]);
    }

    Set_Page_Address(j++);
    Set_Column_Address(VOLT_START_POS);
    for (i = 0; i < 16; i++)
    {
        write_d((picDot[i] & 0x0f) + (picNum_s[num3][i] << 4));
    }

    Set_Page_Address(j++);
    Set_Column_Address(VOLT_START_POS);
    for (i = 0; i < 16; i++)
    {
        write_d((picNum_s[num3][i] >> 4) + (picV[i] << 4));
    }
    Set_Page_Address(j++);
    Set_Column_Address(VOLT_START_POS);
    for (i = 0; i < 16; i++)
    {
        write_d((picV[i] >> 4));
    }
    if (num1 == 0)
    {
        Set_Page_Address(j++);
        Set_Column_Address(VOLT_START_POS);
        for (i = 0; i < 16; i++)
        {
            write_d(0);
        }
    }
}

#define NUM_START_POS 17
/**
 * @description:
 * @param {u8} num  传入挡位数字
 * @param {u8} *
 * @return {*}
 */
void DisplayNum(u8 num)
{
    u8 i;
    u8 num1, num2;

    if (!isDisplay)
        return;
    num2 = (u8)(num % 10);
    num1 = (u8)(num / 10);

    Set_Page_Address(0);
    Set_Column_Address(NUM_START_POS);
    for (i = 0; i < 32; i++)
    {
        write_d(picNum[num1][i] << 4);
    }
    Set_Page_Address(1);
    Set_Column_Address(NUM_START_POS);
    for (i = 0; i < 32; i++)
    {
        write_d((picNum[num1][i] >> 4) + (picNum[num1][i + 32] << 4));
    }
    Set_Page_Address(2);
    Set_Column_Address(NUM_START_POS);
    for (i = 0; i < 32; i++)
    {
        //第一个数字的第三行的低4位 加上 第二个数字的第一行低4位
        write_d((picNum[num1][i + 32] >> 4) + (picNum[num2][i] << 4));
    }

    Set_Page_Address(3);
    Set_Column_Address(NUM_START_POS);
    for (i = 0; i < 32; i++)
    {
        //第二个数字的第1行的高4位 加上 第二个数字的第二行低4位
        write_d((picNum[num2][i] >> 4) + (picNum[num2][i + 32] << 4));
    }
    Set_Page_Address(4);
    Set_Column_Address(NUM_START_POS);
    for (i = 0; i < 32; i++)
    {
        write_d((picNum[num2][i + 32] >> 4));
    }

    DisplayVolt(num * 5 + 45);
}
