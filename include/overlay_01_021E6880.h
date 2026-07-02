#ifndef POKEHEARTGOLD_OV_01_021E6880_H
#define POKEHEARTGOLD_OV_01_021E6880_H

#include "field_system.h"
#include "unk_02031B0C.h"

typedef struct Soundplate {
    u8 soundplateSoundID;
    u8 volumeIndex;
    u8 unk2;
    u8 unk3;
    u8 x;
    u8 z;
    u8 xBounds;
    u8 zBounds;
} Soundplate;

typedef struct SoundplateStruct {
    u8 unk0[2];
    vu16 unk2; // fakematch?
    Soundplate soundplates[];
} SoundplateStruct;

BOOL ov01_021E690C(FieldSystem *fieldSystem);
void ov01_021F6B64_thunk(FieldSystem *fieldSystem, int arg1);
void FieldInput_Update(FieldInput *fieldInput, FieldSystem *fieldSystem, u16 newKeys, u16 heldKeys);
int FieldInput_Process(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_Colosseum(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_UnionRoom(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_BattleTower(FieldInput *fieldInput, FieldSystem *fieldSystem);
void ov01_021E7F00(FieldSystem *fieldSystem, BOOL arg1);
u32 ov01_021E7F54(FieldSystem *fieldSystem);

// TODO: Put in the relevant headers.
int sub_0205DD94(PlayerAvatar *avatar, u16 newKeys, u16 heldKeys);
BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);
BOOL sub_0203DC64(FieldSystem *fieldSystem, LocalMapObject** localMapObject);
u16 GetInteractedBackgroundEventScript(FieldSystem *fieldSystem, BG_EVENT *bgEvents, int numEvents);
void ov01_021F6B64(FieldSystem *fieldSystem, int arg1);
BOOL ov01_021F1D94(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents);
u32 sub_0203DDA4(FieldSystem *fieldSystem, BG_EVENT *bgEvents, int numEvents);
void sub_02059D44(FieldSystem *fieldSystem);
BOOL sub_02037958();
u8 sub_02057F18(BOOL arg0);
BOOL sub_02058740();
u16 sub_02033250();
void sub_020380B0();
BOOL sub_020548C0(FieldSystem *fieldSystem, int x, int z);
BOOL ov02_0224E35C(FieldSystem *fieldSystem);
BOOL ov02_0224E4CC(u8 tile, int arg1);
BOOL ov02_0224E4DC(u8 tile, int arg1);
BOOL ov04_02255090(FieldSystem *fieldSystem);
BOOL ov01_021F3114(FieldSystem *fieldSystem, PlayerAvatar *avatar, int facingDirection);
void ov04_02256BE4(FieldSystem *fieldSystem, u8 arg1);
u16 sub_0203DE04(FieldSystem *fieldSystem, const COORD_EVENT *coordEvent, int numEvents);
BOOL PlayerStepEvent_RepelCounterDecrement(SaveData* saveData, FieldSystem* fieldSystem);
void ov01_021FB630(u32 unk20);
void sub_02032058(SaveApricornBox *saveApricornBox, u32 steps);
void sub_02006088(u16 sndseq);
SoundplateStruct *sub_02054874(FieldSystem *fieldSystem, u32 x, u32 z);

#endif
