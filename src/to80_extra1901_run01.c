#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x1e440];void*walker;u8 p2[0x80c];int a,b;u8 courses[30];} Work;
extern int Pokewalker_CourseIsUnlocked(void*,int);
int ov112_021EC950(Work*);
int ov112_021EC950(Work*w){int index,count;w->b=0;w->a=0;MI_CpuFill8(w->courses,255,30);count=0;index=count;do{if(Pokewalker_CourseIsUnlocked(w->walker,index)){w->courses[count]=index;count++;}index++;}while((s32)index<27);return count;}
