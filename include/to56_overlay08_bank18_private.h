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
extern const Ov08Coord ov08_022253B4[];
extern const Ov08Coord ov08_022253C4[];
extern const Ov08Coord ov08_022253D4[];
extern const Ov08Coord ov08_022253E4[];
extern const Ov08Coord ov08_022253F4[];
extern const Ov08Coord ov08_02225454[];
extern const Ov08Coord ov08_0222550C[];
extern const Ov08Coord ov08_02225534[];
extern const Ov08Coord ov08_02225564[];

extern void ManagedSprite_SetDrawFlag(void *, BOOL);
extern void ov08_02220A8C(void *, int, int);
extern void ov08_02220B90(int, void *, int, int);
extern void ov08_02220BB4(Ov08Work *, Ov08BattlerRecord *, const void *);
extern void ov08_02220C08(u16, void *, int, int);
extern void ov08_02220C3C(BOOL, void *, int, int);
extern void ov08_02220AEC(Ov08Work *, void *, int, u8);
extern void ov08_02220B3C(Ov08Work *, void *, u32);
extern void ov08_022213C8(void *);
extern void ov08_02221500(void *);

void ov08_02220C5C(Ov08Work *, u32);
void ov08_02220CF4(Ov08Work *);
void ov08_02220D90(Ov08Work *);
void ov08_02220DE8(Ov08Work *);
void ov08_02220E80(Ov08Work *);
void ov08_02220F58(Ov08Work *);
void ov08_0222101C(Ov08Work *);
void ov08_02221088(Ov08Work *);
void ov08_0222114C(Ov08Work *);
void ov08_022211B8(Ov08Work *);
void ov08_02221230(Ov08Work *);
void ov08_02221294(Ov08Work *);
void ov08_02221328(Ov08Work *);
#endif
