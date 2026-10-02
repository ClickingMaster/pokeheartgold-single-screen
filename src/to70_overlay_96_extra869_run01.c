#pragma force_active on
#include "global.h"
extern void*NARC_New(int,int);
extern void NARC_ReadWholeMember(void*,int,void*),NARC_Delete(void*);
extern int ov96_021E679C(int,int);
typedef struct{u8 p[0x284];int heap;}Work;
typedef struct{u8 p[0x3f0];u16 a,b;}Entry;
void ov96_021E604C(Work*);
void ov96_021E604C(Work*w){void*narc=NARC_New(169,w->heap);int row,col;u8*out;row=0;out=(u8*)w+0x618;for(;row<4;w=(Work*)((u8*)w+0x7c),out+=0x3c,row++){Entry*entry=(Entry*)w;u8*dst=out;for(col=0;col<3;col++,entry=(Entry*)((u8*)entry+0x28),dst+=20)NARC_ReadWholeMember(narc,ov96_021E679C(entry->a,entry->b),dst);}NARC_Delete(narc);}
