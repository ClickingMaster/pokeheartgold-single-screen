#include "global.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct Ov92BattleSequenceR14 Ov92BattleSequenceR14;
typedef struct W W;
extern int ov92_0225DFA4(void *, int);
extern void ov92_0225DF58(void *, int, int);
extern int ov92_0225F968(Ov92BattleSequenceR14 *, int, u32 *);
extern SysTask *ov92_0225EDB4(W *, int);
extern void ov92_0225EE04(W *, int);
typedef struct {
 u32 id, participants;u8 reserved08[0x21a4-8];u32 objectTimer;
 u8 reserved21a8[0x2afe - 0x21a8];s16 count,counters[4];u32 outputs[4];
 u8 reserved2b18[0x2b9c - 0x2b18];u32 states[4];u8 reserved2bac[12];u32 notified;
} Parent;
typedef char ParentSize2bbc[(sizeof(Parent)==0x2bbc)?1:-1];
void ov92_0225F9C4(void *);
void ov92_0225F9C4(void *data){
 Parent *w=data;int n=(s32)w->participants,i;
 for(i=0;i<n;i++){
  if(i==(s32)w->id)continue;
  switch(w->states[i]){
  case 0:
   if(ov92_0225DFA4(data,i)!=1)ov92_0225DF58(data,i,1);
   if(ov92_0225F968(data,w->counters[i],&w->outputs[i]))ov92_0225EE04(data,i);
   (w->counters[i])++;break;
  case 1:
   w->count=0;w->counters[i]=0;w->outputs[i]=0;w->objectTimer=0;
   if(ov92_0225DFA4(data,i)!=3&&!w->notified){ov92_0225DF58(data,i,3);ov92_0225EDB4(data,i);w->notified=1;}
   break;
  case 2:
   w->notified=0;w->count=0;w->counters[i]=0;w->outputs[i]=0;w->states[i]=0;break;
  }
 }
}
