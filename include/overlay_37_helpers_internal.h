#ifndef OVERLAY_37_PRIVATE_H
#define OVERLAY_37_PRIVATE_H

#include "global.h"

#include "overlay_37_work.h"

typedef struct Ov37YesNoTemplate {
    void *bgConfig;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
} Ov37YesNoTemplate;


/* module-local rodata blobs, left in the assembly */
extern const Ov37StateEntry ov37_021E7D20[];
extern const u8 _021E7968[];

/* external game functions */
void GF_RunVramTransferTasks(void);
void OamManager_ApplyAndResetBuffers(void);
void DoScheduledBgGpuUpdates(void *bgConfig);
void Heap_Free(void *ptr);
void String_Delete(void *str);
void YesNoPrompt_Destroy(void *prompt);
void YesNoPrompt_Reset(void *prompt);
u32 YesNoPrompt_InitFromTemplate(void *prompt, const Ov37YesNoTemplate *tmpl);
void FreeBgTilemapBuffer(void *bgConfig, u32 layer);
void RemoveWindow(Ov37Window *window);
void ClearFrameAndWindow2(Ov37Window *window, u32 arg1);
u32 TextPrinterCheckActive(u8 printerId);
void RemoveTextPrinter(u8 printerId);
void Sprite_SetAnimCtrlSeq(void *sprite, u32 seq);
void Sprite_SetDrawFlag(void *sprite, u32 flag);
u32 sub_02034818(u32 index);
u32 Comm_GetLocalNetId(void);
u32 Comm_CountConnected(void);
void sub_02038C1C(u32 arg0);
u32 sub_02037B38(u32 arg0);
void sub_02037AC0(u32 arg0);
void BufferPlayersName(void *msgFormat, u32 idx, u32 arg2);
void PlaySE(u32 seq);
void BeginNormalPaletteFade(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 heapId);
void GF_AssertFail(void);
s32 GF_SinDeg(u16 deg);
void GX_LoadOBJPltt(const void *src, u32 offset, u32 size);
u32 TouchscreenHitbox_FindRectAtTouchNew(const void *hitboxes);

/* module functions */
void ov37_021E5CC8(void *bgConfig);
void ov37_021E5F20(Ov37Work *work);
void ov37_021E5F5C(void *bgConfig);
void ov37_021E6540(Ov37Work *work);
void ov37_021E6818(void **sprites, u32 sel);
void ov37_021E6848(Ov37SpriteHolder *holder, u32 arg1);
void ov37_021E68AC(Ov37Work *work);
u32 ov37_021E68D0(Ov37Work *work, u32 arg1);
u32 ov37_021E6928(Ov37Work *work, u32 arg1);
u32 ov37_021E694C(Ov37Work *work, u32 arg1);
u32 ov37_021E6980(Ov37Work *work, u32 arg1);
u32 ov37_021E6BFC(Ov37Work *work, u32 arg1);
u32 ov37_021E6B40(Ov37Work *work, u32 arg1);
u32 ov37_021E6C38(Ov37Work *work, u32 arg1);
u32 ov37_021E6C58(Ov37Work *work, u32 arg1);
u32 ov37_021E6C84(Ov37Work *work, u32 arg1);
u32 ov37_021E6CC0(Ov37Work *work, u32 arg1);
u32 ov37_021E6DD0(Ov37Work *work, u32 arg1);
u32 ov37_021E6E04(Ov37Work *work, u32 arg1);
u32 ov37_021E6E2C(Ov37Work *work, u32 arg1);
u32 ov37_021E6E6C(Ov37Work *arg0, u32 arg1);
u32 ov37_021E6E70(Ov37Work *work, u32 arg1);
u32 ov37_021E6E90(Ov37Work *work, u32 arg1);
u32 ov37_021E6EB4(Ov37Work *work, u32 arg1);
u32 ov37_021E6F14(Ov37Work *work, u32 arg1);
void ov37_021E741C(u16 *arg0);
u32 ov37_021E745C(void);
void ov37_021E755C(Ov37Work *work);
u32 ov37_021E65EC(Ov37Work *work);
void ov37_021E72E8(Ov37Window *window, Ov37Entry *entries, Ov37Point *points, u32 arg3);
void ov37_021E73B4(Ov37Work *work);
u32 ov37_021E75C4(void);
u32 ov37_021E75E0(void);
u32 ov37_021E75E8(Ov37Work *work);
void ov37_021E762C(Ov37Work *work, u32 arg1, u32 arg2);
u32 ov37_021E76A0(u32 printerId);
void ov37_021E76C0(Ov37Work *work);
void ov37_021E76D0(Ov37Work *work, u32 arg1);
void ov37_021E7844(Ov37Work *work, u32 state);
void ov37_021E784C(Ov37Work *work, u32 arg1);
u32 ov37_021E7880(Ov37Work *work, const Ov37YesNoTemplate *tmpl);
void ov37_021E78A4(Ov37Work *work);
u32 ov37_021E78C4(void);
u32 ov37_021E6860(Ov37Work *work, u32 arg1);

#endif
