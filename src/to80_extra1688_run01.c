#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u16 type;u8 pad[5];u8 count;}Entry;
typedef struct{u8 window[16];void*string;u8 p[0x7e4-0x14];Entry entries[5];void*more;u8 q[6];u8 end1,end2;u8 r[0x868-0x818];u8 step,state;u8 s[2];u8 index:4;u8 count:4;u8 counter;u16 printer;}State;
extern struct{u8 pad[0x48];u32 keys;} gSystem;
extern void ov02_02250504(u8*,State*),MapObject_UnpauseMovement(void*),MapObject_PauseMovement(void*),ClearFrameAndWindow2(void*,int),RemoveWindow(void*),String_Delete(void*);
extern int ov02_0224FB44(State*,Entry*),ov02_0224FB54(u8*,State*,Entry*),FollowMon_TryPrintInteractionMessage(u8*,State*,Entry*),ov02_0224FC74(State*,Entry*),ov02_02250004(u8*,State*,u16),DialogBox_IsPrintFinished(u8);
static inline int Finish(u8*w,State*s){ov02_02250504(w,s);if(s->more)return 2;if(s->end1)return 3;if(s->end2)return 4;return 1;}
static inline void Close(u8*w,State*s){ClearFrameAndWindow2(s,0);RemoveWindow(s);String_Delete(s->string);w[0xd2]&=~0x40;s->state=3;}
int ov02_0224F8FC(u8*,State*);
int ov02_0224F8FC(u8*w,State*s){Entry*e;u16 type;u32 index=s->index;if(index>=5)return Finish(w,s);e=&s->entries[index];type=e->type;if(type==0xffff)return Finish(w,s);switch(s->state){case 0:if(ov02_0224FB44(s,e)){MapObject_UnpauseMovement(*(void**)(w+0xe4));s->step=0;s->state=5;break;}
case 1:if(ov02_0224FB54(w,s,e)){MapObject_UnpauseMovement(*(void**)(w+0xe4));s->state=2;break;}
case 2:if(FollowMon_TryPrintInteractionMessage(w,s,e)){MapObject_PauseMovement(*(void**)(w+0xe4));s->state=6;break;}
case 3:if(ov02_0224FC74(s,e)){s->state=7;break;}
case 4:s->index=(u8)(s->index+1);s->state=0;break;
case 5:if(ov02_02250004(w,s,type))s->state=1;break;
case 6:if(DialogBox_IsPrintFinished((u8)s->printer)==1){if(s->count>s->index+1){if(gSystem.keys&3)Close(w,s);}else{if(s->more)s->state=3;else if(gSystem.keys&3)Close(w,s);}}break;
case 7:if(s->counter>=e->count){s->index=(u8)(s->index+1);s->state=0;}else s->counter++;break;}return 0;}
