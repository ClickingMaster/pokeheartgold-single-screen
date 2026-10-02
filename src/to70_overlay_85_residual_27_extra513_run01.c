#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct {int active,state,a,b,c;void*p;} Entry;
typedef struct {int active,state,a,b;void*p;} Request;
typedef struct {u8 pad[0xb54];Request entries[5];} Work;
extern int GF_SinDegNoWrap(u16);
extern void ov85_021E8530(int*,int);
void ov85_021E7E88(Work*w,void*p);
void ov85_021E7ED0(Entry*e);
void ov85_021E7ED0(Entry*e){int*p=e->p;int old;switch(e->state){case 0:p[13]=0;e->c=0;e->a=30<<12;e->b=5<<14;e->state++;case 1:{int sine=GF_SinDegNoWrap((u16)(e->c/4096));int amplitude=e->a/4096;int value=amplitude*sine;p[13]=0;ov85_021E8530(&p[13],value);}old=e->c;ov85_021E8530(&e->c,e->b);if(e->c<old){e->a=(u32)e->a>>1;if(e->a==0){p[13]=0;memset(e,0,24);}}break;}}
