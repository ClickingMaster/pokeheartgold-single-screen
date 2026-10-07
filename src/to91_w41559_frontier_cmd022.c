#include "global.h"
#include "frontier/frontier_script_context.h"
#pragma force_active on
typedef struct {u8 head[0x44];void *callback;u8 middle[0x18];void *result;u8 tail[0x58];} Context;
typedef char ContextSize[sizeof(Context)==0xbc?1:-1];
extern void *ov80_0222BE24(void *,u32);
extern void *ov80_0222E558(void *,u8,u8,u8,u8,u16 *,void *,void *);
BOOL FrtCmd_022(FrontierScriptContext *);
static inline u8 ReadOperand(FrontierScriptContext *c){const u8 *p=c->scriptPtr;c->scriptPtr=p+1;return *p;}
BOOL FrtCmd_022(FrontierScriptContext *c){int e,d,b,a;u16 f;u16 *p;Context *w;w=(Context *)c->frontierSystem;a=ReadOperand(c);b=ReadOperand(c);d=ReadOperand(c);e=ReadOperand(c);f=FrontierScriptContext_ReadHalfWord(c);p=(u16 *)ov80_0222BE24(c,f);w->result=ov80_0222E558(w,a,b,d,e,p,w->callback,c->msgLoader);c->data[0]=f;return TRUE;}
