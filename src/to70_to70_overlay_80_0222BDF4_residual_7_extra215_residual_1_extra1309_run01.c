#pragma force_active on
#include "global.h"
typedef struct{void **parent;u8 p[0x18];u8 *pc;} Context;
typedef struct{u16 value;u8 mode,pad;} Entry;
extern void *sub_0209680C(void *),*Frontier_GetLaunchArgs(void *),*Save_PlayerData_GetProfile(void *),*CommPlayerInfo_GetProfile(int);
extern u32 FrontierScriptContext_ReadWord(Context *);extern u16 FrontierScript_ReadVar(Context *),ov80_0222A7EC(void *);
extern int sub_02037474(void),Comm_CountConnected(void);extern void FrontierMap_AddObjectGfx(void *,Entry *);
int FrtCmd_034(Context *);
int FrtCmd_034(Context *ctx){void **parent=ctx->parent;void *w=sub_0209680C(parent[0]);u32 offset=FrontierScriptContext_ReadWord(ctx);u8 *saved=ctx->pc;Entry item;ctx->pc+=offset;for(;;){item.value=FrontierScript_ReadVar(ctx);if(item.value==0xfd13)break;item.mode=*ctx->pc++;if(item.value==0xeeee){void *args=Frontier_GetLaunchArgs(parent[0]);item.value=ov80_0222A7EC(Save_PlayerData_GetProfile(((void **)args)[2]));FrontierMap_AddObjectGfx(w,&item);}else if(item.value==0xeeef){if(sub_02037474()==1){int count=Comm_CountConnected();int i;for(i=0;i<count;i++){item.value=ov80_0222A7EC(CommPlayerInfo_GetProfile(i));FrontierMap_AddObjectGfx(w,&item);}}}else FrontierMap_AddObjectGfx(w,&item);}ctx->pc=saved;return 0;}
