#include "global.h"

extern void GF_AssertFail(void);
extern void ov112_021F135C(void *, int, int, int, int);
extern void ManagedSprite_SetAnimNoRestart(void *, int);
extern void ManagedSprite_SetPositionXY(void *, int, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern int ManagedSprite_GetDrawFlag(void *);
extern int ManagedSprite_IsAnimated(void *);
extern int ManagedSprite_GetActiveAnim(void *);
extern void ManagedSprite_SetAnim(void *, int);
extern void Sprite_DeleteAndFreeResources(void *);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern void SpriteSystem_Free(void *);
extern void Heap_Free(void *);
extern void FreeBgTilemapBuffer(void *, int);
extern void RemoveWindow(void *);
extern void MessageFormat_Delete(void *);
extern void DestroyMsgData(void *);
extern void FontID_Release(int);
extern void PlayBGM(int);
extern u32 ov112_021FF9B8[];
extern void ov112_021F1FD0(void *);
extern void ov112_021F1EFC(void *, int);
void ov112_021F0C30(void *, u32, u32, u32, u32);
int ov112_021F0F48(void *, int);
u8 ov112_021F0F60(void *);
int ov112_021F0F70(u32 **, u32 **);
void ov112_021F1324(void *);
void ov112_021F13BC(void *, int, int, int);
void ov112_021F178C(void *);
void ov112_021F1814(void *);
void ov112_021F194C(void *, u32);
void ov112_021F1A08(void *);
void ov112_021F1CC8(void *, int);
void ov112_021F1D28(void *, int);
void ov112_021F1D58(void *, int);
void ov112_021F1F3C(void *, int);
void ov112_021F1F54(void *, int, int, int);
void ov112_021F1FD0(void *);


int ov112_021F0F48(void *p, int which) {
    if (which != 0) return ((u32)*(u8 *)((u8 *)p + 0x85) << 24) >> 31;
    return ((u32)*(u8 *)((u8 *)p + 0x86) << 24) >> 31;
}

u8 ov112_021F0F60(void *p) {
    u32 value = *(u8 *)((u8 *)p + 0x76);
    if (value >= 8) value = 0;
    return value;
}

int ov112_021F0F70(u32 **a, u32 **b) {
    if (**a == **b) return 0;
    if (**a > **b) return 1;
    return -1;
}
