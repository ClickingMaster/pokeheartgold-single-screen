#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x19d8];int clients[4],state;} Work;extern struct {int a;Work*w;} _0221A684;
extern int VCT_AddConferenceClient(u8);
int ov00_021E7AF4(u32,int);
int ov00_021E7AF4(u32 mask,int skip) {int i;if(_0221A684.w==0 || _0221A684.w->state!=3)return 0;for(i=0;i<4;i++){if(i!=skip && (mask&(1<<i)) && _0221A684.w->clients[i]!=1){if(VCT_AddConferenceClient((u8)i)!=0)return 0;_0221A684.w->clients[i]=1;}}return 1;}
