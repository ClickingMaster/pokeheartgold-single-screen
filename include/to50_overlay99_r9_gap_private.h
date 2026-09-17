#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"

#include "overlay_98_helpers_internal.h"
#include "sprite_system.h"
typedef struct Ov99WorkR9 {
    u8 pad[0x404];
    Ov98SpriteContext *ctx;
    ManagedSprite *sprites[64];
} Ov99WorkR9;
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern const ManagedSpriteTemplate ov99_021EA180[];
void ov99_021E80D0(void *);
extern const u32 ov99_021E9FB0[];
void ManagedSprite_SetDrawFlag(ManagedSprite *, int);
void ov99_021E810C(Ov99WorkR9 *);
void ov99_021E8198(void *);
#include "message_format.h"
void ov98_0221EC24(Ov98TextContext *, u32, u32, u32);
void ov98_0221EC08(Ov98TextContext *, u32, u32, u32, u8);
void ov98_0221EBEC(Ov98TextContext *, u32, u32, u32, u8, u32);
void ov98_0221EBD8(Ov98TextContext *, u32, u32, u32);
void BufferPokeathlonMedalName(MessageFormat *, u32, u32);
int ov99_021E718C(void *);
void *ov99_021E7150(void *);
int ov99_021E724C(void *);
void ov99_021E8224(void *);

#endif
