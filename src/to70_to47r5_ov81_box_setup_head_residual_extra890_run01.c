#pragma force_active on
#include "global.h"
typedef struct{u8 gap[18];u8 count;u8 a:2,flag:1,b:5;u8 gap2[0x34c];void*objects[1];}Work;extern void*ov81_022412C4(Work*,int);extern void ov81_02242EB8(void*,int),ov81_02242F48(void*),ov81_02242FB0(void*,int),ov81_02242F94(void*,int);
void ov81_0223EBE4(Work*w);
void ov81_0223EBE4(Work*w){int i;for(i=0;i<w->count;i++){w->objects[i]=ov81_022412C4(w,i);ov81_02242EB8(w->objects[i],1);if(!w->flag){ov81_02242F48(w->objects[i]);ov81_02242FB0(w->objects[i],0);ov81_02242F94(w->objects[i],0);}}}
