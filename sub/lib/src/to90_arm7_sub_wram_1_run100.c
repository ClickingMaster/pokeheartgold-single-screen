#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Ch35176 Ch35176;
struct Ch35176 {u8 id,type,state,flags,vol,key;u16 tune;u8 key2,vel,a,b;u16 c,d;s32 env,e,f;u8 attack,sustain;u16 decay,release;u8 priority,pan;u16 volume,timer;u8 lfo[10];s16 pitch;u32 next;struct {u8 format,loop;u16 rate,timer,start;u32 length;} wave;u32 data;void (*cb)(Ch35176 *,int,void *);void *arg;u32 last;};
extern Ch35176 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int,int);
extern const s16 SNDi_DecibelTable[];
int SND_UpdateExChannelEnvelope(Ch35176 *p,int update);
// @fn SND_UpdateExChannelEnvelope
int SND_UpdateExChannelEnvelope(Ch35176 *p,int update){if(update)switch(p->state){case 0:p->env=(s32)(0u-((u32)((s32)((0u-(u32)p->env)*p->attack)>>8)));if(p->env==0)p->state=1;break;case 1:{int limit=SNDi_DecibelTable[p->sustain]*128;p->env=(s32)((u32)p->env-p->decay);if(p->env<=limit){p->env=limit;p->state=2;}break;}case 2:break;case 3:p->env=(s32)((u32)p->env-p->release);break;}return p->env>>7;}
