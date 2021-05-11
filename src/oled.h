/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:35
 * @LastEditTime: 2021-05-09 17:10:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\oled.h
 */
#ifndef __OLED_H__
#define __OLED_H__
#include "Tools.h"

void Initial(void);
void clear(void);
void DisplayChar_b(u8 num);
void DisplayBat(u8 bat);
void DisplayChar_s(u16 num);
void DisplayOn();
void DisplayOff();
void DisplayShan_s(u8 visable);
#endif
