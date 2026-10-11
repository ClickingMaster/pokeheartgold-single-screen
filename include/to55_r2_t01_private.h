#ifndef TO55_T01_PRIVATE_H
#define TO55_T01_PRIVATE_H
#include "global.h"
typedef struct SimpleWork {
    u8 state;
    u8 pad[3];
    void *owner;
} SimpleWork;
void *Heap_Alloc(int, int);
void Heap_Free(void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void BattleAnimSystem_DestroySoundTask(void *, void *);
void BattleAnimSystem_CreateTask(void *, void (*)(void *, SimpleWork *), SimpleWork *, u32);
void BattleAnimSystem_CreateSoundTask(void *, void (*)(void *, SimpleWork *), SimpleWork *, u32);
void *SysTask_CreateOnMainQueue(void (*)(void *, SimpleWork *), SimpleWork *, u32);
void SysTask_Destroy(void *);
void Pokepic_SetAttr(void *, int, int);
void ov07_02223064(void *, SimpleWork *);
void ov07_0222308C(void *);
void ov07_022230B4(void *, SimpleWork *);
void ov07_022230DC(void *);
void ov07_02223104(void *, SimpleWork *);
void ov07_02223128(void *);
void ov07_02223224(void *);

typedef struct SpriteWork {
    u8 state;
    u8 pad03[3];
    void *owner;
    u8 pad08[4];
    void *spriteSystem;
    void *resource;
    void **sprite0;
    void **sprite1;
    void **sprite2;
} SpriteWork;
void Sprite_TickFrame(void *);
void Sprite_OffsetPositionXY(void *, int, int);
void SpriteSystem_DrawSprites(void *);
void Sprite_DeleteAndFreeResources(void *);
int PaletteData_GetSelectedBuffersBitmask(void *);
void *ov07_02222AF4(void *);
void BattleAnimBackground_ResetLayer(void *, int);
void ov07_0222314C(void *, SpriteWork *);
void ov07_02223E94(void *, void *);
void ov07_022247C8(void *, void *);
void ov07_0222519C(void *, void *);
#endif
