#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    char *buf;
    int max;
    int count;
} HeaderList;
typedef struct {
    u8 pad0[0x1a08];
    char *response;
    u8 pad1a0c[0x1a38 - 0x1a0c];
    char headers[0x100];
} HttpCtx;
extern const char ov00_022166C4[];
extern const char ov00_02216884[];
extern const char ov00_02216888[];
extern const char ov00_02216894[];
extern const char ov00_022166F4[];
extern const char ov00_02216898[];
extern const char ov00_0221689C[];
extern const char ov00_022168A0[];
char *strstr(const char *s, const char *sub);
int strncmp(const char *a, const char *b, u32 n);
int ov00_021E9DD8(HttpCtx *ctx, HeaderList *list, const char *key, const char *value);
int ov00_021E9F2C(HttpCtx *ctx, int flag);
// @fn ov00_021E9F2C
int ov00_021E9F2C(HttpCtx *ctx, int flag) {
    char *vp1;
    char *vp7;
    char vc0;
    char vc1;
    HeaderList list;
    char vc2;
    int vi0;
    char *vp3;
    char *vp4;
    char *vp5;
    char *vp0;
    char *vp2;
    char *vp6;
    list.buf = ctx->headers;
    list.max = 0x20;
    list.count = 0;
    MI_CpuFill8(ctx->headers, 0, 0x100);
    vp3 = ctx->response;
    vp7 = strstr(vp3, ov00_022166C4);
    if (vp7 == NULL) {
        return 0;
    }
    vp6 = vp7 + 4 + strlen(vp7 + 4);
    vp5 = strstr(vp3, ov00_02216884);
    if (vp5 == NULL) {
        return 0;
    }
    vc1 = vp5[4];
    vp5[4] = 0;
    vi0 = ov00_021E9DD8(ctx, &list, ov00_02216888, vp5 + 1);
    if (vi0 != 1) {
        vp5[4] = vc1;
        return 0;
    }
    vp5[4] = vc1;
    if (flag == 1 || strncmp(vp5 + 1, ov00_02216894, 3) != 0) {
        return 1;
    }
    vp2 = strstr(vp5 + 5, ov00_022166F4);
    if (vp2 == NULL) {
        return 0;
    }
    vp2 += 2;
    while (vp2[0] != '\r' && vp2[1] != '\n') {
        vp4 = strstr(vp2, ov00_02216898);
        if (vp4 == NULL) {
            break;
        }
        vc0 = *vp4;
        *vp4 = 0;
        vp5 = vp4 + 2;
        vp1 = strstr(vp5, ov00_022166F4);
        if (vp1 == NULL) {
            *vp4 = vc0;
            break;
        }
        vc2 = *vp1;
        *vp1 = 0;
        if (ov00_021E9DD8(ctx, &list, vp2, vp5) != 1) {
            *vp4 = vc0;
            *vp1 = vc2;
            return 0;
        }
        vp2 = vp5 + strlen(vp5);
        *vp4 = vc0;
        *vp1 = vc2;
        vp2 += 2;
    }
    vp2 = vp7 + 4;
    if (vp2 < vp6) {
        do {
            vp0 = strstr(vp2, ov00_0221689C);
            if (vp0 == NULL) {
                break;
            }
            vc0 = *vp0;
            *vp0 = 0;
            vp3 = vp0 + 1;
            vp1 = strstr(vp3, ov00_022168A0);
            if (vp1 == NULL) {
                vp1 = strstr(vp3, ov00_022166F4);
            }
            if (vp1 != NULL) {
                vc2 = *vp1;
                *vp1 = 0;
            }
            if (ov00_021E9DD8(ctx, &list, vp2, vp3) != 1) {
                *vp0 = vc0;
                if (vp1 != NULL) {
                    *vp1 = vc2;
                }
                return 0;
            }
            vp2 = vp3 + strlen(vp3);
            *vp0 = vc0;
            vp2++;
            if (vp1 != NULL) {
                *vp1 = vc2;
            }
        } while (vp2 < vp6);
    }
    return 1;
}
