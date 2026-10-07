#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void *(*AllocFunc)(const void *tag, u32 size);
typedef struct {
    u8 pad[0x1010];
    AllocFunc alloc;
} Ctx;
typedef struct {
    u8 *base;
    u8 *cursor;
    u8 *end;
    u32 size;
} Buf;
extern const u8 ov00_022167B8[];
int ov00_021E9B44(Ctx *ctx, Buf *buf, u32 size);
// @fn ov00_021E9B44
int ov00_021E9B44(Ctx *ctx, Buf *buf, u32 size) {
    AllocFunc alloc = ctx->alloc;
    if (size == 0) {
        return 0;
    }
    buf->base = alloc(ov00_022167B8, size);
    if (buf->base == NULL) {
        return 0;
    }
    buf->cursor = buf->base;
    buf->size = size;
    buf->end = buf->base + buf->size;
    return 1;
}
