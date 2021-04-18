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
