#pragma force_active on
#include "global.h"
typedef struct {u16 cur,max,limit;u8 active,pad;int step;} A;
extern u32 ov102_021E8798(void *);
int ov102_021E8C64(A *);
int ov102_021E8C64(A *a){if(a->cur<a->max){if(a->cur+5<=a->max){a->step=5;a->cur+=5;}else{a->step=a->max-a->cur;a->cur=a->max;}if(ov102_021E8798(a)<a->limit)return 2;a->active=0;return 3;}return 4;}
typedef struct {u32 pad,mode;u8 mail[8];u16 fields[31];u16 selected,word;u8 pad2[0x10];u16 requested;u8 pad3[7];u8 kind;} W;
extern u32 sub_02091C60(void *,u16,u32),sub_02091CAC(void *,u16,u32);
extern void MailMsg_SetFieldI(void *,u16,u32),ov102_021E8F14(W *);
void ov102_021E8CA4(W *);
void ov102_021E8CA4(W *w){u32 value=w->requested;if(value==65535)value=ov102_021E8798((u8*)w+0x54);if(w->kind==0)value=sub_02091C60(*(void**)((u8*)w+0x18),w->word,value);else value=sub_02091CAC(*(void**)((u8*)w+0x18),w->word,value);switch(w->mode){case 0:w->fields[0]=value;break;case 1:w->fields[w->selected]=value;break;case 2:MailMsg_SetFieldI(w->mail,w->selected,value);break;}ov102_021E8F14(w);}
