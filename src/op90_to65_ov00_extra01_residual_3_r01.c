#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} SockAddrIn;
typedef struct {
    int sock;
    u8 pad4[0x24];
    int active;
} NnConn;
void ov00_0220F958(const u8 **data, int *len);
BOOL ov00_021FF6BC(int sock);
int ov00_021FF984(int sock, const u8 *data, int len, int flags, SockAddrIn *to, int toLen);
int ov00_021FFA60(int sock);
int ov00_0220E490(NnConn *conn, u32 ip, int port);
void ov00_0220F6E8(NnConn *conn);
void *ov00_0220EEEC(NnConn *conn, u32 ip, int port);
int ov00_0220C860(NnConn *conn, void *peer, u32 ip, int port, int a, const u8 *data, int len, int b);
int ov00_0220F474(NnConn *conn, u32 ip, int port, const u8 *data, int len);
// @fn ov00_0220F474
int ov00_0220F474(NnConn *conn, u32 ip, int port, const u8 *data, int len) {
    int err;
    ov00_0220F958(&data, &len);
    if (!ov00_021FF6BC(conn->sock)) {
        return 1;
    }
    {
        SockAddrIn to = {0};
        to.family = 2;
        to.port = ((port >> 8) & 0xff) | ((port << 8) & 0xff00);
        to.addr = ip;
        err = ov00_021FF984(conn->sock, data, len, 0, &to, sizeof(to));
    }
    if (err == -1) {
        err = ov00_021FFA60(conn->sock);
        if (err == -15) {
            if (ov00_0220E490(conn, ip, port) == 0) {
                return 0;
            }
        } else {
            if (err == -42 || err == -6) {
                return 1;
            }
            if (err != -35) {
                ov00_0220F6E8(conn);
                return 0;
            }
        }
    } else if (conn->active != 0) {
        if (ov00_0220C860(conn, ov00_0220EEEC(conn, ip, port), ip, port, 0, data, len, 1) == 0) {
            return 0;
        }
    }
    return 1;
}
