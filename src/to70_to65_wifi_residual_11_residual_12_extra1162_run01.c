#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u8 pad[36];
    u32 seq, ack;
    u8 end[56];
} Packet;

extern Packet _021D4764;
extern u16 _021D4720[];
extern u32 _021D4620[];
extern void tcp_send_handshake(Packet *, int, int);
#define swap(a)   ((u16)(((a) >> 8) | ((a) << 8)))
#define read32(p) (((u32)swap((p)[0]) << 16) | swap((p)[1]))
void tcp_send_rst(u16 *, u16 *, int, int);

void tcp_send_rst(u16 *ip, u16 *tcp, int length, int arg) {
    Packet *p = &_021D4764;
    MI_CpuFill8(p, 0, 100);
    _021D4720[0x27] = swap(tcp[1]);
    _021D4720[0x2e] = swap(tcp[0]);
    _021D4620[0x58] = read32(ip + 6);
    if (((u8 *)tcp)[13] & 16) {
        p->ack = read32(tcp + 4);
        tcp_send_handshake(p, 4, arg);
        return;
    }
    p->ack = 0;
    p->seq = length + read32(tcp + 2);
    if (((u8 *)tcp)[13] & 3) {
        p->seq++;
    }
    tcp_send_handshake(p, 20, arg);
}
