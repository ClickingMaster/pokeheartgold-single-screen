#pragma force_active on
#include "global.h"
typedef struct{u8 a[0x31c];void*sprite;void*other;u8 b[0x3ec-0x324];int current;volatile int next;} Work;extern void ov57_022394D8(Work*,int),ov57_0223B504(Work*,int,int),ov57_02239728(void*,int,int,int),PlaySE(int),ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_SetAnim(void*,int);

int ov57_0223B32C(Work*w,int yes);

int ov57_0223B32C(Work*w,int yes){int next=w->next;int current=w->current;if(current==next)yes=0;if(yes){ov57_022394D8(w,current);ov57_0223B504(w,1,1);ov57_02239728((u8*)w+0xec,0,11,1);PlaySE(1500);return 2;}ManagedSprite_SetDrawFlag(w->other,0);ManagedSprite_SetAnim(w->sprite,0);ov57_0223B504(w,0,0);PlaySE(1501);return 3;}
