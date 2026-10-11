#pragma force_active on
#include "global.h"
typedef struct { u32 words[4]; } Win;
typedef struct { void *font; Win *window; void *spriteList; void *pltt; void *unk10; u32 offset; int x; int y; int a; int b; int vram; int heap; } Tmpl;
typedef struct { u32 a; u32 b; u32 c; } Alloc;
typedef struct {
    u8 pad0[0x2F4];
    void *manager;
    void *bg;
    u8 pad2FC[0x304 - 0x2FC];
    u8 rects[29][4];
    void *font;
    void *sprites[2];
    Alloc alloc[3];
} Owner;
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *NewString_ReadMsgData(void *, int);
extern void InitWindow(Win *);
extern void AddTextWindowTopLeftCorner(void *, Win *, u8, u8, u16, u8);
extern int FontID_String_GetCenterAlignmentX(int, void *, int, int);
extern void AddTextPrinterParameterizedWithColor(Win *, int, void *, int, int, int, int, void *);
extern int FontOAM_GetCharSize(Win *, int, int);
extern void sub_02021AC8(int, int, int, Alloc *);
extern void *SpriteManager_GetSpriteList(void *);
extern void *SpriteManager_FindPlttResourceProxy(void *, int);
extern void *FontOAM_Create(Tmpl *);
extern void TextOBJ_SetPalOffsetRespectVramOffset(void *, int);
extern void String_Delete(void *);
extern void DestroyMsgData(void *);
extern void RemoveWindow(Win *);
void sub_02087090(Owner *w, int idx, int a2, int a3, int pal);
// @fn sub_02087090
void sub_02087090(Owner *w, int idx, int a2, int a3, int pal){Tmpl t; Win win; void *msg; void *str; s16 x, y; int size; msg = NewMsgDataFromNarc(0, 27, 38, 108); str = NewString_ReadMsgData(msg, idx + 2); InitWindow(&win); AddTextWindowTopLeftCorner(w->bg, &win, 10, 2, 0, 0); AddTextPrinterParameterizedWithColor(&win, 2, str, FontID_String_GetCenterAlignmentX(2, str, 0, 80), 0, 255, 0xF0D02, NULL); size = FontOAM_GetCharSize(&win, 1, 108); sub_02021AC8(size, 1, 1, &w->alloc[idx]); x = (w->rects[idx + 26][2] + w->rects[idx + 26][3]) / 2; y = (w->rects[idx + 26][0] + w->rects[idx + 26][1]) / 2; t.font = w->font; t.window = &win; t.spriteList = SpriteManager_GetSpriteList(w->manager); t.pltt = SpriteManager_FindPlttResourceProxy(w->manager, 1003); t.unk10 = NULL; t.offset = w->alloc[idx].b; t.x = (s16)(x - 40); t.y = (s16)(y - 7); t.a = 0; t.b = 0; t.vram = 1; t.heap = 108; w->sprites[idx] = FontOAM_Create(&t); TextOBJ_SetPalOffsetRespectVramOffset(w->sprites[idx], pal); String_Delete(str); DestroyMsgData(msg); RemoveWindow(&win);}
