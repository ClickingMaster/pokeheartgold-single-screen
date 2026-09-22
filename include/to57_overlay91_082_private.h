#ifndef C82_H
#define C82_H
#include "global.h"
typedef struct NARC NARC;
typedef struct SpriteResource SpriteResource;
typedef struct Sprite Sprite;
typedef struct TextOBJ TextOBJ;
typedef struct Font Font;
typedef struct Layout Layout;
typedef struct Header82 {
    u32 w[9];
} Header82;
typedef struct Window82 {
    u32 w[4];
} Window82;
typedef struct Transfer82 {
    u32 w[3];
} Transfer82;
typedef struct Simple82 {
    void *spriteList;
    Header82 *header;
    VecFx32 position;
    u32 priority;
    int whichScreen;
    int heapID;
} Simple82;
typedef struct TextTemplate82 {
    void *font;
    Window82 *window;
    void *spriteList;
    void *plttResourceProxy;
    Sprite *sprite;
    u32 offset;
    int x, y, unk_20, unk_24, vram;
    int heapID;
} TextTemplate82;
SpriteResource *AddCharResObjFromOpenNarc(void *, NARC *, int, int, int, int, int);
SpriteResource *AddPlttResObjFromOpenNarc(void *, NARC *, int, int, int, int, int, int);
SpriteResource *AddCellOrAnimResObjFromOpenNarc(void *, NARC *, int, int, int, int, int);
SpriteResource *AddPlttResObjFromNarc(void *, int, int, int, int, int, int, int);
BOOL SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(SpriteResource *);
BOOL SpriteTransfer_CreatePlttTransferTask(SpriteResource *);
void sub_0200A740(SpriteResource *);
void CreateSpriteResourcesHeader(Header82 *, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *);
Sprite *Sprite_Create(const Simple82 *);
void Sprite_SetDrawFlag(Sprite *, BOOL);
void *String_New(u32, int);
void AddTextWindowTopLeftCorner(void *, Window82 *, int, int, int, int);
Layout *sub_02013910(Window82 *, int);
u32 sub_02013948(Layout *, int);
BOOL sub_02021AC8(u32, BOOL, int, Transfer82 *);
void *SpriteTransfer_GetPaletteProxy(SpriteResource *, void *);
TextOBJ *TextOBJ_Create(const TextTemplate82 *, Layout *);
void TextOBJ_SetSpritesDrawFlag(TextOBJ *, BOOL);
void RemoveWindow(Window82 *);
void ov91_0225D40C(void *, fx32, fx32, fx32, int);
void GF_AssertFail(void);
#endif
