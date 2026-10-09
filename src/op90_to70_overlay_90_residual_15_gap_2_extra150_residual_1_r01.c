#pragma force_active on
#include "global.h"
typedef struct Window {
    u8 data[0x10];
} Window;
typedef struct Sprite Sprite;
typedef struct Ov90Obj {
    Window window;
    Sprite *sprite1;
    Sprite *sprite2;
    u16 count;
    u8 index;
    u8 slot;
} Ov90Obj;
typedef struct Ov90Bg {
    void *bgConfig;
} Ov90Bg;
typedef struct Ov90Res {
    void *spriteList;
} Ov90Res;
extern const u8 ov90_0225C310[][12];
extern const u8 ov90_0225C314[][12];
extern const u8 ov90_0225C318[][12];
extern const s16 ov90_0225C274[];
extern const s16 ov90_0225C276[];
s32 ov90_0225A050(u32 count, int first, u32 second);
void SetBgPriority(u8, u16);
void AddWindowParameterized(void *bgConfig, Window *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile);
void FillWindowPixelBuffer(Window *window, u8 fillValue);
void CopyToBgTilemapRect(void *bgConfig, u8 bgId, u8 destX, u8 destY, u8 destWidth, u8 destHeight, const void *buffer, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight);
Sprite *ov90_02258EB4(void *, void *, u8, u8, int, int);
void Sprite_SetAnimCtrlSeq(Sprite *, int);
void Sprite_SetPalOffset(Sprite *, int);
void Sprite_SetDrawFlag(Sprite *, int);
void Sprite_SetAnimActiveFlag(Sprite *, int);
void ov90_02259BCC(Ov90Obj *obj, int count, int mode, u16 *src, Ov90Bg *bg, int index, u32 self, Ov90Res *res, void *renderer, int heapId);
// @fn ov90_02259BCC
void ov90_02259BCC(Ov90Obj *obj, int count, int mode, u16 *src, Ov90Bg *bg, int index, u32 self, Ov90Res *res, void *renderer, int heapId) {
    u32 off;
    int row;
    int anim;
    int y;
    int slot;
    s16 cy;

    switch (mode) {
    case 0:
    case 2:
        slot = ov90_0225A050(count, index, self);
        break;
    case 1:
        if (self > index) {
            slot = index + 1;
        } else if (self == index) {
            slot = 0;
        } else {
            slot = index;
        }
        break;
    default:
        slot = index;
        break;
    }
    SetBgPriority(1, 2);
    SetBgPriority(0, 1);
    off = count * 12;
    row = ((const u8 *)ov90_0225C310 + off)[slot];
    AddWindowParameterized(bg->bgConfig, &obj->window, 0, 5, row + 1, 0x1A, 4, 12, index * 0x68 + 0x201);
    FillWindowPixelBuffer(&obj->window, 0);
    CopyToBgTilemapRect(bg->bgConfig, 1, 0, row, 0x20, 6, src + 6, 0, ((const u8 *)ov90_0225C314 + off)[index], src[0] >> 3, src[1] >> 3);
    y = row * 8;
    anim = ((const u8 *)ov90_0225C318 + off)[slot];
    cy = ov90_0225C276[anim * 2];
    Sprite_SetAnimCtrlSeq(obj->sprite1 = ov90_02258EB4(renderer, res->spriteList, ov90_0225C274[anim * 2], cy + y, 0, heapId), anim);
    Sprite_SetPalOffset(obj->sprite1, index);
    Sprite_SetDrawFlag(obj->sprite1, FALSE);
    Sprite_SetAnimActiveFlag(obj->sprite1, TRUE);
    y += 0x15;
    obj->sprite2 = ov90_02258EB4(renderer, res->spriteList, 0x18, y, 0, heapId);
    Sprite_SetAnimCtrlSeq(obj->sprite2, 0);
    Sprite_SetDrawFlag(obj->sprite2, FALSE);
    obj->index = index;
    obj->slot = slot;
    obj->count = count;
}
