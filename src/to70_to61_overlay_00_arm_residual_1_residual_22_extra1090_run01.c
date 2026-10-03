#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{int active;void *pointer;int a,b,c,d,e;u8 flags[4];int f,g,h;int cb[12];} State;
typedef struct{int a,b;State *state;int c;} Global;
extern Global ov00_0221A918;
void ov00_021EFF48(State *,void *,int,int,int);
void ov00_021F018C(int,int,int,int,int,int,int,int);
extern int ov00_021EC230(void),ov00_021F05EC(void),ov00_021F1248(void),ov00_0220AED0(void);
extern void ov00_0220AEF0(void),ov00_0220AE60(void),ov00_021F06A8(int,int),ov00_021F0914(void);
void ov00_021F0028(void);
void ov00_021EFF48(State *s,void *p,int a,int b,int c){ov00_0221A918.state=s;ov00_0221A918.state->active=0;ov00_0221A918.state->pointer=p;ov00_0221A918.state->a=0;{State *current=ov00_0221A918.state;current->b=0;current->c=0;current->d=c;}ov00_0221A918.state->e=b;ov00_0221A918.state->flags[0]=0;ov00_0221A918.state->flags[1]=0;ov00_0221A918.state->flags[2]=0;ov00_0221A918.state->flags[3]=0;ov00_0221A918.state->f=0;ov00_0221A918.state->g=0;ov00_0221A918.state->h=a;ov00_0221A918.state->cb[0]=0;ov00_0221A918.state->cb[1]=0;ov00_0221A918.state->cb[2]=0;ov00_0221A918.state->cb[3]=0;ov00_0221A918.state->cb[4]=0;ov00_0221A918.state->cb[5]=0;ov00_0221A918.state->cb[6]=0;ov00_0221A918.state->cb[7]=0;ov00_0221A918.state->cb[8]=0;ov00_0221A918.state->cb[9]=0;ov00_0221A918.state->cb[10]=0;ov00_0221A918.state->cb[11]=0;}
void ov00_021F0028(void){State *s;if(!ov00_0221A918.state)return;if(ov00_021EC230())return;if(!ov00_0221A918.state->e){if(ov00_0221A918.state->pointer&&*(int*)ov00_0221A918.state->pointer)ov00_021F05EC();return;}if(ov00_021F1248()||ov00_0220AED0()){ov00_0221A918.a=1;ov00_0221A918.c=0;ov00_0220AEF0();ov00_0221A918.a=0;if(ov00_0221A918.c==1){ov00_0221A918.c=0;ov00_0220AE60();}}if(ov00_0221A918.state->pointer&&*(int*)ov00_0221A918.state->pointer){if(ov00_021F05EC())return;s=ov00_0221A918.state;if(!s->active)return;if(s->e&&s->flags[2]!=3&&((u32)s->a)>7){if(s->flags[2]<=1)ov00_021F06A8(s->e,s->d);if(ov00_0221A918.state->flags[0]>=ov00_0221A918.state->d){ov00_0221A918.state->flags[2]=3;ov00_0221A918.state->flags[3]++;}}}if(ov00_0221A918.state->flags[3]>=2){ov00_0221A918.state->flags[3]=0;ov00_021F0914();}}
void ov00_021F018C(int unused0,int unused1,int a,int b,int c,int d,int e,int f){ov00_0221A918.state->cb[0]=a;ov00_0221A918.state->cb[1]=b;ov00_0221A918.state->cb[2]=c;ov00_0221A918.state->cb[3]=d;ov00_0221A918.state->cb[4]=e;ov00_0221A918.state->cb[5]=f;ov00_0221A918.state->flags[1]=0;ov00_0221A918.state->flags[2]=0;ov00_0221A918.state->flags[3]=0;ov00_0221A918.state->flags[0]=0;ov00_0221A918.state->active=1;if(!ov00_0221A918.state->e)ov00_0221A918.state->flags[3]++;ov00_0221A918.state->flags[3]++;}
