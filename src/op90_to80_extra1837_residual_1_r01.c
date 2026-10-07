#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*AddrCallback)(u32 ip, u16 port, void *param);
typedef struct {
    u8 pad0[0xa8];
    AddrCallback callback;
    u8 padac[0x104 - 0xac];
    u32 ip;
    u16 port;
    u8 pad10a[2];
    void *param;
} AddrCtx;
extern const char ov00_02219EF0[];
int sscanf(const char *s, const char *fmt, ...);
void ov00_022112A8(AddrCtx *ctx, const char *str);
// @fn ov00_022112A8
void ov00_022112A8(AddrCtx *ctx, const char *str) {
    u32 ip;
    int portValue;
    u16 port;
    sscanf(str, ov00_02219EF0, &ip, &portValue);
    port = (u16)portValue;
    ip = ((ip >> 24) & 0xff) | ((ip >> 8) & 0xff00) | ((ip << 8) & 0xff0000) | ((ip << 24) & 0xff000000);
    if (ip == 0 || port == 0) {
        return;
    }
    if (ctx->ip == ip && ctx->port == port) {
        return;
    }
    ctx->ip = ip;
    ctx->port = port;
    ctx->callback(ip, port, ctx->param);
}
