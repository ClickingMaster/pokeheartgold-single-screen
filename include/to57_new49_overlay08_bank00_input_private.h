#ifndef TO57_NEW49_OVERLAY08_BANK00_INPUT_PRIVATE_H
#define TO57_NEW49_OVERLAY08_BANK00_INPUT_PRIVATE_H
#include "global.h"

#include "system.h"

typedef struct Ov08N49Root {
    void *unk00;
    u8 pad04[4];
    void *unk08;
    u8 pad0C[5];
    u8 battler;
    u8 pad12[0x10];
    u16 item;
    u8 pad24[4];
    void *unk28;
    u8 data2C[4];
    u8 pad30[4];
    u8 selected;
} Ov08N49Root;

typedef struct Ov08N49Work {
    Ov08N49Root *root;
    u8 pad0004[0x1FA8 - 4];
    void *msgData;
    u8 pad1FAC[4];
    void *string;
    u8 pad1FB4[0x2077 - 0x1FB4];
    u8 modeLow : 4;
    u8 modeHigh : 4;
    u8 pad2078;
    u8 state;
    u8 pad207A[2];
    u8 field207C;
    u8 pad207D[0x2088 - 0x207D];
    void *smallState;
    u8 field208C;
    u8 result;
    u8 field208E;
} Ov08N49Work;

extern const u8 sBattlePartyLearnMoveHitboxes[];
extern const u8 sBattlePartyForgetMoveHitboxes[];
extern const u8 sBattlePartyRestorePPHitboxes[];
int BattlePartyMenu_CheckTouchHitbox(void *, const void *);
int BattleDpadCursor_HandleInput(void *);
void BattlePartyMenu_DeactivateCursor(void *);
void BattlePartyMenu_StartButtonAnim(void *, int);
void PlaySE(int);
int BattlePartyMenu_IsSelectedMoveHM(void *);
void BattlePartyMenu_PrintHMCantBeForgotten(void *);
void BattlePartyMenu_ClearBattleMoveInfo(void *);
void BattlePartyMenu_ClearContestMoveInfo(void *);
int BattleSystem_RecoverStatus(void *, int, int, int, int);
void ReadMsgDataIntoString(void *, int, void *);
void BattlePartyMenu_ShowMessage(void *);

int BattlePartyMenu_HandleLearnMoveInput(Ov08N49Work *);
int BattlePartyMenu_HandleForgetMoveInput(Ov08N49Work *);
int BattlePartyMenu_HandleRestorePPInput(Ov08N49Work *);
#endif
