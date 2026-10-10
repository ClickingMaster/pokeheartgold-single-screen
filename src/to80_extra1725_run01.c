#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 words[78];
} Snapshot;

typedef struct {
    Snapshot snapshot;
    u32 pad;
    int state;
    u8 q[0x250 - 0x140];
    int error;
    u32 length;
    void *buffer;
    int token;
    u32 kind;
    u8 r[0x268 - 0x264];
    int stop;
} Global;

extern Global _0222AB84;
extern int ov39_02229FCC(void);
extern int ov39_02229AD4(void);
extern int ov39_02229BE4(void);
extern int ov39_02229D2C(void);
extern int ov39_02229E40(void);
extern void ov39_022298FC(void);
extern void ov39_02229910(void);
extern void ov39_022298E0(void *);
extern void ov39_02229FC0(int);
extern int ov39_0222A6DC(int);
extern int ov39_0222A59C(int);
extern int ov39_02229980(void);
extern void *ov39_022298CC(int);
extern void ov39_02229408(void);
extern void ov39_0222A4EC(void *, u32, void *);
extern void *memcpy(void *, const void *, u32);
int ov39_02229630(int, void *, u32, int);
void ov39_02229474(void);

static inline int Next2(BOOL *done) {
    if (!ov39_02229FCC()) {
        *done = 1;
        return _0222AB84.error == 13 ? 7 : 9;
    }
    return 3;
}

typedef struct {
    u8 bytes[4];
} Header;

static inline void copyHeader(void *d, const void *s) {
    *(Header *)d = *(const Header *)s;
}

int ov39_02229630(int id, void *data, u32 type, int requested) {
    u32 length;
    ov39_022298FC();
    switch (_0222AB84.state) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        ov39_02229910();
        _0222AB84.state = 9;
        _0222AB84.error = 1;
        return 0;
    case 7:
    case 8:
    case 9:
        ov39_02229408();
        break;
    }
    ov39_02229910();
    _0222AB84.token = ov39_0222A6DC(id);
    if (_0222AB84.token == -1) {
        _0222AB84.state = 9;
        _0222AB84.error = 2;
        return 0;
    }
    _0222AB84.kind = type;
    if (requested != -1) {
        length = requested + 0x140;
    } else {
        length = ov39_0222A59C(id);
        if (length == (u32)-1) {
            _0222AB84.state = 9;
            _0222AB84.error = 2;
            return 0;
        }
    }
    _0222AB84.buffer = ov39_022298CC(length);
    if (!_0222AB84.buffer) {
        return 0;
    }
    _0222AB84.length = length;
    { memcpy(_0222AB84.buffer, &length, 4); }
    {
        u8 *buffer = _0222AB84.buffer;
        *(u16 *)(buffer + 4) = id;
        *(Snapshot *)(buffer + 8) = _0222AB84.snapshot;
        *(u16 *)(buffer + 6) = 0;
        memcpy(buffer + 0x140, data, length - 0x140);
    }
    ov39_0222A4EC((u8 *)_0222AB84.buffer + 4, _0222AB84.length - 4, (u8 *)_0222AB84.buffer + 4);
    _0222AB84.state = 2;
    ov39_022298FC();
    if (!ov39_02229980()) {
        _0222AB84.state = 9;
        _0222AB84.error = 6;
        ov39_02229910();
        return 0;
    }
    ov39_02229910();
    return 1;
}
