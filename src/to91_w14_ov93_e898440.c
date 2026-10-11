#include "global.h"
#include "system.h"
#include "sprite_system.h"
#include "sound_playback.h"
#pragma force_active on
#define W(p,o) (*(s32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
typedef struct State40066 {u8 opaque00[0x20];s32 state;u8 opaque24[0x1e8];s32 keys[4];u8 opaque21c[0x18];ManagedSprite *sprite;u8 opaque238[0xc];s32 request;u8 opaque248[0x14];s32 duration;u8 opaque260[8];s16 timer;u8 opaque26a[10];u8 active,queued;u8 opaque276[2];} State40066;
typedef struct Ov93WorkScratch Ov93WorkScratch;
typedef struct Ov93Work Ov93Work;
typedef struct Ov93Queue Ov93Queue;
typedef struct Ov93Copy16 {u32 a,b,c,d;} Ov93Copy16;
typedef struct Ov93Request {u8 id,side;u16 value;u32 payload4,payload8;} Ov93Request;
typedef union Message16 {Ov93Copy16 queue;Ov93Request request;u8 raw[16];} Message16;
static inline void copyKeys(void *w,const u8 *sys){((State40066*)w)->keys[0]=H(sys,0x20);((State40066*)w)->keys[1]=H(sys,0x22);((State40066*)w)->keys[2]=H(sys,0x26);((State40066*)w)->keys[3]=H(sys,0x24);}
extern int ov93_0225F370(Ov93WorkScratch *),ov93_0225F44C(Ov93WorkScratch *);
extern void ov93_0225EB38(void *),ov93_0225EB70(u8 *),ov93_0225FB6C(Ov93Work *,ManagedSprite *),ov93_0225EDFC(void *),ov93_0225EDE8(void *);
extern u32 ov93_0225EDB8(void *,s32);
extern s32 ov93_0225EA6C(void *,s32);
extern void ov93_02260F14(void *,u32,u32,void *);
extern u32 ov93_02260F3C(Ov93Queue *,const Ov93Copy16 *);
extern BOOL ov93_0225FEC4(void *,const Ov93Request *);
/* Requires the preserved native-observed unused-input repair proposal. */
extern void ov93_0225EA98(void *,s32);
extern void ov93_0225EAE0(void *,void *,u32,u32);
void ov93_0225E898(void *,void *);
void ov93_0225E898(void *ctx,void *w){Message16 temp;((State40066*)w)->request=0;
 if(W(ctx,0x20)==2){((State40066*)w)->keys[0]=W(ctx,0x1758);((State40066*)w)->keys[1]=W(ctx,0x175c);((State40066*)w)->keys[2]=W(ctx,0x1760);((State40066*)w)->keys[3]=W(ctx,0x1764);}else if(W(ctx,0x2fb8)==0 || W(ctx,0x2fbc)==1){((State40066*)w)->keys[0]=0;((State40066*)w)->keys[1]=0;((State40066*)w)->keys[2]=0;((State40066*)w)->keys[3]=0;}else{copyKeys(w,(const u8 *)&gSystem+0x40);}
 ov93_0225F370(w);if(((State40066*)w)->keys[2]==0){ov93_0225EB38(w);if(((State40066*)w)->state==0){if(((State40066*)w)->state==0)((State40066*)w)->active=0;}}
 ov93_0225F44C(w);ov93_0225EB70(w);ov93_0225FB6C(w,((State40066*)w)->sprite);
 if(((State40066*)w)->timer>0){((State40066*)w)->timer+=8;if(((State40066*)w)->timer>30)((State40066*)w)->timer=30;ov93_0225EDFC(w);}
 if(W(ctx,0x2fbc)==0 && W(ctx,0x2fb8)==1 && ((State40066*)w)->request>0){int bonus=0,amount;if(((State40066*)w)->duration<800){if(ov93_0225EDB8(w,((State40066*)w)->request)==1){((State40066*)w)->timer=8;((State40066*)w)->queued=0;PlaySE(1422);}}else{ov93_0225EDE8(w);((State40066*)w)->timer=0;bonus=1;}
 amount=ov93_0225EA6C(w,((State40066*)w)->request);if(bonus==1)amount+=25*amount/100;
 ov93_02260F14(ctx,(u32)W(ctx,0x2fc8),(u32)amount,&temp);ov93_02260F3C(ctx,&temp.queue);ov93_0225FEC4(ctx,&temp.request);ov93_0225EA98(w,((State40066*)w)->request);ov93_0225EAE0(ctx,w,(u32)((State40066*)w)->request,(u32)bonus);((State40066*)w)->request=0;}
}
