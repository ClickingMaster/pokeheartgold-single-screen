// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Card{u8 unused0[28];u32 source,destination,length,channel;u8 unused2c[0xec];u32 instructionThreshold,dataThreshold;}Card;
typedef struct Header{u8 unused[0x60];u32 control;}Header;

extern void CARDi_OnReadCard(void),CARDi_SetCardDma(void);
BOOL CARDi_TryReadCardDma(CARDRomStat*);
static inline BOOL Overlap(u32 a0,u32 a1,u32 b0,u32 b1){return b0<a1&&a0<b1;}
// @fn CARDi_TryReadCardDma
BOOL CARDi_TryReadCardDma(CARDRomStat*registers){Card*c=(Card*)&cardi_common;u32 size=c->length;BOOL ready=0,aligned=0,accessible=0,eligible=0;u32 destination=c->destination,misalignment=destination&31;
 if(!misalignment&&c->channel<=3)eligible=1;
 if(eligible){u32 dtcm=OS_GetDTCMAddress();BOOL bad=1,m=0;if((destination+size)>(u32)OS_IrqHandler&&destination<0x02000000)m=1;if(!m){if(dtcm>=(destination+size)||dtcm+0x4000<=destination)bad=0;}if(!bad)accessible=1;}
 if(accessible){if(!((c->source|size)&511))aligned=1;}if(aligned&&size)ready=1;
 registers->ctrl=(((Header*)cardi_rom_header_addr)->control&~0x07000000u)|0xa1000000;
 if(ready){OSIntrMode interrupts=OS_DisableInterrupts();if(size<c->instructionThreshold)IC_InvalidateRange((void*)destination,size);else IC_InvalidateAll();if(size<c->dataThreshold){if(misalignment){destination-=misalignment;DC_StoreRange((void*)destination,32);DC_StoreRange((void*)(destination+size),32);size+=32;}DC_InvalidateRange((void*)destination,size);DC_WaitWriteBufferEmpty();}else DC_FlushAll();OS_SetIrqFunction(0x80000,CARDi_OnReadCard);OS_ResetRequestIrqMask(0x80000);OS_EnableIrqMask(0x80000);OS_RestoreInterrupts(interrupts);CARDi_SetCardDma();}return ready;}
