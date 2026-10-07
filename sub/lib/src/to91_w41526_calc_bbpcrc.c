#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void FLASH_Read(u32,u32,void*);
u16 calc_NextCRC(int,int);
u32 CalcBbpCRC(void);
u32 CalcBbpCRC(void){u32 address,ordinal;int crc;u32 value;value=0;crc=0;ordinal=0;address=100;do{FLASH_Read(address,1,&value);if(ordinal==1)value&=128;crc=calc_NextCRC((int)(value&255),crc);ordinal++;address++;}while(ordinal<105);return (u32)crc;}
