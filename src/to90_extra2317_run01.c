#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u16 a, error;
    u8 p[4];
    u16 state;
    u8 p2[6];
    u16 id;
} Message;

typedef struct {
    u8 p[0x4c];
    int (*callback)(Message *);
    u8 p2[2];
    u16 flags;
} Work;

typedef struct {
    void (*log)(int, const char *, ...);
    Work *w;
} Global;

extern Global ov13_0224F5C0;
extern char ov13_0224720C[];
extern char ov13_02247238[];
extern char ov13_02247264[];
extern void ov13_0223D724(int);
extern void ov13_0223D6A8(int);
extern int ov13_0223DA64(void);
extern int WM_Disconnect(void *, u16);
void ov13_0223D8F4(Message *);
#pragma force_active on

void ov13_0223D8F4(Message *m) {
    struct {
        Message *message;
        u16 mask;
        u32 aid;
    } frame;

    frame.message = m;
    frame.aid = frame.message->id;
    {
        u32 one = 1;
        frame.mask = one << frame.aid;
    }
    if (frame.message->error) {
        ov13_0223D724(frame.message->error);
        ov13_0223D6A8(9);
        return;
    }
    switch (frame.message->state) {
    case 7:
        if (ov13_0224F5C0.log) {
            ov13_0224F5C0.log(0x8000000, ov13_0224720C, frame.aid);
        }
        if (ov13_0224F5C0.w->callback && ov13_0224F5C0.w->callback(frame.message) == 0) {
            int result = WM_Disconnect(0, frame.message->id);
            if (result == 2) {
                return;
            }
            ov13_0223D724(result);
            ov13_0223D6A8(9);
            return;
        }
        ov13_0224F5C0.w->flags |= frame.mask;
        return;
    case 9:
        if (ov13_0224F5C0.log) {
            ov13_0224F5C0.log(0x8000000, ov13_02247238, frame.aid);
        }
        ov13_0224F5C0.w->flags &= ~frame.mask;
        return;
    case 0:
        if (!ov13_0223DA64()) {
            ov13_0223D6A8(9);
        }
        return;
    case 2:
        return;
    default:
        if (ov13_0224F5C0.log) {
            ov13_0224F5C0.log(0x8000000, ov13_02247264, frame.message->state);
        }
        return;
    }
}
