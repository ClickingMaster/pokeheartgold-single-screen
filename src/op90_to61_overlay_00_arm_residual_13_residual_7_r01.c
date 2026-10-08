#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*AddrFunc)(u32 addr, void *param);
typedef void (*DataFunc)(const u8 *buf, int len, void *param);
typedef struct {
    u8 pad0[0xa0];
    AddrFunc onAddr;
    DataFunc onData;
    u8 pada8[0x10c - 0xa8];
    void *param;
} NatCtx;
typedef struct { u8 b[6]; } Magic6;
typedef struct { u8 b[4]; } Addr4;
extern const Magic6 ov00_022163E8;
void ov00_022118FC(NatCtx *ctx, const u8 *buf, int len);
// @fn ov00_022118FC
void ov00_022118FC(NatCtx *ctx, const u8 *buf, int len) {
    Magic6 magic = ov00_022163E8;
    u32 addr;
    int i;
    BOOL match = TRUE;
    if (len >= 10) {
        for (i = 0; i < 6; i++) {
            if (magic.b[i] != buf[i]) {
                match = FALSE;
                break;
            }
        }
    } else {
        match = FALSE;
    }
    if (match) {
        memcpy(&addr, buf + 6, 4);
        if (ctx->onAddr == NULL) {
            return;
        }
        ctx->onAddr(((addr >> 24) & 0xff) | ((addr >> 8) & 0xff00) | ((addr << 8) & 0xff0000) | ((addr << 24) & 0xff000000), ctx->param);
    } else {
        if (ctx->onData == NULL) {
            return;
        }
        ctx->onData(buf, len, ctx->param);
    }
}
