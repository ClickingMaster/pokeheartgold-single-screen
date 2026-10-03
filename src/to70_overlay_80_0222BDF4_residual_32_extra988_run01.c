#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x48];void *field;} Work;extern void ov80_0222E2B8(Work *),ov80_0222E400(void *,int,int,u16,u16),ov80_0222E344(Work *,int,int,u8,int);void ov80_0222E3B8(Work *,int,int,int,u16,u16,u8);
void ov80_0222E3B8(Work *w,int value,int a,int b,u16 c,u16 d,u8 e){ov80_0222E2B8(w);ov80_0222E400(w->field,a,b,c,d);if(e!=255)ov80_0222E344(w,1,value,e,0);else ov80_0222E344(w,1,0,e,0);}
