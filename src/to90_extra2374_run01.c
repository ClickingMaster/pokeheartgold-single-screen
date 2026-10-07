#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Obj {u32 key,type;} Obj;
typedef struct Work {void*root,*first,*second,*unused;Obj*selected;u8 pad[16];int x,y,flag;} Work;
typedef struct Task {Work*w;int unused;void(*first)(void),(*second)(void);} Task;
extern int ov41_022481BC(void*),ov41_02248820(void*),ov41_02249710(u32);extern Obj*ov41_022481F4(void*,int*,int*,u32),*ov41_02248858(void*,int*,int*,u32);extern void ov41_022480C8(void*,Obj*),ov41_02248030(Obj*),ov41_02249B44(Obj*,int*,int*),ov41_02248158(void*),ov41_022493BC(Work*,Obj*,int,int,int,int),ov41_0224642C(void),ov41_022486F0(Obj*),ov41_02248724(void*),ov41_02249390(Task*),PlaySE(u16),ov41_02249574(Work*);extern void ov41_022490F0(void),ov41_02249280(void),ov41_022490B0(void),ov41_022490AC(void),ov41_022492B0(void),ov41_022492E0(void);
void ov41_02248F80(Task*);
void ov41_02248F80(Task*t){Work*w=t->w;int x,y;Obj*o;int result;if(ov41_022481BC(w->first)){o=ov41_022481F4(w->first,&x,&y,*(u32*)((u8*)w->root+56));if(o&&o->type==3&&!w->flag)o=0;if(o){if(!o->type){ov41_022480C8(w->first,o);result=0;}else{ov41_02248030(o);result=ov41_02249710(o->key);ov41_02249B44(o,&w->x,&w->y);}ov41_02248158(w->first);ov41_022493BC(w,o,1,x,y,result);ov41_0224642C();}}else if(ov41_02248820(w->second)){o=ov41_02248858(w->second,&x,&y,*(u32*)((u8*)w->root+56));if(o){ov41_022486F0(o);ov41_02248724(w->second);ov41_022493BC(w,o,0,x,y,0);ov41_0224642C();}}if(w->selected){int sound=1515;switch(w->selected->type){case 0:t->first=ov41_022490F0;t->second=ov41_02249280;break;case 1:t->first=ov41_022490B0;t->second=ov41_022490AC;ov41_02249390(t);sound=1661;break;case 3:t->first=ov41_022492B0;t->second=ov41_022492E0;break;}PlaySE((u16)sound);ov41_02249574(w);}}
