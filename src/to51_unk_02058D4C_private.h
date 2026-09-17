#ifndef TO51_UNK_02058D4C_PRIVATE_H
#define TO51_UNK_02058D4C_PRIVATE_H
#include "global.h"

#include "screen_fade.h"
#include "system.h"

typedef struct FieldSystemMini {
    void *unk00;
    void *unk04;
    void *bgConfig;
    void *saveData;
    u8 pad10[0x2c];
    void *mapObjectManager;
    void *playerAvatar;
} FieldSystemMini;

typedef void (*WorkCallback)(int, void *);
typedef struct Work {
    void *unk_00;
    void *partyMenu;
    WorkCallback unk_08;
    void *unk_0C;
    void *unk_10;
    u8 unk_14[0x10];
    FieldSystemMini *fieldSystem;
    void *unk_28;
    void *unk_2C;
    int unk_30;
    int unk_34;
    int unk_38;
    u8 unk_3C;
    u8 unk_3D[6];
    u8 unk_43;
    u8 unk_44;
    u8 pad45[3];
    u8 *unk_48;
    u8 *unk_4C;
    void *unk_50;
    u8 unk_54[0x10];
    u8 unk_64[0x10];
    void *unk_74;
    void *unk_78;
    void *unk_7C;
    u8 unk_80;
    s8 unk_81;
    u8 unk_82;
    u8 unk_83;
    u8 unk_84;
    u8 unk_85;
    u16 unk_86;
    u8 unk_88;
    u8 unk_89;
} Work;

BOOL sub_02058D4C(void *);
void *TaskManager_GetEnvironment(void *);
FieldSystemMini *TaskManager_GetFieldSystem(void *);
void sub_02057E08(void);
void ReadMsgDataIntoString(void *, int, void *);
u8 sub_02059478(void *, void *);
BOOL DialogBox_IsPrintFinished(u8);
void sub_02037AC0(int);
BOOL sub_02037B38(int);
void sub_0205F55C(void *);
void sub_02059650(void *);
void Heap_Free(void *);
void sub_02057F70(void);
void *PlayerAvatar_GetMapObject(void *);
BOOL MapObject_IsMovementPaused(void *);
void FieldMap_FadeScreen(int);
void sub_020594C8(void *, BOOL);
void sub_02058B84(void *, int);
BOOL sub_02058C80(void *, void *);
void *SaveArray_Party_Get(void *);
void sub_02058AEC(void *, void *, void *, int, int, int);
BOOL sub_02058CD8(void *, void *);
void FieldSystem_LoadFieldOverlay(void *);
void sub_0203996C(void *);
BOOL sub_02058D04(void *);
BOOL sub_02058D24(void);
void sub_020596A8(void *, BOOL);
void BufferPlayersName(void *, int, void *);
void StringExpandPlaceholders(void *, void *, void *);
BOOL sub_020596F0(void *);
BOOL sub_02059738(void *);
BOOL sub_02059748(void *);
void sub_020597A8(void *);
void sub_02059820(void *, int);
int sub_02059A08(void *);
void sub_02059AD8(void *);
void *Party_GetMonByIndex(void *, int);
void *Mon_GetBoxMon(void *);
void BufferBoxMonSpeciesName(void *, int, void *);
void sub_0205993C(void *, int);
BOOL IsPaletteFadeFinished(void);
void sub_0205975C(void *);
BOOL sub_02059798(void *);
void sub_020597D4(void *);
void ClearFrameAndWindow2(void *, BOOL);
#endif
