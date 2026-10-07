#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*ConnCallback)(int result, void *param);
typedef void (*ErrCallback)(int err, int code, int extra, void *param);
typedef struct {
    u8 pad0[8];
    int id;
    u8 padc[4];
    int state;
    u8 pad14[0xc];
    int unk20;
    u8 pad24[8];
    u32 ip;
    u16 port;
    u8 pad32[2];
    ConnCallback connCb;
    ErrCallback errCb;
    void *param;
} Conn;
typedef struct {
    u8 pad0[0xc];
    u32 ip;
    u16 port;
    u8 pad12;
    u8 status;
} ConnReply;
void ov00_02210408(Conn *conn, int arg);
void ov00_0221019C(int id);
void ov00_0220FEC4(Conn *conn);
void ov00_022104D4(Conn *conn, ConnReply *reply, int arg);
// @fn ov00_022104D4
void ov00_022104D4(Conn *conn, ConnReply *reply, int arg) {
    int err;
    if (reply->status == 0) {
        ov00_02210408(conn, arg);
    }
    if (conn->state >= 2) {
        return;
    }
    if (reply->status != 0) {
        err = 3;
        if (reply->status == 1) {
            err = 1;
        } else if (reply->status == 2) {
            err = 2;
        }
        conn->errCb(err, -1, 0, conn->param);
        ov00_0221019C(conn->id);
        return;
    }
    conn->ip = reply->ip;
    conn->port = ((reply->port >> 8) & 0xff) | ((reply->port << 8) & 0xff00);
    conn->unk20 = 0;
    conn->state = 2;
    conn->connCb(conn->state, conn->param);
    ov00_0220FEC4(conn);
}
