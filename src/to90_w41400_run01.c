#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
struct Str {union {u32 flags;struct {u32 large:1;u32 cap:31;} bits;} u;u32 size;char *ptr;};
extern struct Str *ov45_02237DB4(struct Str *,u32,u32,struct Str *);
struct Str *ov45_02237D48(struct Str *,struct Str *);
#pragma force_active on
struct Str *ov45_02237D48(struct Str *dst,struct Str *src) {
    BOOL isLong = dst->u.bits.large ? TRUE : FALSE;
    if (!isLong && !src->u.bits.large) {
        *dst = *src;
        return dst;
    }
    return ov45_02237DB4(dst,0,
        isLong ? dst->size : ((u32)((const u8 *)dst)[0] << 24) >> 25,src);
}
