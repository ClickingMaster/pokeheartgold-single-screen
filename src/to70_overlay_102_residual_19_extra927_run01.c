#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1e0];void*a,*b;u8 q[0x1f4-0x1e8];u8 flag;} Work;
extern void ov102_021EAE40(void*,int),ov102_021EB2EC(void*,int,int),ov102_021EB518(void*),ov102_021EAF50(void*),ov102_021E94A4(void*);
void ov102_021EA164(void*,Work**);
void ov102_021EA164(void*unused,Work**pp){Work*w=*pp;ov102_021EAE40(w->a,0);if(w->flag==1){ov102_021EB2EC(w->b,1,1);ov102_021EB518(w->b);}else{ov102_021EB2EC(w->b,1,0);ov102_021EAF50(w->a);}ov102_021E94A4(pp);}
