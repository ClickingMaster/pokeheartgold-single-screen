#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void SetAllPCBoxesModified(void);void Save_PrepareForAsyncWrite(void*,int);u16 LCRandom(void);
int ov70_02240854(void*p);
int ov70_02240854(void*p){SetAllPCBoxesModified();Save_PrepareForAsyncWrite(PTR(PTR(p,0),32),2);W(p,44)=31;W(p,4544)=(u16)((int)LCRandom()%60)+2;return 3;}
