#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 fn;void*arg;} Pair;typedef struct{u8 pad[0x1bc];Pair cb;} D;typedef struct{D*d;} C;typedef struct{int state,pad,handle,id,deadline;u32 unused;u32 input[4],output[2];int ready;} T;typedef struct{int id,tick;char*text;} M;
extern char ov00_0221904C[],ov00_022190DC[],ov00_022190EC[];extern int ov00_0220267C(C*,int,void*,int*,int,char*),ov00_02206E04(C*,T*),ov00_02202454(C*,int,void*,int*,int*,char*),ov00_022027FC(C*,void*,char**,int*,int*),ov00_02202A84(C*,Pair,void*,void*,int),ov00_021FFA70(int);extern void *ov00_021EC3A8(int);extern char *ov00_021FF3D4(char*);extern void ov00_0220A890(C*,char*),ov00_02201BC4(C*,int,int,char*),ov00_0220A1C0(C*,T*,int,int,char*,int),ov00_02202938(C*,void*);
int ov00_02206ED8(C*,T*);
// @fn ov00_02206ED8
int ov00_02206ED8(C*c,T*t){int got;int closed;Pair cb;char*line;int code;int extra;D*d=c->d;if(t->ready){int result=ov00_0220267C(c,t->handle,t->output,&closed,1,ov00_0221904C);if(closed||result){t->state=106;return 0;}}if(!t->ready){int result=ov00_02206E04(c,t);if(result)return result;if(t->state==106)return 0;}if(ov00_02202454(c,t->handle,t->input,&got,&closed,ov00_0221904C)){t->state=106;return 0;}if(got>0)t->deadline=ov00_021FFA70(0)+300;do{int result=ov00_022027FC(c,t->input,&line,&code,&extra);if(result)return result;if(line){switch(code){case 1:cb=d->cb;if(cb.fn){M*p=ov00_021EC3A8(12);if(!p){ov00_0220A890(c,ov00_022190DC);return 1;}p->id=t->id;p->text=ov00_021FF3D4(line);p->tick=ov00_021FFA70(0);result=ov00_02202A84(c,cb,p,0,2);if(result)return result;}break;case 102:ov00_02201BC4(c,t->id,103,ov00_022190EC);break;case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:ov00_0220A1C0(c,t,code,t->input[0],line,extra);break;}ov00_02202938(c,t->input);}}while(line);if(closed)t->state=106;return 0;}
