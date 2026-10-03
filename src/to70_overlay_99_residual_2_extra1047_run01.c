#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 a,b,c,d;u8 p[36];} Entry;typedef struct {Entry *entries;} Work;extern void GF_AssertFail(void);u16 ov99_021E5BD8(Work *,int,int);
u16 ov99_021E5BD8(Work *w,int index,int mode){if(w==0)GF_AssertFail();if(mode==3){u16 value=w->entries[index].d-w->entries[index].a;value-=w->entries[index].b;value-=w->entries[index].c;return value;}return ((u16 *)&w->entries[index])[mode];}
