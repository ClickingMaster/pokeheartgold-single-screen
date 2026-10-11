#ifndef PRIVATE_UNK_02087E70_H
#define PRIVATE_UNK_02087E70_H
#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "system.h"

typedef struct BattleAppWork {
    u8 pad00[0x10];
    int *statePtr;
    u8 pad14[0x44];
    u32 result58;
    u8 value5C;
    u8 pad5D[0x693];
    void *obj6F0;
    void *obj6F4;
    u8 pad6F8[0x138];
    void *saveData;
} BattleAppWork;

extern u8 SDK_OVERLAY_OVY_40_ID;
#define OVERLAY_40_ID ((FSOverlayID) & SDK_OVERLAY_OVY_40_ID)

void StopAllSE(int);
BattleAppWork *sub_02087A78(OverlayManager *);
void VsRecorder_MenuPop(BattleAppWork *);
void GF_SndHandleSetPlayerVolume(u32, u32);
BOOL sub_02055198(void *, u16);
void *Save_Misc_Get(void *);
void sub_0202AC0C(void *, u8 *);
u32 VsRecorder_GetBlendColor(BattleAppWork *);
void VsRecorder_InitResources(BattleAppWork *);
BOOL VsRecorder_Main(BattleAppWork *, int *);
void VsRecorder_DestroyDecorTasks(BattleAppWork *);
void VsRecorder_DeleteDecorSprites(BattleAppWork *);
void VsRecorder_DeleteMenuRowSprites(BattleAppWork *);
void VsRecorder_FreeMenuRowText(BattleAppWork *);
BOOL OrbitCursor_Delete(void *);
void VsRecorder_FreeResources(BattleAppWork *);
void UnloadOverlayByID(FSOverlayID);
void GfGfx_SwapDisplay(void);
void Field_SetEnvironmentSoundState_None_Unk2(void);

int sub_02087E70(OverlayManager *, int *, BOOL);
int sub_02087EF8(OverlayManager *, int *);
int sub_02087F04(OverlayManager *, int *);
int sub_02087F10(OverlayManager *, int *);
int sub_02087F2C(OverlayManager *, int *);
#endif
