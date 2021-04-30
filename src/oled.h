/*
 * @Author: your name
 * @Date: 2021-04-29 21:47:35
 * @LastEditTime: 2021-04-30 22:47:42
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
#endif
