#ifndef OV40_RESIDUAL_4_PART4_PRIVATE_H
#define OV40_RESIDUAL_4_PART4_PRIVATE_H

#include "global.h"

#define PTR(p, off)   (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off)  (*(u32 *)((u8 *)(p) + (off)))
#define SWORD(p, off) (*(s32 *)((u8 *)(p) + (off)))

typedef struct Ov40SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} Ov40SpriteTemplate;

extern const u8 ov40_02244DC0[];

void *SpriteSystem_NewSprite(void *spriteSystem, void *spriteManager, const Ov40SpriteTemplate *tmpl);
void ManagedSprite_TickFrame(void *sprite);
void ManagedSprite_SetAnim(void *sprite, int anim);
void ManagedSprite_SetDrawFlag(void *sprite, int flag);
void ManagedSprite_SetPaletteOverrideOffset(void *sprite, int offset);
void PaletteData_BlendPalettes(void *data, int buffer, u16 selected, u8 amount, u16 color);
void sub_020136B4(void *p, int x, int y);
void ov40_0222D288(void *sprite, s16 x, s16 y);
void ov40_0222D294(void *sprite, s16 *x, s16 *y);
void ov40_0222D3E8(u8 *work, u8 *entry, int msgId, u8 *selection);

void ov40_0222CBE0(void *w);
void ov40_0222CCAC(void *w);
void ov40_0222CE7C(void *w);

#endif
