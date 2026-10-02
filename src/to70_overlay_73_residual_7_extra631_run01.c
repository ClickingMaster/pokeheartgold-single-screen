#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x318];int state;u8 gap[0x4714];u16 id;volatile s16 timer;}Work;extern int sub_02037454(void),ov73_021E746C(void),sub_0203769C(void);extern void ov73_021E66F0(Work*),sub_02037030(int,void*,int);
int ov73_021E69E8(Work*w,int result);
int ov73_021E69E8(Work*w,int result){u8 a[4];if(w->id!=sub_02037454()||w->id!=ov73_021E746C()){w->timer=0;w->state=8;ov73_021E66F0(w);return result;}w->timer++;if(w->timer>30){MI_CpuFill8(a,0,4);a[2]=1;a[0]=sub_0203769C();sub_02037030(112,a,4);w->timer=0;w->state=9;}ov73_021E66F0(w);return result;}
