/*
 * @Author: your name
 * @Date: 2021-04-02 00:46:37
 * @LastEditTime: 2021-04-18 22:07:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\IIC.c
 */
#include "OB38R08A1.h"
#include "IIC.h"
  
#define scl P1_0
#define sda P1_1 
#define ADDR_W 0xE8
#define ADDR_R 0xE9

void init();  
void somenop();  
void start();  
void stop();  
void send_ack(unsigned char);  
u8 rec_ack();  
void write_byte(unsigned char);  
unsigned char read_byte();   
  

//端口初始化  
void init()  
{  
    sda = 1;  
    scl = 1;  
    somenop();  
}  
//起始信号  
void start()  
{  
    sda = 1;  
    somenop();  
    scl = 1;  
    somenop();  
    sda = 0;  
    somenop();  
}  
//终止信号  
void stop()  
{  
    sda = 0;  
    somenop();  
    scl = 1;  
    somenop();  
    sda = 1;  
    somenop();  
}  
//主机发送应答信号  
void send_ack(unsigned char ack)  
{  
    scl = 0;  
    somenop();  
    sda = ack;  
    somenop();  
    scl = 1;  
    somenop();  
    scl = 0;  
    somenop();  
    sda = 1;  
    somenop();  
}  
//主机接收应答信号  
u8 rec_ack()  
{  
    u8 re=0;
    sda=1;
    scl=1;
    somenop();  

    while(sda==1 && re<255)
    {
        somenop();  
        re++;
    }
    
    re=sda;
    scl=0;
    Delay_10us(1);
    return re;
}  
//写一个字节  
void write_byte(unsigned char dat)  
{  
    unsigned char i,a;  
    a = dat;  
    for(i=0;i<8;i++)  
    {  
        a = a<<1;  
        scl = 0;  
        somenop();  
        sda = CY;  
        somenop();  
        scl = 1;  
        somenop();  
    }  
    scl = 0;  
    somenop();  
    sda = 1;  
    somenop();  
}  
//读一个字节  
unsigned char read_byte()  
{  
    unsigned char i,a;  
    scl = 0;  
    somenop();  
    sda = 1;  
    somenop();  
    for(i=0;i<8;i++)  
    {  
        scl = 1;  
        somenop();  
        a = (a<<1)|sda;  
        scl = 0;  
        somenop();  
    }  
    return a;  
}  
//向add地址写入len长度的dat数据  
unsigned char write_add(unsigned char add,unsigned char *dat,unsigned char len)  
{  
    unsigned char flag,i;  
    start();  
    write_byte(ADDR_W);  
    if(rec_ack() == 0)  
    {  
        write_byte(add);  
        if(rec_ack() == 0)  
        {  
            for(i=0;i<len;i++)  
            {  
                write_byte(dat[i]);  
                if(rec_ack() == 1)  
                {  
                    i = 0;  
                    break;  
                }  
            }  
            if(i == len)  
                flag = 1; 
            else
                Debug(0xe3);  
        }else
            Debug(0xe2);  
    }else
        Debug(0xe1);
    
    stop();  
    return flag;          
}  
//从add地址读出len长度的数据给dat  
unsigned char read_add(unsigned char add,unsigned char *dat,unsigned char len)  
{  
    unsigned char flag,i;     
    start();  
    write_byte(ADDR_W);  
    if(rec_ack() == 0)  
    {  
        write_byte(add);  
        if(rec_ack() == 0)  
        {  
            start();  
            write_byte(ADDR_R);  
            if(rec_ack() == 0)  
            {  
                for(i=0;i<len;i++)  
                {  
                    dat[i] = read_byte();  
                    //Debug(dat[i]);
                    //send_ack(0);  
                    if(i==len-1)
                        send_ack(1);
                    else
                        send_ack(0);
                }  
                flag = 1;   
                somenop();  
            }else
                Debug(0xe3);  
        }else
            Debug(0xe2);  
    }else
        Debug(0xe1);
    
    stop();  
    return flag;  
}  
//delay 5us  
void somenop()  
{;;}  