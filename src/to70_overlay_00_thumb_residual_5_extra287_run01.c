#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[0xfb4];void(*callback)(int,void*);void*arg;u8 gap[0x1070-0xfbc];int state;u8 gap2[8];int count;u8 gap3[0x10a8-0x1080];int ready;}W;
extern W*_0221A680;extern int ov00_021EE490(void),ov00_021EE4FC(void),ov00_021F9988(u8,int);extern void ov00_021E62D8(void),ov00_021E727C(int),ov00_021E5E34(void);
void ov00_021E62F4(void),ov00_021E6388(int),ov00_021E63CC(int,int);
void ov00_021E62F4(void){int i;for(i=0;i<4;i++)ov00_021F9988((u8)i,0);if(ov00_021EE490()==0){for(i=0;i<_0221A680->count;i++){if(i!=ov00_021EE490()&&(ov00_021EE4FC()&(1<<i))){if(!ov00_021F9988((u8)i,10000))GF_AssertFail();}}}else{if(!ov00_021F9988(0,10000))GF_AssertFail();}_0221A680->ready=1;ov00_021E62D8();}
void ov00_021E6388(int unused){int i;_0221A680->state=7;for(i=0;i<_0221A680->count;i++){if(i!=ov00_021EE490())ov00_021E727C(i);}ov00_021E62F4();}
void ov00_021E63CC(int a,int b){W*w;if(a==0){if(b==0)ov00_021E6388(1-ov00_021EE490());else ov00_021E5E34();}else _0221A680->state=12;w=_0221A680;if(w->callback){int self=ov00_021EE490();_0221A680->callback(self,w->arg);}}
