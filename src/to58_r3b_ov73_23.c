#include "global.h"

extern u32 Save_SysInfo_GetDwcProfileId(void *);
extern void Save_SysInfo_SetDwcProfileId(void *, u32);
extern u64 DWC_CreateFriendKey(u32);
extern void NintendoWfcHttp_Init(u32, u32, u32);
int ov73_021E8E7C(void *);

int ov73_021E8E7C(void *work) {
    if (Save_SysInfo_GetDwcProfileId(*(void **)((u8 *)*(void **)work + 8)) == 0) {
        Save_SysInfo_SetDwcProfileId(
            *(void **)((u8 *)*(void **)work + 8),
            *(u32 *)((u8 *)*(void **)work + 0x1c));
    }
    u32 profile = Save_SysInfo_GetDwcProfileId(*(void **)((u8 *)*(void **)work + 8));
    u64 key = DWC_CreateFriendKey(*(u32 *)((u8 *)*(void **)work + 0x14));
    NintendoWfcHttp_Init(profile, (u32)key, (u32)(key >> 32));
    *(u32 *)((u8 *)work + 0x1c) = 7;
    return 3;
}
