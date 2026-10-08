#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x110];
    char f110[0x34];
    char f144[0x177 - 0x144];
    char password[0x1f4 - 0x177];
    char buf[0x46c - 0x1f4];
    int f46c;
    int f470;
} LoginCtx;
typedef struct {
    LoginCtx *ctx;
} LoginConn;
typedef struct {
    u8 pad0[0x2c2];
    char name[0x40];
} LoginInfo;
extern const char ov00_022189C8[], ov00_022189D4[], ov00_022189DC[], ov00_022189E4[], ov00_02218990[], ov00_0221899C[], ov00_0221B16C[], ov00_022189A8[], ov00_0221893C[], ov00_022189F4[], ov00_022189B8[], ov00_022189C0[];
void ov00_021FFB40(u32 seed);
int ov00_021FFB5C(int min, int max);
void ov00_021FFC18(const char *src, char *dst, u32 len, int mode);
void ov00_022020EC(LoginConn *conn, char *buf, const char *str);
void ov00_0220211C(LoginConn *conn, char *buf, int value);
int ov00_022037CC(LoginConn *conn, LoginInfo *info);
static inline void EncodeString(const char *src, char *tmp, char *dst) {
    u32 len = strlen(src);
    u32 i;
    ov00_021FFB40(0x79707367);
    for (i = 0; i < len; i++) {
        tmp[i] = src[i] ^ (s8)ov00_021FFB5C(0, 0xff);
    }
    tmp[i] = 0;
    ov00_021FFC18(tmp, dst, len, 1);
}

// @fn ov00_022037CC
int ov00_022037CC(LoginConn *conn, LoginInfo *info)
{
    char tmp2 [ 66 ];
    char tmp1 [ 31 ];
    LoginCtx * ctx = conn->ctx;
    char enc1 [ 45 ];
    char enc2 [ 90 ];
    u32 len;
    char *pv608;
    u32 i;
    EncodeString(ctx->password, tmp1, enc1);
    ov00_022020EC(conn, ctx->buf, ov00_022189C8);
    ov00_022020EC(conn, ctx->buf, ov00_022189D4);
    ov00_022020EC(conn, ctx->buf, ctx->f144);
    ov00_022020EC(conn, ctx->buf, ov00_022189DC);
    ov00_022020EC(conn, ctx->buf, ctx->f110);
    ov00_022020EC(conn, ctx->buf, ov00_022189E4);
    ov00_022020EC(conn, ctx->buf, enc1);
    ov00_022020EC(conn, ctx->buf, ov00_02218990);
    ov00_0220211C(conn, ctx->buf, ctx->f46c);
    ov00_022020EC(conn, ctx->buf, ov00_0221899C);
    ov00_022020EC(conn, ctx->buf, ov00_0221B16C);
    ov00_022020EC(conn, ctx->buf, ov00_022189A8);
    ov00_0220211C(conn, ctx->buf, ctx->f470);
    ov00_022020EC(conn, ctx->buf, ov00_0221893C);
    ov00_022020EC(conn, ctx->buf, ctx->f110 + 0x1f);
    if (0 != info->name[0])
    {
        len = strlen(info->name);
        ov00_021FFB40(0x79707367);
        i = 0;
        if (i < len) {
            pv608 = tmp2;
            do {
                *pv608 = info->name[i] ^ (s8)ov00_021FFB5C(0, 0xff);
                pv608++;
                i++;
            } while (i < len);
        }
        tmp2[i] = 0;
        ov00_021FFC18(tmp2, enc2, len, 1);
        ov00_022020EC(conn, ctx->buf, ov00_022189F4);
        ov00_022020EC(conn, ctx->buf, enc2);
    }
    ov00_022020EC(conn, ctx->buf, ov00_022189B8);
    ov00_022020EC(conn, ctx->buf, ov00_022189C0);
    return 0;
}
