#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma opt_rotateloops off
#pragma force_active on
typedef struct {u32 first[2];const void *bytes;int index;} Spi;
extern Spi _021E4088;
extern u8 _021E4158[];
extern u16 _02110FBC;
extern u8 _021E408C[];
extern void CARDi_SpiWaitWriteByte(void *),CARDi_SpiDummyWait(void);
void CARD_SpiWaitWriteRange(const void *,int);
void CARD_SpiWaitWriteRange(const void *data,int size){int i;
 _021E4088.bytes=_021E4158;_021E4088.index=0;_021E4158[0]=2;
 CARD_LockBackup(_02110FBC);OS_LockCard(_02110FBC);
 while(*(volatile u16*)0x040001A0 & 0x80){};
 *(volatile u16*)0x040001A0=0xa042;
 CARDi_SpiWaitWriteByte(_021E408C);CARDi_SpiDummyWait();
 _021E4088.bytes=data;
 for(i=0;i<size;i++){if(i==size-1)*(volatile u16*)0x040001A0=0xa002;CARDi_SpiWaitWriteByte(_021E408C);}
 while(*(volatile u16*)0x040001A0 & 0x80){};
 OS_UnLockCard(_02110FBC);CARD_UnlockBackup(_02110FBC);
}
