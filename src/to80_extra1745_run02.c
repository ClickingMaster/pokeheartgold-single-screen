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
typedef struct{int state,pad,handle,id,pad10,retries;Buffer input;int pad24;Buffer output;}Task;
typedef struct{u8 p[0x177];char password[31];u8 q[0x1a0-0x196];int identifier;}Work;
typedef struct{Work*work;}Ctx;
extern char ov00_022190B4[],ov00_022190A8[],ov00_02218FC0[],ov00_0221904C[],ov00_02219044[],ov00_02219024[],ov00_0221902C[],ov00_02219034[],ov00_0221903C[],ov00_022190D4[],ov00_02219050[],ov00_0221907C[];
extern int ov00_02202454(Ctx*,int,Buffer*,int*,int*,char*),ov00_0220A4A0(char*,char*,char*,int);extern void __msl_assertion_failed(char*,char*,char*,int),ov00_021FF31C(char*,int,char*),ov00_022020EC(Ctx*,Buffer*,char*);
int ov00_02206BA8(Ctx*,Task*);
int ov00_02206BA8(Ctx*c,Task*t){Work*w=c->work;struct{int closed,got;char digest[33],token[33],name[31],number[16],formatted[256];}local;char*end,*line;int result,id;if(t->state!=104)__msl_assertion_failed(ov00_022190B4,ov00_022190A8,ov00_02218FC0,193);result=ov00_02202454(c,t->handle,&t->input,&local.got,&local.closed,ov00_0221904C);if(result)return result;if(local.closed){t->state=106;return 0;}end=strstr(t->input.data,ov00_02219044);if(end){*end=0;line=t->input.data;if(!strncmp(line,ov00_02219024,6)){if(!ov00_0220A4A0(line,ov00_0221902C,local.number,16)){t->state=106;return 0;}id=atol(local.number);if(!ov00_0220A4A0(t->input.data,ov00_02219034,local.name,31)){t->state=106;return 0;}if(!ov00_0220A4A0(t->input.data,ov00_0221903C,local.token,33)){t->state=106;return 0;}sprintf(local.formatted,ov00_022190D4,w->password,w->identifier,id);ov00_021FF31C(local.formatted,strlen(local.formatted),local.digest);if(strcmp(local.token,local.digest)){ov00_022020EC(c,&t->output,ov00_02219050);ov00_022020EC(c,&t->output,ov00_02219044);t->state=106;return 0;}ov00_022020EC(c,&t->output,ov00_0221907C);ov00_022020EC(c,&t->output,ov00_02219044);t->state=105;t->id=id;}else{t->state=106;return 0;}t->input.length=0;}return 0;}
