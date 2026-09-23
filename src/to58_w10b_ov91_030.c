#include "global.h"
typedef struct Ov91Vec { u32 x, y, z; } Ov91Vec;
extern const Ov91Vec ov91_02261C28;
extern void ov91_0225DD50(void *, int);
extern void ov91_0225DD24(void *, int);
extern void ov91_0225E09C(void *, int);
void ov91_0225DC3C(void *, u8 *);
void ov91_0225DC3C(void *unused, u8 *dst) {
    Ov91Vec vec;
    *(u16 *)(dst + 6) = 0;
    ov91_0225DD50(dst, 0x7FFF);
    ov91_0225DD24(dst, 0xE38);
    dst[0] = 0;
    dst[1] = 1;
    *(u32 *)(dst + 0x94) = 0xC8000;
    *(u32 *)(dst + 0x98) = 123;
    *(u32 *)(dst + 8) = 0x50000;
    vec = ov91_02261C28;
    *(Ov91Vec *)(dst + 0x9C) = vec;
    ov91_0225E09C(dst, 0x9C4);
    *(Ov91Vec *)(dst + 0xD8) = vec;
    *(u32 *)(dst + 0xE4) = *(u32 *)(dst + 0xC0);
}
#include "global.h"
extern void ov91_0225DD24(void *, int);
extern int ov91_0225E044(void *, int);
extern int ov91_0225E070(void *, int);
extern void ov91_0225DD50(void *, int);
extern void *ov91_02261C58[];
void ov91_0225DCC4(u8 *, u8 *);
void ov91_0225DCC4(u8 *src, u8 *dst) {
    if (*(u32 *)src == 1) dst[1] = 0;
    else dst[1] = 1;
    int (*fn)(u8 *, int, int) = (int (*)(u8 *, int, int))ov91_02261C58[*(u32 *)src];
    int result = fn(dst, *(u32 *)(src + 8), *(u32 *)(src + 12));
    ov91_0225DD24(dst, result);
    *(u32 *)(dst + 8) = ov91_0225E044(dst, *(u16 *)(dst + 4));
    *(u32 *)(dst + 0xE4) = ov91_0225E070(dst, *(u16 *)(dst + 4));
    ov91_0225DD50(dst, (u16)(*(u16 *)(dst + 2) + *(u16 *)(dst + 6)));
}
