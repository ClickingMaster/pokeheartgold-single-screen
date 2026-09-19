#ifndef POKEHEARTGOLD_OVERLAY83_PARTY_UI_R21_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY83_PARTY_UI_R21_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void Heap_Free(void *);
u8 AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, void *);
void ov83_0224755C(void *, int);
void ov83_02247568(void *, u16, u16);
u32 ov83_02240EC4(void *, u16, u8);
void ov83_02240C48(void *, u32, u32, u32, u32);
void ov83_02241DD8(void *, void *, void *, int, int, int, int, u32, int);
void ov83_02242894(u32, u16 *, u16 *);
void ov83_02240664(void *);
void ov83_022407FC(void *);
void ov83_0224088C(void *);
void ov83_022428A8(void *, u32);
void ov83_022429E4(void *);

#endif
