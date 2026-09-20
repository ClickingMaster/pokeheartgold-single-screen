#ifndef TO56_R3B_W16_PRIVATE_H
#define TO56_R3B_W16_PRIVATE_H
#include "global.h"

#include "constants/heap.h"
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
typedef struct Ov08Record16 {
    u8 p0[8];
    u16 species;
    u8 pA[0x11];
    u8 flags1B;
    u8 p1C[0x16];
    u8 form_value;
    u8 p33[0x1D];
} Ov08Record16;
typedef struct Ov08Root16 {
    u8 p0[8];
    void *battleSystem;
} Ov08Root16;
typedef struct Ov08Work16 {
    Ov08Root16 *root;
    u8 p04[0x1FB0];
    SpriteManager *spriteManager;
    u8 p1FB8[0x1C];
    ManagedSprite *sprites[6];
    u8 p1FEC[0x18];
    ManagedSprite *sprite2004;
    ManagedSprite *sprite2008;
} Ov08Work16;
typedef struct Ov08Pair16 {
    int a;
    int b;
    int c;
    int d;
} Ov08Pair16;
typedef struct Ov08RecBB4 {
    u8 p0[0x14];
    u8 first;
    u8 second;
} Ov08RecBB4;
extern u32 GetMonIconPaletteEx(u32, u32, u32);
extern void ManagedSprite_SetPaletteOverride(ManagedSprite *, int);
extern SpriteSystem *BattleSystem_GetSpriteSystem(void *);
extern int sub_020776B4(void);
extern int sub_02077678(u32);
extern int sub_0207769C(u32);
extern int sub_02077830(void);
extern int sub_02077800(u32);
extern int sub_02077818(u32);
extern void SpriteSystem_ReplaceCharResObj(SpriteSystem *, SpriteManager *, int, int, BOOL, int);
extern void ManagedSprite_SetAnim(ManagedSprite *, u32);
extern void ov08_02220A8C(ManagedSprite *, int, int);
extern BOOL ItemIdIsMail(u16);
void ov08_02220AAC(Ov08Work16 *);
void ov08_02220AEC(Ov08Work16 *, ManagedSprite *, int, u32);
void ov08_02220B3C(Ov08Work16 *, ManagedSprite *, u32);
void ov08_02220B90(int, ManagedSprite *, int, int);
void ov08_02220BB4(Ov08Work16 *, Ov08RecBB4 *, const Ov08Pair16 *);
void ov08_02220C08(u16, ManagedSprite *, int, int);
#endif
