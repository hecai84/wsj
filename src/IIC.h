/*
 * @Author: your name
 * @Date: 2021-04-02 00:46:37
 * @LastEditTime: 2021-04-29 23:24:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \WSJ\src\IIC.h
 */
/*--------------------------------------------------------------------------
Header file for IIC Master.
Modify on 02/24/2014.
--------------------------------------------------------------------------*/

#ifndef __IIC_MASTER_H__
#define __IIC_MASTER_H__
#include "Tools.h"


unsigned char write_add(unsigned char,unsigned char *,unsigned char);  
unsigned char read_add(unsigned char,unsigned char *,unsigned char); 

#endif
