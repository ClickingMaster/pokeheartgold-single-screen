#ifndef TO57_NEW44_PRIVATE_H
#define TO57_NEW44_PRIVATE_H
#include "global.h"

typedef struct X07ScaleSprite {
    void *sprite;
    u8 scaleAnim[0x24];
    int flip;
    int delay;
    int start;
    int end;
} X07ScaleSprite;

typedef struct X07Group {
    void *owner;
    void *spriteSystem;
    void *spriteManager;
    int state;
    int timer;
    int index;
    void *pokepic;
    s16 x;
    s16 y;
    u8 motion[0x24];
    X07ScaleSprite sprites[4];
    u8 pad124[4];
} X07Group;

typedef struct X07ManagedSpriteTemplate {
    u8 data[0x34];
} X07ManagedSpriteTemplate;
void *ov07_022324D8(void *, u32);
void *ov07_0221C470(void *);
void *ov07_0221FA48(void *, void *);
int Pokepic_GetAttr(void *, int);
void ov07_022227A8(void *, s16, s16, s16, int);
void ov07_0221F9E8(X07ManagedSpriteTemplate *, void *);
void *SpriteSystem_NewSprite(void *, void *, const X07ManagedSpriteTemplate *);
void ov07_0222D4F4(X07ScaleSprite *, void *, s16, s16, BOOL);
void ov07_0221C410(void *, void (*)(void *, void *), void *);
void ov07_0222D6E0(void *, void *, void *, void *);
void ov07_0222D60C(void *, X07Group *);

typedef struct X07PairWork {
    void *owner;
    void *spriteSystem;
    void *spriteManager;
    int state;
    void *pic1;
    s16 x1;
    s16 y1;
    u8 movement[0x24];
    int distance;
    void *pic2;
    s16 x2;
    s16 y2;
    void *sprite;
    u8 linear[0x24];
    u8 sine[0x24];
} X07PairWork;
void *ov07_0221C468(void *);
int ov07_02222004(void *, void *);
void ov07_02222268(void *, s16, s16, s16, s16, int);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ov07_02222338(void *, void *, s16, s16, s16, s16, int, int);
BOOL ov07_022222B4(void *);
void ov07_022220B8(void *, void *, s16, s16);
BOOL ov07_022223CC(void *, void *, void *);
void ManagedSprite_SetDrawFlag(void *, BOOL);
void Sprite_DeleteAndFreeResources(void *);
void ov07_0221C448(void *, void *);
void Heap_Free(void *);
void SpriteSystem_DrawSprites(void *);
void Pokepic_SetAttr(void *, int, int);
void ov07_0222D7A4(void *, X07PairWork *);
void ov07_0222D974(void *, void *, void *, void *);
#endif
