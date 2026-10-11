#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 bytes[0x834];
} State;

typedef struct {
    u8 p[12];
    State *state;
} Socket;

typedef struct {
    u8 init;
    u8 p[3];
    u32 count;
} RandomState;

typedef struct {
    u64 state, mul, add;
} Generator;

extern RandomState _021D5E60;
extern Generator _021D4688;
extern u8 _021D5FD8[20];
extern u8 _021D5FEB[];
extern u8 *(*gCpsAlloc)(u32);
extern void (*gCpsFree)(void *);
extern u8 *CPSi_TcpReadRaw(u32 *, Socket *);
extern void CPSi_SocConsumeRaw(u32, Socket *);
extern void client_hello_v2(State *, void *);
extern void update_digest(State *, void *, u32);
extern void parse_record_in_buf(State *, u8 *);
extern void CPS_SslAddRandomSeed(void *, int);
extern void CPSi_sha1_init(void *);
extern void CPSi_sha1_calc(void *, void *, int);
extern void CPSi_sha1_result_prng(void *, void *);
extern int tcp_read_raw_nbytes(void *, u32, Socket *);
int parse_record(Socket *);
void set_random(void *, int);

int parse_record(Socket *sock) {
    State *s = sock->state;
    u32 size;
    u8 *raw, *buf;
    for (;;) {
        raw = CPSi_TcpReadRaw(&size, sock);
        if (!size) {
            s->bytes[0x455] = 9;
            return 9;
        }
        if (size >= 5) {
            break;
        }
    }
    if (raw[0] == 128) {
        if (s->bytes[0x454] && !s->bytes[0x455]) {
            size = raw[1];
            CPSi_SocConsumeRaw(2, sock);
            buf = gCpsAlloc(size);
            if (!buf) {
                s->bytes[0x455] = 9;
                return 9;
            }
            if (tcp_read_raw_nbytes(buf, size, sock) == 0 && buf[0] == 1) {
                client_hello_v2(s, buf + 1);
            } else {
                s->bytes[0x455] = 9;
            }
            update_digest(s, buf, size);
            gCpsFree(buf);
        } else {
            s->bytes[0x455] = 9;
        }
    } else {
        size = (raw[3] << 8) + raw[4] + 5;
        if (size > 0x4805) {
            s->bytes[0x455] = 9;
            return 9;
        }
        buf = gCpsAlloc(size);
        if (!buf) {
            s->bytes[0x455] = 9;
            return 9;
        }
        if (tcp_read_raw_nbytes(buf, size, sock)) {
            gCpsFree(buf);
            s->bytes[0x455] = 9;
            return 9;
        }
        parse_record_in_buf(s, buf);
    }
    return s->bytes[0x455];
}
