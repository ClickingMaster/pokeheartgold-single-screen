#include "global.h"

#include "bg_window.h"
#include "pokepic.h"
#include "sprite_system.h"

typedef struct W10Work {
    u8 pad00[0x14];
    void *owner14;
    u8 pad18[0x8];
    SpriteManager *sprites20;
    u8 pad24[0x14];
    ManagedSprite *sprite38;
    u8 pad3C[0x98];
    s32 rotationD4;
} W10Work;

BOOL ov07_022223CC(void *a, void *b, ManagedSprite *sprite);
BOOL ov07_02222440(void *a);
void ov07_0221C448(void *owner, int arg);
void ov07_02232508(void *work);
void ov07_0222928C(int arg0, W10Work *work);

typedef struct W10EndWork {
    u8 pad00[8];
    void *owner08;
    u8 pad0C[0x14];
    ManagedSprite *sprite20;
    ManagedSprite *sprite24;
    u8 pad28[0x2C];
} W10EndWork;

void *ov07_022324D8(void *owner, u32 size);
void ov07_02231FE4(void *owner, void *work);
ManagedSprite *ov07_0221C4E8(void *owner, int index);
void ov07_02222AC4(void *work, int a, int b, int c, int d, int e);
void ov07_0221EB98(void *owner, int arg);
void ov07_0221C410(void *owner, void (*callback)(int, void *), void *work);
void ov07_0222A710(int arg0, void *work);
void ov07_0222A838(void *owner);

void ov07_0222A838(void *owner) {
    W10EndWork *work = ov07_022324D8(owner, sizeof(W10EndWork));
    ov07_02231FE4(owner, (u8 *)work + 4);
    work->sprite20 = ov07_0221C4E8(work->owner08, 0);
    ManagedSprite_SetOamMode(work->sprite20, GX_OAM_MODE_OBJWND);
    *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & 0xFFFF1FFF) | (0x04000000U >> 11);
    *(volatile u16 *)0x0400004A = (*(volatile u16 *)0x0400004A & ~0x3F) | 0x1B;
    *(volatile u16 *)0x0400004A = (*(volatile u16 *)0x0400004A & ~0x3F00) | 0x1700;
    work->sprite24 = ov07_0221C4E8(work->owner08, 1);
    ManagedSprite_SetOamMode(work->sprite24, GX_OAM_MODE_XLU);
    ov07_02222AC4((u8 *)work + 0x2C, 0x10, 2, 2, 0x10, 0x10);
    ov07_0221EB98(work->owner08, 2);
    ToggleBgLayer(2, 1);
    ov07_0221C410(work->owner08, ov07_0222A710, work);
}

void ov07_0222A4B4(int arg0, void *arg1);

Pokepic *ov07_0221FA48(void *owner, int index);
int ov07_0221C468(void *owner);
BOOL ov07_02222AF4(void *work);
int ov07_0221FAF8(void *owner, int index);
int ov07_02222D88(int a, int b);
int ov07_0221BFD0(void *owner);
void *ov07_02222CCC(int a, int b, int c, int d, int e, int f, int g, int h, int i);
void ov07_02222D3C(void *arg);
void ov07_0221EC7C(void *owner, int arg);
void ov07_0222A710(int arg0, void *arg1);
