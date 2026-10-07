#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Page18{u8 payload[254];u16 crc;} Page18;
typedef struct BackupWork18{Page18 pages[4];u8 verifyBuffer[256];MATHCRC16Table table;} BackupWork18;
typedef struct BackupState18{volatile u16 error;u16 pad;volatile s32 ready;u32 address;} BackupState18;
extern BackupState18 _021D43C8;
extern u16 MATH_CalcCRC16(const MATHCRC16Table*,const void*,u32);
extern void writeNvram(u32,u32,void*);
extern BOOL verify(void*,u32,u32,void*),writeDisable(void);
BOOL DWCi_BACKUPlWriteAll(void*);
typedef char WorkSize18[(sizeof(BackupWork18)==0x700)?1:-1];
typedef char PageSize18[(sizeof(Page18)==256)?1:-1];
BOOL DWCi_BACKUPlWriteAll(void *argument){
 BackupWork18 *work=argument;
 struct{u32 address;Page18 *page;} cursor;int i;
 cursor.address=_021D43C8.address;cursor.page=work->pages;
 for(i=0;i<4;i++,cursor.page++,cursor.address+=256){
  work->pages[i].crc=MATH_CalcCRC16(&work->table,cursor.page->payload,254);
  do{writeNvram(cursor.address,256,cursor.page);}while(!verify(cursor.page,cursor.address,256,work->verifyBuffer));
 }
 return writeDisable()!=0;
}
