#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
typedef struct{u16 a,b;VecFx32 vel;}Motion;typedef struct{Motion motion;int duration;}Rule;typedef struct{u8 pad[4];u16 id;u8 tail[30];}Profile;
extern Rule ov01_02206464[];extern Profile ov01_02206478[];extern u32 ov01_02209B60;extern void GF_AssertFail(void);
void ov01_021EACBC(u8 *,int);
void ov01_021EACBC(u8 *p,int which){int k;if(!which)return;if(W(p,0x28)){if(p[7]==which)H(p,0x2e)=((int)H(p,0x2e)+1)%2;else GF_AssertFail();return;}Profile *profile=&ov01_02206478[W(p,0x30)];p[7]=which;W(p,0x28)=1;if(ov01_02209B60==0){H(p,0x2c)=profile->id;k=which-1;*(Motion *)(p+8)=ov01_02206464[k].motion;ov01_02209B60=p[7];}else{k=which-1;H(p,0x2c)=ov01_02206464[k].motion.a;H(p,8)=profile->id;H(p,10)=0;*(VecFx32 *)(p+12)=ov01_02206464[k].motion.vel;W(p,12)*=-1;W(p,16)*=-1;W(p,20)*=-1;ov01_02209B60=0;}p[4]=ov01_02206464[k].duration;p[5]=0;}
