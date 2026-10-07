#pragma force_active on
#include "global.h"
#include "unk_02035900.h"
#include "overlay92_battle_sequence_r14_task_setup_to42_private.h"
#pragma force_active on
typedef struct {
 u32 id;u8 unused04[0x21a4-4];u32 objectTimer;u8 unused21a8[0x2af4-0x21a8];
 u32 previousPhase,phase;s16 timer,count,counters[4];u8 unused2b08[0x10];s32 value;
 u8 unused2b1c[0x80];u32 states[4];u8 remaining2bac[0x10];
} World;
typedef char WorldSize2BBC[(sizeof(World)==0x2bbc)?1:-1];
extern int ov92_0225ED68(void *, int);
extern void ov92_0225ED80(void *),ov92_0225FC9C(void *),ov92_0225FF1C(void *),ov92_0225EBE0(char *),ov92_0225FEB4(void *);
void ov92_0225FAB8(void *);
void ov92_0225FAB8(void *data){
 World *w=data;u32 state=w->states[w->id];switch(state){
 case 0:
  if(ov92_0225ED68(data, 0)){
   w->timer=0;ov92_0225EEBC(data);ov92_0225FEB4(data);w->states[w->id]=1;
   sub_02037030(24,&w->states[w->id],4);w->value=0;w->count=0;w->objectTimer=0;
  }else{
   ov92_0225ED80(data);ov92_0225FC9C(data);ov92_0225FF1C(data);ov92_0225F878(data);w->counters[w->id]++;
  }break;
 case 1:
  w->timer++;if(w->timer>=90)w->states[w->id]=2;w->value=0;w->count=0;w->counters[w->id]=0;break;
 case 2:
  if(w->timer){w->value=0;w->count=0;w->timer=0;w->counters[w->id]=0;if(w->phase!=w->previousPhase)w->previousPhase=w->phase;ov92_0225EBE0(data);sub_02037030(24,&w->states[w->id],4);}else w->states[w->id]=0;break;
 case 3:
  w->timer++;if((u32)w->timer>=29){w->timer=0;ov92_0225EBE0(data);w->states[w->id]=0;}break;
 }
}
