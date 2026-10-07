#pragma force_active on
#include "global.h"
#include "grid_cursor_internal.h"
extern int sub_020947C0(void*,int),sub_02094894(void*,int);
typedef struct{u8 pad[15];u8 mode;} Head;
typedef void(*Callback)(int,int,void*);
typedef struct{u8 pad[15];u8 mode;u8 q[0x874];struct{int a,b,c,d;} entries[1];u8 r[0x3dec];int active,value;u8 t[0x30];void*manager;u8 z[0x1c];Callback callback;void*context;int savedB,savedA;} Work;
void sub_020956B8(Work*);
#define FIELD(w,off,type) (*(type*)((u8*)(w)+(off)))
void sub_020956B8(Work*w){int a,b,index;if(w->callback){a=b=-1;switch(sub_02095DD8(w->manager)){case 0:case 1:case 5:break;case 2:case 6:a=((Head*)w)->mode;index=sub_02095DE8(w->manager);if(sub_020947C0(w,index)==1)b=index;break;case 3:index=sub_02095DE8(w->manager);if(sub_02094894(w,index)==1){Work*entry=(Work*)((u8*)w+index*16);a=entry->entries[0].a;b=entry->entries[0].b;}break;case 4:if(w->active){a=((Head*)w)->mode;b=w->value;}break;}if(w->savedA!=a||w->savedB!=b)w->callback(a,b,w->context);w->savedA=a;w->savedB=b;}}
