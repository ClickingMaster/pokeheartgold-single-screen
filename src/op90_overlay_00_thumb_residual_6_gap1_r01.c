#pragma force_active on
#include "global.h"
typedef struct State {
    u8 pad_0000[0xFA4];
    void (*recvLocal)(u32, void *, u16);
    void (*recvOnline)(u32, void *, u16);
    u8 pad_0FAC[0x10A8 - 0xFAC];
    u32 receivePending;
    u8 pad_10AC[0x10DD - 0x10AC];
    u8 packetByte;
} State;
typedef struct Sub {
    u32 pending;
    u8 pad[0x31];
    u8 packetByte;
} Sub;
typedef struct State2 {
    u8 pad_0000[0xFA4];
    void (*recvLocal)(u32, void *, u16);
    void (*recvOnline)(u32, void *, u16);
    u8 pad_0FAC[0x10A8 - 0xFAC];
    Sub sub;
} State2;
typedef struct State3 {
    u8 pad_0000[0xFA4];
    void (*recvLocal)(u32, void *, u16);
    void (*recvOnline)(u32, void *, u16);
    u8 pad_0FAC[0x10A8 - 0xFAC];
    u8 bytes[0x40];
} State3;
typedef struct Header {
    u32 value;
} Header;
extern State *_0221A680;
void ov00_021E644C(u32 value);
BOOL ov00_021E77A4(u32 value, const void *a1, u32 a2);
int ov00_021EE490(void);
void *ov00_021E65D4(int, u32, int);
void ov00_021E6638(int, void *, u32);
void MI_CpuCopy8(const void *src, void *dst, u32 length);
void ov00_021E6484(u16 id, const u8 *data, int length);
// @fn ov00_021E6484
void ov00_021E6484(u16 id, const u8 *data, int length) {
    void *buf;
    u32 header;

    header = data[0] | ((data[1] << 8) | ((data[3] << 24) | (data[2] << 16)));
    ((State2 *)_0221A680)->sub.pending = 1;
    if ((u8)header == 1) {
        ov00_021E644C(header);
        ((State2 *)_0221A680)->sub.packetByte = data[2];
    } else {
        if (ov00_021E77A4(id, data, length) == 0) {
            ov00_021E644C(header);
        }
        return;
    }
    length -= 4;
    buf = ov00_021E65D4(0, length, 4);
    if (buf != NULL) {
        MI_CpuCopy8(data + 4, buf, length);
        if (ov00_021EE490() == 0) {
            if (_0221A680->recvLocal != NULL) {
                _0221A680->recvLocal(id, buf, length);
            }
        } else {
            if (_0221A680->recvOnline != NULL) {
                _0221A680->recvOnline(id, buf, length);
            }
        }
        ov00_021E6638(0, buf, length);
    }
}
