#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void (*gCpsFree)(void *);
extern int make_plaintext(u8 *, u8 *);
extern void client_hello(u8 *, u8 *);
extern void rcv_client_key_exchange(u8 *, u8 *);
extern void rcv_server_hello(u8 *, u8 *);
extern void rcv_certificate(u8 *, u8 *);
extern void rcv_finished(u8 *, u8 *);
extern void update_digest(u8 *, u8 *, int);
void parse_record_in_buf(u8 *, u8 *);

void parse_record_in_buf(u8 *w, u8 *buf) {
    int state = B(w, 0x455), type;
    u8 *p;
    int length;
    if (state == 9) {
        gCpsFree(buf);
        return;
    }
    length = (buf[3] << 8) + buf[4] + 5;
    type = buf[0];
    if (((u8)(state + 249) <= 1 && type != 21) || (type == 21 && (u32)length > 7)) {
        length = make_plaintext(w, buf);
        state = B(w, 0x455);
        if (state == 9) {
            gCpsFree(buf);
            return;
        }
    }
    p = buf + 5;
    length -= 5;
    switch (type) {
    case 20:
        if (!W(w, 0x1d4)) {
            B(w, 0x455) = 9;
        } else {
            MI_CpuFill8(w + 0x2e4, 0, 8);
            B(w, 0x455) = 7;
        }
        break;
    case 21:
        if (*p == 2) {
            B(w, 0x455) = 9;
        }
        break;
    case 22:
        do {
            int message = p[0];
            u32 size = (p[1] << 16) + (p[2] << 8) + p[3];
            p += 4;
            switch (message) {
            case 1:
                if (B(w, 0x454) && state == 0) {
                    client_hello(w, p);
                }
                break;
            case 16:
                rcv_client_key_exchange(w, p);
                break;
            case 2:
                rcv_server_hello(w, p);
                break;
            case 11:
                rcv_certificate(w, p);
                B(w, 0x5ac) = 0;
                break;
            case 13:
                B(w, 0x5ac) = 1;
                break;
            case 14:
                B(w, 0x455) = 4;
                break;
            case 20:
                rcv_finished(w, p);
                break;
            default:
                B(w, 0x455) = 9;
                break;
            }
            update_digest(w, p - 4, size + 4);
            p += size;
            length -= size + 4;
            if (!length) {
                break;
            }
            state = B(w, 0x455);
        } while (state != 9);
        break;
    case 23:
        P(w, 0x824) = buf;
        W(w, 0x82c) = 5;
        W(w, 0x828) = length + 5;
        B(w, 0x456) = 1;
        return;
    default:
        B(w, 0x455) = 9;
        break;
    }
    gCpsFree(buf);
}
