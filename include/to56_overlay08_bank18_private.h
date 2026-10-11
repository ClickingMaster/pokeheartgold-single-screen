#ifndef TO56_OVERLAY08_BANK18_PRIVATE_H
#define TO56_OVERLAY08_BANK18_PRIVATE_H
#include "global.h"

#include "move.h"

typedef struct Ov08MoveSlot {
    u16 move;
    u8 pad02[2];
    u8 value4;
    u8 value5;
    u8 pad06[2];
} Ov08MoveSlot;

typedef struct Ov08BattlerRecord {
    u8 pad00[4];
    u16 value04;
    u8 pad06[0x11];
    u8 flags17_low : 3;
    u8 flags17 : 4;
    u8 flags17_high : 1;
    u8 pad18[2];
    u16 value1A;
    u8 pad1C[0x11];
    u8 enabled2D;
    u8 pad2E[2];
    Ov08MoveSlot moves[4];
} Ov08BattlerRecord;

typedef struct Ov08BattleData {
    u8 pad00[0x11];
    u8 battler;
    u8 pad12[0x12];
    u16 move;
    u8 pad26[0x0e];
    u8 slot;
} Ov08BattleData;

typedef struct Ov08Work {
    Ov08BattleData *data;
    Ov08BattlerRecord battlers[6];
    u8 pad1E4[0x1FB8 - 0x1E4];
    void *sprites[38];
} Ov08Work;

typedef struct Ov08Coord {
    int x, y;
} Ov08Coord;
extern const Ov08Coord sBattlePartyForgetMoveTypeIconPos[];
extern const Ov08Coord sBattlePartyLearnMoveTypeIconPos[];
extern const Ov08Coord sBattlePartyCheckMovesTypeIconPos[];
extern const Ov08Coord sBattlePartyMoveDetailTypeIconPos[];
extern const Ov08Coord sBattlePartySummaryTypeIconPos[];
extern const Ov08Coord sBattlePartyCheckMovesMoveIconPos[];
extern const Ov08Coord sBattlePartyLearnMoveMoveIconPos[];
extern const Ov08Coord sBattlePartyListIconPos[];
extern const Ov08Coord sBattlePartyListStatusIconPos[];

extern void ManagedSprite_SetDrawFlag(void *, BOOL);
extern void BattlePartyMenu_ShowSpriteAt(void *, int, int);
extern void BattlePartyMenu_ShowStatusIcon(int, void *, int, int);
extern void BattlePartyMenu_ShowMonTypeIcons(Ov08Work *, Ov08BattlerRecord *, const void *);
extern void BattlePartyMenu_ShowHeldItemIcon(u16, void *, int, int);
extern void BattlePartyMenu_ShowCapsuleIcon(BOOL, void *, int, int);
extern void BattlePartyMenu_SetTypeIcon(Ov08Work *, void *, int, u8);
extern void BattlePartyMenu_SetMoveCategoryIcon(Ov08Work *, void *, u32);
extern void BattlePartyMenu_ShowMoveIcons(void *);
extern void BattlePartyMenu_ShowConditionMarkers(void *);

void BattlePartyMenu_ShowPageSprites(Ov08Work *, u32);
void BattlePartyMenu_ShowPartyListSprites(Ov08Work *);
void BattlePartyMenu_ShowMonMenuSprites(Ov08Work *);
void BattlePartyMenu_ShowSummarySprites(Ov08Work *);
void BattlePartyMenu_ShowCheckMovesSprites(Ov08Work *);
void BattlePartyMenu_ShowMoveDetailSprites(Ov08Work *);
void BattlePartyMenu_ShowLearnMoveSprites(Ov08Work *);
void BattlePartyMenu_ShowForgetMoveSprites(Ov08Work *);
void BattlePartyMenu_ShowRestorePPSprites(Ov08Work *);
void BattlePartyMenu_ShowContestForgetMoveSprites(Ov08Work *);
void BattlePartyMenu_ShowContestLearnMoveSprites(Ov08Work *);
void BattlePartyMenu_ShowMoveTypeIcons(Ov08Work *);
void BattlePartyMenu_ShowMoveContestTypeIcons(Ov08Work *);
#endif
