/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:35
 * @LastEditTime: 2022-01-06 19:54:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wsj\src\oled.h
 */
#ifndef __OLED_H__
#define __OLED_H__
#include "Tools.h"

void Initial(void);
void clear(void);
void DisplayRotation(u8 forward);
void DisplayBat(u8 bat);
void DisplayNum(u8 num);
void DisplayOn();
void DisplayOff();
void LcdPowerOff();
void LcdPowerOn();
#endif
