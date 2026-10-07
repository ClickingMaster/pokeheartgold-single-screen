#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Ch35171 Ch35171;
struct Ch35171 {u8 id,type,state;union {u8 flags;struct {u8 active:1,started:1,reserved:1,updates:5;};};u8 vol,key;u16 tune;u8 key2,vel,a,b;u16 c,d;s32 env,e,f;u8 attack,sustain;u16 decay,release;u8 priority,pan;u16 volume,timer;u8 lfo[10];s16 pitch;u32 next;struct {u8 format,loop;u16 rate,timer,start;u32 length;} wave;u32 data;void (*cb)(Ch35171 *,int,void *);void *arg;u32 last;};
extern Ch35171 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int,int);
void SND_ExChannelInit(void);
// @fn SND_ExChannelInit
void SND_ExChannelInit(void){int i;for(i=0;i<16;i++){SNDi_Work[i].id=i;SNDi_Work[i].updates=0;SNDi_Work[i].active=0;}sWeakLockChannel[1]=0;sWeakLockChannel[0]=0;}
