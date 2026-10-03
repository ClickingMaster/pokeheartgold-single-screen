#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
extern int strncmp(const char*,const char*,unsigned);
extern char *strstr(const char*,const char*);
extern int sprintf(char*,const char*,...);
extern long atol(const char*);
typedef struct{char*data;int allocated,length;}Buffer;
typedef struct Task Task;
struct Task{int state,pad,handle,id,pad10,retries;Buffer input;int pad24;Buffer output;int ready,pad38;Task*next;};
typedef struct{u8 p[0x110];char name[0x1f];u8 q[0x177-0x12f];char password[31];u8 r[0x1a0-0x196];int identifier;u8 s[0x434-0x1a4];Task*head;}Work;
typedef struct{Work*work;}Ctx;
typedef struct{u8 p[24];char*text;}Peer;
extern char ov00_02219008[],ov00_02219024[],ov00_0221902C[],ov00_02219034[],ov00_0221903C[],ov00_02219044[],ov00_0221904C[],ov00_02219050[],ov00_02219058[],ov00_0221907C[],ov00_02219084[],ov00_022190A4[],ov00_022190A8[],ov00_02218FE4[];
extern int ov00_02207610(Ctx*,Task*),ov00_0220A5A8(Ctx*,int,int*),ov00_02207DD0(Ctx*,int,Peer**),ov00_02207FB8(Peer*),ov00_02202454(Ctx*,int,Buffer*,int*,int*,char*),ov00_022075AC(Ctx*,Task*),ov00_0220267C(Ctx*,int,Buffer*,int*,int,char*);
extern void ov00_0220A890(Ctx*,char*),ov00_022020EC(Ctx*,Buffer*,char*),ov00_0220211C(Ctx*,Buffer*,int),ov00_021EC3D8(void*),ov00_02207E40(Ctx*,Peer*),__msl_assertion_failed(char*,char*,char*,int);
int ov00_0220684C(Ctx*,Task*);
int ov00_0220684C(Ctx*c,Task*t){Work*w=c->work;struct{Peer*peer;int closed,got,mode;}local;int result;switch(t->state){case 101:break;case 102:result=ov00_02207610(c,t);if(result)return result;break;case 103:result=ov00_0220A5A8(c,t->handle,&local.mode);if(result)return result;if(local.mode==4){ov00_0220A890(c,ov00_02219008);return 3;}if(local.mode==3){BOOL freePeer=TRUE;Task*other;if(!ov00_02207DD0(c,t->id,&local.peer)){ov00_0220A890(c,ov00_02219008);return 3;}ov00_022020EC(c,&t->output,ov00_02219024);ov00_022020EC(c,&t->output,ov00_0221902C);ov00_0220211C(c,&t->output,w->identifier);ov00_022020EC(c,&t->output,ov00_02219034);ov00_022020EC(c,&t->output,w->name);ov00_022020EC(c,&t->output,ov00_0221903C);ov00_022020EC(c,&t->output,local.peer->text);ov00_022020EC(c,&t->output,ov00_02219044);for(other=w->head;other;other=other->next)if(other->id==t->id &&other!=t &&other->state<=103)freePeer=FALSE;if(freePeer){ov00_021EC3D8(local.peer->text);local.peer->text=0;if(ov00_02207FB8(local.peer))ov00_02207E40(c,local.peer);}t->state=104;}break;case 104:result=ov00_02202454(c,t->handle,&t->input,&local.got,&local.closed,ov00_0221904C);if(result)return result;{char*end=strstr(t->input.data,ov00_02219044);if(end){char*line;*end=0;line=t->input.data;if(!strncmp(line,ov00_02219050,7)){if(++t->retries>1){ov00_0220A890(c,ov00_02219058);return 3;}result=ov00_022075AC(c,t);if(result)return result;}else if(strncmp(line,ov00_0221907C,6)){ov00_0220A890(c,ov00_02219084);return 3;}t->state=105;t->input.length=0;}}break;default:__msl_assertion_failed(ov00_022190A4,ov00_022190A8,ov00_02218FE4,159);break;}if(t->output.length>0){result=ov00_0220267C(c,t->handle,&t->output,&local.closed,1,ov00_0221904C);if(local.closed||result)t->state=106;}return 0;}
