#include "global.h"

extern void *Frontier_GetLaunchArgs(void *);
extern u16 *FrontierScript_ReadVarPtr(void *);
extern u16 sub_0202FD28(void *, u32, void *, u32);
int FrtCmd_112(void *ctx);

int FrtCmd_112(void *ctx) {
    void *launchArgs = Frontier_GetLaunchArgs(*(void **)(*(void **)ctx));
    u16 *target = FrontierScript_ReadVarPtr(ctx);
    u32 out;
    *target = sub_0202FD28(*(void **)((u8 *)launchArgs + 8), 11, &out, 0);
    return 0;
}
