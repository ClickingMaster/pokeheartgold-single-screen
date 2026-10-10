#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern char *ov00_021FF3D4(const char *);
extern void ov00_021FE754(void *, void *);

typedef struct {
    int kind;
    void *callback;
    void *data;
    int arg, count;
    char *name;
} Item;

#define ASSERT(test, sym, line) \
    if (!(test))                \
    __msl_assertion_failed(sym, ov45_02255468, ov45_022553C8, line)
#define FAIL()                    \
    {                             \
        ov00_021EC3D8(dst);       \
        ov00_021EC3D8(item.name); \
        return 0;                 \
    }
int ov45_02244D30(u8 *, int, void *, u32 *, int, int, char *, int);
extern char ov45_022553C8[];
extern char ov45_02255468[];
extern char ov45_022554EC[];
extern char ov45_022554F0[];
extern char ov45_02255500[];
extern char ov45_0225550C[];
extern char ov45_02255520[];
extern char ov45_02255544[];
extern char ov45_02255558[];
extern char ov45_02255570[];
extern char ov45_02255588[];
extern char ov45_02255590[];
extern char ov45_022555B0[];
extern char ov45_022555CC[];
extern char ov45_022555E8[];
extern char ov45_02255604[];
extern char ov45_02255620[];
extern char ov45_02255644[];

