#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Ch35189 Ch35189;
struct Ch35189 {u8 id,type,state;union {u8 flags;struct {u8 active:1,started:1,reserved:1,updates:5;};};u8 vol,key;u16 tune;u8 key2,vel,a,b;u16 c,d;s32 env,e,f;u8 attack,sustain;u16 decay,release;u8 priority,pan;u16 volume,timer;u8 lfo[10];s16 pitch;u32 next;struct {u8 format,loop;u16 rate,timer,start;u32 length;} wave;u32 data;void (*cb)(Ch35189 *,int,void *);void *arg;u32 last;};
extern Ch35189 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int,int);
void SND_InvalidateWave(u32 start,u32 end);
// @fn SND_InvalidateWave
void SND_InvalidateWave(u32 start,u32 end){u8 i;for(i=0;i<16;i++){Ch35189 *p=&SNDi_Work[i];if(p->active&&p->type==0&&start<=p->data&&p->data<=end){p->started=0;SND_StopChannel(i,0);}}}
