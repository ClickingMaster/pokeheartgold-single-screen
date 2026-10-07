#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct Ch35173 Ch35173;
struct Ch35173 {u8 id,type,state,flags,vol,key;u16 tune;u8 key2,vel,a,b;u16 c,d;s32 env,e,f;u8 attack,sustain;u16 decay,release;u8 priority,pan;u16 volume,timer;u8 lfo[10];s16 pitch;u32 next;struct {u8 format,loop;u16 rate,timer,start;u32 length;} wave;u32 data;void (*cb)(Ch35173 *,int,void *);void *arg;u32 last;};
extern Ch35173 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int,int);
typedef struct {u32 a,b,c;} Wave35173;
extern void StartExChannel(Ch35173 *,int);
int SND_StartExChannelPcm(Ch35173 *p,Wave35173 *w,u32 data,int v);
// @fn SND_StartExChannelPcm
int SND_StartExChannelPcm(Ch35173 *p,Wave35173 *w,u32 data,int v){p->type=0;*(Wave35173 *)&p->wave=*w;p->data=data;StartExChannel(p,v);return 1;}
