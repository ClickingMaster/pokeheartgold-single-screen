#pragma force_active on
#include "global.h"
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern void *ov96_021E8A20(void *);
extern void SysTask_Destroy(void *);
typedef struct {u8 a,b,c,d;void *task4,*task8,*taskc,*task10,*task14;u16 x18,x1a;u8 p[8];u16 x24,x26;u32 x28;u16 x2c,x2e;void *course;} Work;
void ov96_021F2D98(Work *);
void ov96_021F2D98(Work *w){w->d=0;if(w->task4){((u8*)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(w->course)))[8]=1;SysTask_Destroy(w->task4);w->task4=0;}if(w->task8){SysTask_Destroy(w->task8);w->task8=0;}if(w->taskc){SysTask_Destroy(w->taskc);w->taskc=0;}if(w->task10){SysTask_Destroy(w->task10);w->task10=0;}if(w->task14){SysTask_Destroy(w->task14);w->task14=0;}*(vu32*)0x04000018=0x800000;w->a=0;w->b=0;w->c=0;w->x18=0;w->x1a=0;w->x26=0;w->x2e=0;w->x24=0;w->x2c=0;w->x28=12;G2x_SetBlendAlpha_(0x04000050,0,1,12,4);}
