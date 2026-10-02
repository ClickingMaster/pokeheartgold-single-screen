#pragma force_active on
#include "global.h"
#pragma force_active on
struct State{s8 direction;u8 b;s8 previous;u8 flag:1;u8 rest:7;void*obj;};
struct State*sub_0205F40C(void*);int ov01_021FA2D4(void*);int MapObject_GetFacingDirection(void*);int sub_0205F330(void*);void ov01_021FA3E8(void*,void*);void ov01_021F8C88(void*,void*);
extern void(*ov01_02208AE4[])(void*,void*,struct State*,int);
void ov01_021F82F0(void*);
void ov01_021F82F0(void*m){struct State*s=sub_0205F40C(m);int direction,index;void*obj=s->obj;if(ov01_021FA2D4(m)!=1&&obj!=0){direction=MapObject_GetFacingDirection(m);index=sub_0205F330(m);if(index>=12)GF_AssertFail();if(s->previous==0&&index!=s->previous&&s->flag==0)s->direction=-1;ov01_02208AE4[index](m,obj,s,direction);s->direction=direction;s->previous=index;ov01_021FA3E8(m,obj);ov01_021F8C88(m,obj);}}
