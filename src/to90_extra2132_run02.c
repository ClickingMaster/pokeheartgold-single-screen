#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { s16 x,y,dx,dy; } Calibration;
typedef struct { u8 pad[28]; s32 x,dx,mx,y,dy,my; u16 valid; } CalibrationState;
extern CalibrationState _021E36B0;
#define DIVMODE (*(volatile u16 *)0x04000280)
#define DIVNUM (*(volatile u32 *)0x04000290)
#define DIVDEN (*(volatile s32 *)0x04000298)
#define DIVHIGH (*(volatile u32 *)0x0400029c)
#define DIVRESULT (*(volatile s32 *)0x040002a0)
#pragma opt_common_subs off
#pragma opt_lifetimes off
#pragma opt_propagation off
#pragma opt_dead_assignments off
void TP_SetCalibrateParam(const TPCalibrateParam *p) {
 OSIntrMode irq; s32 divisor;
 if(!p) { _021E36B0.valid=0; return; }
 irq=OS_DisableInterrupts();
 if((divisor=p->xDotSize)) { DIVMODE=0; DIVNUM=0x10000000; *(volatile u64 *)0x04000298=(u32)divisor; _021E36B0.x=p->x0; _021E36B0.dx=p->xDotSize; while(DIVMODE&0x8000){} _021E36B0.mx=DIVRESULT; }
 else { _021E36B0.x=0; _021E36B0.dx=0; _021E36B0.mx=0; }
 if((divisor=p->yDotSize)) { DIVMODE=0; DIVNUM=0x10000000; *(volatile u64 *)0x04000298=(u32)divisor; _021E36B0.y=p->y0; _021E36B0.dy=p->yDotSize; while(DIVMODE&0x8000){} _021E36B0.my=DIVRESULT; }
 else { _021E36B0.y=0; _021E36B0.dy=0; _021E36B0.my=0; }
 OS_RestoreInterrupts(irq); _021E36B0.valid=1;
}
