#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{char data[256];} Buffer;typedef struct{u8 pad[0x10];int remaining;u8 pad2[0x2c];int flag;u8 pad3[8];int state;u8 pad4[0x5d0];int value;} S;
extern char *ov00_0221A0B8[],ov00_0221A528[];extern unsigned int strlen(const char*);extern int sprintf(char*,const char*,...),ov00_022145F0(void*,char*,int,int,int),ov00_02213938(S*);extern void ov00_02212B88(S*,int),ov00_021FF3C0(int);
void ov00_02213708(S*,int,int,u8*,int,int,int,int);
void ov00_02213708(S*s,int async,int value,u8 *ids,int n,int a,int b,int c){Buffer buf={0};int len=0,i;int result;s->value=value;s->flag=0;for(i=0;i<n;i++){char *name=ov00_0221A0B8[ids[i]];if(len+(int)strlen(name)+1>=256)break;len+=sprintf(buf.data+len,ov00_0221A528,name);ov00_02212B88(s,ids[i]);}result=ov00_022145F0(&s->state,buf.data,a,b,c);if(result)return;if(async)return;while(s->state==3||(s->remaining>0&&!result)){ov00_021FF3C0(10);result=ov00_02213938(s);}}
