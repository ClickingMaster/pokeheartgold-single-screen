#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct {
    const char *url;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
} Ov00NasConfig;
extern char ov00_0221643C[];
extern char ov00_02216484[];
extern char ov00_0221A694[];
extern char ov00_0221A6A0[];
Ov00NasConfig ov00_02216420 = { ov00_0221643C, 0, 0x1000, 0, 0, 0, 20000 };
int strcmp(const char *a, const char *b);
int ov00_021E8FA0(void *req, Ov00NasConfig *config);
void DWCi_BM_GetWiFiInfo(void *info);
u32 ov00_021E879C(void *req, void *a, void *b);
int ov00_021E90BC(void *req);
void ov00_021E9154(void *req, u32 prio);
int ov00_021E7F38(int mode);
// @fn ov00_021E7F38
int ov00_021E7F38(int mode) {
    if (strcmp(ov00_02216420.url, ov00_02216484) != 0) {
        ov00_02216420.unk14 = 1;
    }
    ov00_02216420.unkC = W(P(ov00_0221A694, 8), 0x120c);
    ov00_02216420.unk10 = W(P(ov00_0221A694, 8), 0x1210);
    if (ov00_021E8FA0(P(P(ov00_0221A694, 8), 0x1314), &ov00_02216420) != 0) {
        return 4;
    }
    if (mode == 1) {
        DWCi_BM_GetWiFiInfo(ov00_0221A6A0);
    }
    W(P(ov00_0221A694, 8), 0x1004) = ov00_021E879C(P(P(ov00_0221A694, 8), 0x1314), (u8 *)P(ov00_0221A694, 8) + 0x1200, (u8 *)P(ov00_0221A694, 8) + 0x11cc);
    if (W(P(ov00_0221A694, 8), 0x1004) != 0) {
        return 4;
    }
    if (ov00_021E90BC(P(P(ov00_0221A694, 8), 0x1314)) != 0) {
        return 4;
    }
    ov00_021E9154(P(P(ov00_0221A694, 8), 0x1314), OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
    return 0;
}
