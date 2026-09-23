#include "global.h"

extern void *Frontier_GetLaunchArgs(void *);
extern void *NamingScreen_CreateArgs(u32, u32, u32, u32, void *, void *);
extern void Frontier_LaunchApplication(void *, const void *, void *, u32, void *);
extern const u8 gOverlayTemplate_NamingScreen[];
extern void ov80_0222CB94(void *);
int FrtCmd_052(void *);

int FrtCmd_052(void *c) {
    void *launch = Frontier_GetLaunchArgs(*(void **)(*(void **)c));
    void *args = NamingScreen_CreateArgs(11, 0, 0, 8, *(void **)((u8 *)launch + 4), NULL);
    Frontier_LaunchApplication(*(void **)(*(void **)c), gOverlayTemplate_NamingScreen, args, 0, ov80_0222CB94);
    return 1;
}
