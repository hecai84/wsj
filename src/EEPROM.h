/*--------------------------------------------------------------------------
Header file for EEPROM.
Modify on 2015/10/26.
--------------------------------------------------------------------------*/

#ifndef __EEPROM_H__
#define __EEPROM_H__

// Example Code
/*
#define test_len         64
#define test_start_addr  0x3000
void main(void)
{
    unsigned int i;
    unsigned char buf[test_len];

    EEPROM_Page_Erase(test_start_addr);
    EEPROM_Sector_Program(test_start_addr, test_start_addr+test_len-1, 0x55);
    
    for (i = 0; i < test_len; i++)
    {
        buf[i] = EEPROM_Byte_Read(test_start_addr+i);
    }
    
    for (i = 0; i < test_len; i++)
    {
        EEPROM_Byte_Modify(test_start_addr+i, i);
    }
            
    for (i = 0; i < test_len; i++)
    {
        buf[i] = EEPROM_Byte_Read(test_start_addr+i);
    }
    
    while(1)
    {
        Check_ISPFAH();
        test();
        
    }

}
*/

void EEPROM_Enable(void);
void EEPROM_Disable(void);
void EEPROM_Byte_Program(unsigned int Addr, unsigned char Data);
void EEPROM_Page_Erase(unsigned int Addr);        //128-Byte
void EEPROM_Sector_Program(unsigned int Addr_start, unsigned int Addr_end, unsigned char Data);
unsigned char EEPROM_Byte_Read(unsigned int Addr);
//unsigned int EEPROM_Word_Read(unsigned int Addr);
void EEPROM_Byte_Modify(unsigned int Addr, unsigned char Data);
void Write_EEPROM(unsigned char forcePow,unsigned char volt);
unsigned char Read_EEPROM_FORCEPOW();
unsigned char Read_EEPROM_VOLT();
#endif
