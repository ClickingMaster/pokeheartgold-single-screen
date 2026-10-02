#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 data[0xc00];void *renderer,*task;u32 a,b;int active,c;} State;
extern void *sub_02014A08(int,void *,void *),*SysTask_CreateOnVBlankQueue(void (*)(void),void *,int);extern void ov74_0222DE68(void);
void ov74_0222DE8C(u8 *,int,int,int);
void ov74_0222DE8C(u8 *owner,int c,int a,int b){State *s=(State*)(owner+12592);s->c=c;s->a=a;s->b=b;if(s->renderer==0)s->renderer=sub_02014A08(85,s,s->data+0x600);if(s->task==0)s->task=SysTask_CreateOnVBlankQueue(ov74_0222DE68,s,1024);s->active=1;}
