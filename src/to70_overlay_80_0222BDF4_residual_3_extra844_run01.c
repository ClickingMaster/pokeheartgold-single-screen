#pragma force_active on
#include "global.h"
typedef struct{void*p;u8 pad[0x18];u8*cursor;u8 pad2[0x60];void*data;u8 pad3[8];u8 result;}Ctx;
extern u8 ov80_0223B9D8[][3];extern int FrontierScriptContext_ReadWord(Ctx*);extern u16 FrontierScriptContext_ReadHalfWord(Ctx*);extern void FrontierScriptContext_Call(Ctx*,void*),ov80_0222E268(void*,void*,u16,int,void*);
int FrtCmd_014(Ctx*);int FrtCmd_015(Ctx*);
int FrtCmd_014(Ctx*c){int condition=*c->cursor++;int delta=FrontierScriptContext_ReadWord(c);if(ov80_0223B9D8[condition][c->result]==1)FrontierScriptContext_Call(c,c->cursor+delta);return 0;}
int FrtCmd_015(Ctx*c){u16 id=FrontierScriptContext_ReadHalfWord(c);struct{u8 a,b,d,e;}v;v.a=0;v.b=0;v.d=1;v.e=0;ov80_0222E268(c->p,c->data,id,0,&v);return 0;}
