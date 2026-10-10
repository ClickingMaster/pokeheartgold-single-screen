#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void ov45_022444C8(void);
extern void *ov00_021FE5B4(int, int, void (*)(void));
int ov45_02244B9C(u8 *);

typedef struct {
    int kind;
    void *owner;
    u32 *payload;
    u32 pad;
    int count;
} S;

extern void ov00_021EC3D8(void *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char ov45_02255458[];
extern const char ov45_02255468[];
extern const char ov45_022553F8[];
extern const char ov45_02255478[];
extern const char ov45_02255488[];
extern const char ov45_022554A4[];
extern const char ov45_022554BC[];
extern const char ov45_022554DC[];
extern const char ov45_022554EC[];
void ov45_02244500(S *);

void ov45_02244500(S *s) {
    if (!s) {
        __msl_assertion_failed(ov45_02255458, ov45_02255468, ov45_022553F8, 171);
    }
    if (s->kind < 0) {
        __msl_assertion_failed(ov45_02255478, ov45_02255468, ov45_022553F8, 171);
    }
    if (s->kind >= 32) {
        __msl_assertion_failed(ov45_02255488, ov45_02255468, ov45_022553F8, 171);
    }
    if (!s->owner) {
        __msl_assertion_failed(ov45_022554A4, ov45_02255468, ov45_022553F8, 171);
    }
    if (!s->payload) {
        __msl_assertion_failed(ov45_022554BC, ov45_02255468, ov45_022553F8, 171);
    }
    if (s->count < 0) {
        __msl_assertion_failed(ov45_022554DC, ov45_02255468, ov45_022553F8, 171);
    }
    switch (s->kind) {
    case 0: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 1: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 2: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 3: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 4: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 5: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 6: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 7: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 8: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 9: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 10: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 11: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 12: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 13: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 14: {
        int i;
        u32 *p = s->payload;
        for (i = 0; i < (int)p[1]; i++) {
            ov00_021EC3D8(((void **)p[2])[i]);
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 15: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 16: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 17: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 18: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 19: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 20: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        for (i = 0; i < (int)p[5]; i++) {
            ov00_021EC3D8(((void **)p[6])[i]);
        }
        ov00_021EC3D8((void *)p[6]);
        break;
    }
    case 21: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 22: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 23: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 24: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 25: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 26: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 27: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        for (i = 0; i < (int)p[1]; i++) {
            ov00_021EC3D8(((void **)p[2])[i]);
        }
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 28: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 29: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
            if (p[4]) {
                ov00_021EC3D8(((void **)p[4])[i]);
            }
        }
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 30: {
        int i;
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        for (i = 0; i < (int)p[3]; i++) {
            ov00_021EC3D8(((void **)p[4])[i]);
            if (p[5]) {
                ov00_021EC3D8(((void **)p[5])[i]);
            }
        }
        ov00_021EC3D8((void *)p[4]);
        ov00_021EC3D8((void *)p[5]);
        break;
    }
    case 31: {
        u32 *p = s->payload;
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    default:
        __msl_assertion_failed(ov45_022554EC, ov45_02255468, ov45_022553F8, 485);
        break;
    }
    ov00_021EC3D8(s->payload);
}