int ov45_02244D30(u8 *ctx, int kind, void *callback, u32 *src, int arg, int count, char *name, int size) {
    Item item;
    ASSERT(ctx, ov45_022554F0, 0x21f);
    ASSERT(kind >= 0, ov45_02255500, 0x221);
    ASSERT(kind < 32, ov45_0225550C, 0x222);
    ASSERT(*(void **)(ctx + 0x820), ov45_02255520, 0x223);
    ASSERT(callback, ov45_02255544, 0x224);
    ASSERT(src, ov45_02255558, 0x225);
    ASSERT(size, ov45_02255570, 0x226);
    ASSERT(count >= 0, ov45_02255588, 0x227);
    {
        u32 *zero = (u32 *)&item;
        zero[0] = 0;
        zero[1] = 0;
        zero[2] = 0;
        zero[3] = 0;
        zero[4] = 0;
        zero[5] = 0;
    }
    item.kind = kind;
    item.callback = callback;
    item.data = ov00_021EC3A8(size);
    if (!item.data) {
        return 0;
    }
    memcpy(item.data, src, size);
    item.arg = arg;
    item.count = count;
    if (!name) {
        item.name = NULL;
    } else {
        int length = strlen(name);
        item.name = ov00_021EC3A8(length + 1);
        if (!item.name) {
            ov00_021EC3D8(item.data);
            return 0;
        }
        memcpy(item.name, name, length + 1);
    }
    switch (item.kind) {
    case 0: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        break;
    }
    case 1: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        break;
    }
    case 2: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    case 3: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    case 4: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 5: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 6: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    case 7: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        if (src[4]) {
            dst[4] = (u32)ov00_021FF3D4((char *)src[4]);
            if (!dst[4]) {
                FAIL();
            }
        }
        break;
    }
    case 8: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 9: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    case 10: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021EC3A8(32);
            if (!dst[1]) {
                FAIL();
            }
            memcpy((void *)dst[1], (void *)src[1], 32);
        }
        break;
    }
    case 11: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    case 12: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        break;
    }
    case 13: {
        u32 *dst = item.data;
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        break;
    }
    case 14: {
        int i;
        u32 *dst = item.data;
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d3);
        if (!src[2]) {
            dst[2] = 0;
        } else {
            dst[2] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[2]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[2])[i]) {
                    ((u32 *)dst[2])[i] = 0;
                } else {
                    ((u32 *)dst[2])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[2])[i]);
                    if (!((u32 *)dst[2])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[2])[i]);
                        }
                        ov00_021EC3D8((void *)dst[2]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d4);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            dst[3] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d5);
        if ((int)src[1] > 0) {
            int bytes;
            ASSERT(src[4], ov45_022555B0, 0x2d5);
            bytes = src[1] * 4;
            dst[4] = (u32)ov00_021EC3A8(bytes);
            if (!dst[4]) {
                ov00_021EC3D8(dst);
                ov00_021EC3D8(item.name);
                return 0;
            }
            memcpy((void *)dst[4], (void *)src[4], bytes);
        }
        break;
    }
    case 15: {
        u32 *dst = item.data;
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 16: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 17: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021EC3A8(32);
            if (!dst[2]) {
                FAIL();
            }
            memcpy((void *)dst[2], (void *)src[2], 32);
        }
        break;
    }
    case 18: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        break;
    }
    case 19: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        ASSERT((int)src[2] >= 0, ov45_022555CC, 0x301);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[2] >= 0, ov45_022555CC, 0x302);
        if ((int)src[2] > 0) {
            int bytes;
            ASSERT(src[4], ov45_022555E8, 0x302);
            bytes = src[2] * 4;
            dst[4] = (u32)ov00_021EC3A8(bytes);
            if (!dst[4]) {
                FAIL();
            }
            memcpy((void *)dst[4], (void *)src[4], bytes);
        }
        break;
    }
    case 20: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        if (src[4]) {
            dst[4] = (u32)ov00_021FF3D4((char *)src[4]);
            if (!dst[4]) {
                FAIL();
            }
        }
        ASSERT((int)src[5] >= 0, ov45_02255590, 0x30e);
        if (!src[6]) {
            dst[6] = 0;
        } else {
            int i;
            dst[6] = (u32)ov00_021EC3A8(src[5] * 4);
            if (!dst[6]) {
                FAIL();
            }
            for (i = 0; i < (int)src[5]; i++) {
                if (!((u32 *)src[6])[i]) {
                    ((u32 *)dst[6])[i] = 0;
                } else {
                    ((u32 *)dst[6])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[6])[i]);
                    if (!((u32 *)dst[6])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[6])[i]);
                        }
                        ov00_021EC3D8((void *)dst[6]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 21: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        break;
    }
    case 22: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        if (src[4]) {
            dst[4] = (u32)ov00_021FF3D4((char *)src[4]);
            if (!dst[4]) {
                FAIL();
            }
        }
        break;
    }
    case 23: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 24: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        ASSERT((int)src[2] >= 0, ov45_02255604, 0x335);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 25: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        ASSERT((int)src[2] >= 0, ov45_02255620, 0x33e);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 26: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        break;
    }
    case 27: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        ASSERT((int)src[1] >= 0, ov45_022555CC, 0x350);
        if (!src[2]) {
            dst[2] = 0;
        } else {
            int i;
            dst[2] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[2]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[2])[i]) {
                    ((u32 *)dst[2])[i] = 0;
                } else {
                    ((u32 *)dst[2])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[2])[i]);
                    if (!((u32 *)dst[2])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[2])[i]);
                        }
                        ov00_021EC3D8((void *)dst[2]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_022555CC, 0x351);
        if ((int)src[1] > 0) {
            int bytes;
            ASSERT(src[3], ov45_022555E8, 0x351);
            bytes = src[1] * 4;
            dst[3] = (u32)ov00_021EC3A8(bytes);
            if (!dst[3]) {
                FAIL();
            }
            memcpy((void *)dst[3], (void *)src[3], bytes);
        }
        break;
    }
    case 28: {
        u32 *dst = item.data;
        if (src[0]) {
            dst[0] = (u32)ov00_021FF3D4((char *)src[0]);
            if (!dst[0]) {
                FAIL();
            }
        }
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        if (src[3]) {
            dst[3] = (u32)ov00_021FF3D4((char *)src[3]);
            if (!dst[3]) {
                FAIL();
            }
        }
        break;
    }
    case 29: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        ASSERT((int)src[2] >= 0, ov45_02255644, 0x365);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[2] >= 0, ov45_02255644, 0x366);
        if (!src[4]) {
            dst[4] = 0;
        } else {
            int i;
            dst[4] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[4]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[4])[i]) {
                    ((u32 *)dst[4])[i] = 0;
                } else {
                    ((u32 *)dst[4])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[4])[i]);
                    if (!((u32 *)dst[4])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[4])[i]);
                        }
                        ov00_021EC3D8((void *)dst[4]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 30: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        if (src[2]) {
            dst[2] = (u32)ov00_021FF3D4((char *)src[2]);
            if (!dst[2]) {
                FAIL();
            }
        }
        ASSERT((int)src[3] >= 0, ov45_02255644, 0x370);
        if (!src[4]) {
            dst[4] = 0;
        } else {
            int i;
            dst[4] = (u32)ov00_021EC3A8(src[3] * 4);
            if (!dst[4]) {
                FAIL();
            }
            for (i = 0; i < (int)src[3]; i++) {
                if (!((u32 *)src[4])[i]) {
                    ((u32 *)dst[4])[i] = 0;
                } else {
                    ((u32 *)dst[4])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[4])[i]);
                    if (!((u32 *)dst[4])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[4])[i]);
                        }
                        ov00_021EC3D8((void *)dst[4]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[3] >= 0, ov45_02255644, 0x371);
        if (!src[5]) {
            dst[5] = 0;
        } else {
            int i;
            dst[5] = (u32)ov00_021EC3A8(src[3] * 4);
            if (!dst[5]) {
                FAIL();
            }
            for (i = 0; i < (int)src[3]; i++) {
                if (!((u32 *)src[5])[i]) {
                    ((u32 *)dst[5])[i] = 0;
                } else {
                    ((u32 *)dst[5])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[5])[i]);
                    if (!((u32 *)dst[5])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[5])[i]);
                        }
                        ov00_021EC3D8((void *)dst[5]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 31: {
        u32 *dst = item.data;
        if (src[1]) {
            dst[1] = (u32)ov00_021FF3D4((char *)src[1]);
            if (!dst[1]) {
                FAIL();
            }
        }
        break;
    }
    default:
        __msl_assertion_failed(ov45_022554EC, ov45_02255468, ov45_022553C8, 0x380);
        break;
    }
    ov00_021FE754(*(void **)(ctx + 0x820), &item);
    return 1;
}
