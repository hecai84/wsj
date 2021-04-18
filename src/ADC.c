/*
 * @Author: your name
 * @Date: 2021-04-19 00:35:27
 * @LastEditTime: 2021-04-19 00:43:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\ADC.c
 */
#include "OB38R08A1.h"
#include "ADC.h"

unsigned int n_data    = 0x0000;
bit          ADCfinish = 0;


void ADC_Start(unsigned char n_ADC_CH)
{  
    ADCC2  = n_ADC_CH; //Set a new channel
    ADCC2 |= 0x80;      //ADC start conversion
}

void ADC_Init(void)
{
    ADCC1 = d_ADCnEN0;                 //Set ADC channel 
    ADCCS = d_ADCnEN1 | d_ADC_CLK_Sel; //Set ADC channel & Select ADC clock
    IEADC = 1;                         //Enable ADC interrupt.
    EA    = 1;
}

unsigned int ADC_Read12() //12-bit ADC
{
    unsigned int n_ADCD12 = 0x0000;
    n_ADCD12 = (ADCDH<<4) | ADCDL;     //12-bit ADC
    return n_ADCD12;
}
    

void ADC(void)
{ 
    // if (d_ADCnEN0 & 0x01)
    // {
    //     ADC_Start(d_ADC_CH0_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN0 & 0x02)
    // {
    //     ADC_Start(d_ADC_CH1_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN0 & 0x04)
    // {
    //     ADC_Start(d_ADC_CH2_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    if (d_ADCnEN0 & 0x08)
    {
        ADC_Start(d_ADC_CH3_IN);
        while(!ADCfinish);
        ADCfinish = 0;
    }
    // if (d_ADCnEN0 & 0x10)
    // {
    //     ADC_Start(d_ADC_CH4_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN0 & 0x20)
    // {
    //     ADC_Start(d_ADC_CH5_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN0 & 0x40)
    // {
    //     ADC_Start(d_ADC_CH6_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN0 & 0x80)
    // {
    //     ADC_Start(d_ADC_CH7_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
    // if (d_ADCnEN1 & 0x80)
    // {
    //     ADC_Start(d_ADC_BG_IN);
    //     while(!ADCfinish);
    //     ADCfinish = 0;
    // }
}


