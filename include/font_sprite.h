#ifndef POKEHEARTGOLD_FONT_SPRITE_H
#define POKEHEARTGOLD_FONT_SPRITE_H

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"

typedef struct FontSystem FontSystem; // size: 0x68
typedef struct TextOBJLayout TextOBJLayout;
typedef struct FontOAMSprite FontOAMSprite;

// TODO: rename
typedef struct TextOBJTemplate {
    FontSystem *fontSystem;
    Window *window;
    SpriteList *spriteList;
    NNSG2dImagePaletteProxy *plttResourceProxy;
    Sprite *sprite;
    u32 offset;
    int x;
    int y;
    int unk_20;
    int unk_24;
    int vram;
    enum HeapID heapID;
} TextOBJTemplate;

typedef struct TextOBJ {
    FontOAMSprite *sprites; // one per OAM cell, NULL while the pool slot is free
    int spriteCount;
    const Sprite *parent;
    int x;
    int y;
} TextOBJ;

FontSystem *FontSystem_NewInit(int a0, enum HeapID heapId);
void TextOBJ_GetPosition(void *a0, int *a1, int *a2);
int FontOAM_GetCharSize(Window *window, NNS_G2D_VRAM_TYPE vram, int a2);
void FontSystem_Delete(FontSystem *a0);
void TextOBJ_SetDrawPriority(TextOBJ *textOBJ, int a1);
void TextOBJ_SetPosition(TextOBJ *textOBJ, int a1, int a2);
void TextOBJ_SetSpritesDrawFlag(TextOBJ *textOBJ, BOOL draw);
void TextOBJ_SetPaletteNum(TextOBJ *textOBJ, int pltt);
TextOBJLayout *TextOBJLayout_New(Window *window, enum HeapID heapID);
void TextOBJLayout_Delete(TextOBJLayout *a0);
u32 TextOBJLayout_GetCharSize(TextOBJLayout *a0, NNS_G2D_VRAM_TYPE vramType);
TextOBJ *TextOBJ_Create(const TextOBJTemplate *template, TextOBJLayout *a1);
void TextOBJ_Destroy(TextOBJ *textOBJ);
void TextOBJ_CopyFromBGWindow(TextOBJ *textOBJ, TextOBJLayout *a1, Window *window, enum HeapID heapID);
void FontOAM_CopyWindowChars(Window *window, int a1, int a2, int a3, int a4, void *charBuf);
TextOBJ *FontOAM_Create(TextOBJTemplate *textObjTemplate);
void TextOBJ_SetPalOffsetRespectVramOffset(TextOBJ *textObj, int palette);
void FontOAM_Delete(TextOBJ *textObj);

#endif // POKEHEARTGOLD_FONT_SPRITE_H
