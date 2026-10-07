#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x44];void *p;u8 pad2[0x18];void *result;} W;
typedef struct {W *w;u8 pad[0x18];u8 *pc;u8 pad2[0x58];u16 value;u8 pad3[6];void *extra;} C;
extern u16 FrontierScriptContext_ReadHalfWord(C *);
extern void *ov80_0222BE24(C *,u16);
extern void *ov80_0222E558(void *,u8,u8,u8,u8,u16 *,void *,void *);
int FrtCmd_021(C *);
static inline u8 readByte(C*c){u8*p=c->pc;c->pc=p+1;return *p;}
#pragma force_active on
int FrtCmd_021(C *c){W *w=c->w;int a=readByte(c);int b=readByte(c);int d=readByte(c);int e=readByte(c);u16 f=FrontierScriptContext_ReadHalfWord(c);void *p=ov80_0222BE24(c,f);w->result=ov80_0222E558(w,a,b,d,e,p,w->p,0);c->value=f;return 1;}
