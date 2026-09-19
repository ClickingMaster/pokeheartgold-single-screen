#ifndef W08_OV12_BC_PRIVATE_H
#define W08_OV12_BC_PRIVATE_H
#include "global.h"
typedef struct BattleSystem BattleSystem;
typedef struct BattleContext BattleContext;
typedef struct EscapeMessage {
    u8 command;
    u8 escaperMask;
    u16 recordedCount;
    u8 recorded[28];
} EscapeMessage;
typedef struct ForfeitMessage {
    u8 command;
    u8 pad;
    u16 recordedCount;
    u8 recorded[28];
} ForfeitMessage;
typedef struct SwapSpriteMessage {
    u8 command;
    u8 pad01;
    u16 pad02;
    s32 pad04;
    u16 pad08;
    u16 pad0A;
    u16 pad0C;
    u16 pad0E;
    u32 pad10;
    u16 pad14;
    u16 pad16;
    u16 species[4];
    u8 gender[4];
    u8 shiny[4];
    u8 form[4];
    u32 personality[4];
    u32 pad3C[4];
    int pad4C[3];
} SwapSpriteMessage;
typedef struct MoveSoundMessage { u8 command, effectiveness, pad[2]; } MoveSoundMessage;
typedef struct W08BattleMon {
    u16 species;
    u8 pad02[0x24];
    u8 form : 5;
    u8 shiny : 1;
    u8 unk26 : 2;
    u8 pad27[0x41];
    u32 personality;
    u8 pad6c[4];
    u32 status2;
    u8 pad74[0x0a];
    u8 gender : 4;
    u8 unk7e : 4;
    u8 pad7f[0x25];
    u32 transformedPersonality;
    u8 padA8[0x12];
    u16 transformedGender;
    u8 padBC[4];
} W08BattleMon;
typedef struct W08BattleContext {
    u8 pad[0x2d40];
    W08BattleMon battleMons[4];
} W08BattleContext;
u32 BattleSystem_GetBattleType(BattleSystem *);
int BattleSystem_GetMaxBattlers(BattleSystem *);
u32 MaskOfFlagNo(int);
BOOL sub_0202FC48(void);
u8 ov12_0223BE68(BattleSystem *, u8 *);
void ov12_02262240(BattleSystem *, int, int, const void *, int);
void GF_AssertFail(void);
void BattleController_EmitRunAwayMessage(BattleSystem *, BattleContext *);
void BattleController_EmitForefitMessage(BattleSystem *);
void BattleController_EmitSwapToSubstituteSprite(BattleSystem *, BattleContext *, int);
void BattleController_EmitPlayMoveSE(BattleSystem *, BattleContext *, int);
#endif
