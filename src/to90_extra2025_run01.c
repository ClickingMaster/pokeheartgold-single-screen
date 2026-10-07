#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 data[20];} Window;
typedef struct State State;
typedef void(*Callback)(void*,State*);
struct State{u8 pad[0x7c];Window windows[3];u8 active[4];u8 states[4];int values[3];u8 range,delay[3];Callback callback[3];void*user;};
extern int ov49_0225D57C(int*,Window*,void*),ov49_0225D5A0(int*,Window*,void*),ov49_0225D5C8(int*,Window*,void*),ov49_0225D5E4(int*,Window*,void*);
extern void sub_02018198(Window*,int),ov49_0225D328(void*,State*,int);
extern u32 MTRandom(void);
void ov49_0225DD68(void*,State*);
void ov49_0225DD68(void*context,State*s){int i=0;Callback callback1;Window*w=s->windows;int*value=s->values;State*cursor=s;Window*draw=s->windows;int done1,done4;Callback callback4;for(;i<3;i++,w++,draw++,value++,cursor=(State*)((u8*)cursor+4)){if(s->active[i]){switch(s->states[i]){
case 0:ov49_0225D57C(value,w,s->user);sub_02018198(draw,cursor->values[0]);break;
case 1:done1=ov49_0225D5A0(value,w,s->user);sub_02018198(draw,cursor->values[0]);if(done1==1){callback1=cursor->callback[0];ov49_0225D328(context,s,i);if(callback1)callback1(context,s);}break;
case 3:ov49_0225D5C8(value,w,s->user);sub_02018198(draw,cursor->values[0]);break;
case 4:done4=ov49_0225D5E4(value,w,s->user);sub_02018198(draw,cursor->values[0]);if(done4==1){callback4=cursor->callback[0];ov49_0225D328(context,s,i);if(callback4)callback4(context,s);}break;
case 5:if(s->delay[i])s->delay[i]--;else{if(ov49_0225D5A0(value,w,s->user)==1){s->delay[i]=MTRandom()%s->range;cursor->values[0]=0;}sub_02018198(draw,cursor->values[0]);}break;
case 6:if(s->delay[i])s->delay[i]--;else{ov49_0225D57C(value,w,s->user);sub_02018198(draw,cursor->values[0]);}break;
}}}}
