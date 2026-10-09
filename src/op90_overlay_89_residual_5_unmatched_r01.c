#pragma force_active on
#include "global.h"
typedef struct {
    u16 x, y, width, height;
} Ov89Rect;
typedef struct {
    u32 unk0;
    u32 *values;
} Ov89Result;
typedef struct {
    void **ctx;
    u8 pad4[4];
    void *bgConfig;
    u8 padC[0x2C - 0xC];
    void *format;
    void *msgData;
    u8 windows[0x16C - 0x34];
    u32 selected;
    u32 ids[8];
    u8 pad190[0x8D3 - 0x190];
    u8 count;
    u8 pad8D4[0x96C - 0x8D4];
    void *focus;
    void *sprites[8];
    u8 pad990[0x99C - 0x990];
    u16 a[8];
    u16 b[8];
    u8 pad9BC[0x9C0 - 0x9BC];
    u16 tiles[1024];
} Ov89Work;
extern const Ov89Rect ov89_0225CBD8[];
extern const s16 ov89_0225CA18[][2];
void ov45_0222EC3C(Ov89Result *result);
void ov89_0225C724(void *msgData, void *format, void *windows, void *context, u32 id);
void ov89_0225C818(void *windows, int idx);
void *GetBgTilemapBuffer(void *bgConfig, u8 bgId);
void ScheduleBgTilemapBufferTransfer(void *bgConfig, u8 bgId);
void FillBgTilemapRect(void *bgConfig, u8 bgId, u16 fillValue, u8 x, u8 y, u8 width, u8 height, u8 mode);
void ManagedSprite_SetDrawFlag(void *sprite, int flag);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *sprite, s16 x, s16 y, fx32 offset);
void PlaySE(u16 se);
int ov89_02259F9C(Ov89Work *work);
#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off
#pragma opt_lifetimes off
#pragma opt_propagation off
// @fn ov89_02259F9C
int ov89_02259F9C(Ov89Work *work) {
    int added;
    int removed;
    Ov89Result result;
    const Ov89Rect *rects;
    long i;
    u16 *buffer;
    int n;
    const Ov89Rect *rect;
    u32 size;
    int y;
    added = 0;
    removed = 0;
    ov45_0222EC3C(&result);
    rects = ov89_0225CBD8;
    for (i = 0; i < 8; i++) {
        if (work->ids[i] != result.values[i]) {
            if (result.values[i] != (u32)-1) {
                ov89_0225C724(work->msgData, work->format, work->windows, *work->ctx, result.values[i]);
                buffer = GetBgTilemapBuffer(work->bgConfig, 6);
                rect = &rects[i];
                for (y = rect->y; y < rect->y + rect->height; y++) {
                    size = rect->width * 2;
                    MIi_CpuCopy16(&work->tiles[y * 32 + ((const u16 *)ov89_0225CBD8)[i * 4]], &buffer[y * 32 + ((const u16 *)ov89_0225CBD8)[i * 4]], size);
                }
                ScheduleBgTilemapBufferTransfer(work->bgConfig, 6);
                added++;
            } else {
                ov89_0225C818(work->windows, i);
                work->a[i] = 0;
                work->b[i] = 0;
                ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
                FillBgTilemapRect(work->bgConfig, 6, 0, ov89_0225CBD8[i].x, ov89_0225CBD8[i].y, ov89_0225CBD8[i].width, ov89_0225CBD8[i].height, 16);
                ScheduleBgTilemapBufferTransfer(work->bgConfig, 6);
                removed++;
            }
            if (work->selected == result.values[i]) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(work->focus, ov89_0225CA18[i][0], ov89_0225CA18[i][1], 17 << 16);
                ManagedSprite_SetDrawFlag(work->focus, TRUE);
            }
        }
        work->ids[i] = result.values[i];
    }
    n = 0;
    for (i = 0; i < 8; i++) {
        if (work->ids[i] != (u32)-1) {
            n++;
        }
    }
    work->count = n;
    if (added > 0) {
        PlaySE(1508);
        return 1;
    }
    if (removed > 0) {
        PlaySE(1508);
        return 2;
    }
    return 0;
}
