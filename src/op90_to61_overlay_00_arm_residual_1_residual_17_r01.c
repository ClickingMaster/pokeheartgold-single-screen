#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct GTConn GTConn;
typedef void (*ConnCallback)(int error, BOOL disconnected, BOOL special, u32 aid, int value, void *param);
typedef struct {
    u8 pad0[0x24];
    int phase;
    u8 pad28[5];
    u8 connected;
    u8 pad2e[0x90 - 0x2e];
    ConnCallback callback;
    void *param;
    u8 pad98[0x381 - 0x98];
    u8 count;
    u8 pad382[2];
    u32 timestamp;
    volatile u8 remaining;
    volatile u8 mode;
    u8 active;
    u8 pad38b[0x468 - 0x38b];
    u32 list[(0x644 - 0x468) / 4];
    u8 aids[32];
    u32 mask;
} DwcWork;
typedef struct {
    u8 index;
    u8 aid;
} ConnInfo;
extern DwcWork *ov00_0221A784;
extern u32 ov00_0221A78C[];
extern const u8 ov00_022179F4[];
extern int ov00_021F3804(void);
extern ConnInfo *ov00_0220D2E0(GTConn *conn);
extern void ov00_021F9C20(u32 aid);
extern int ov00_021EE820(u32 aid);
extern void ov00_021F36BC(void);
extern void ov00_021F3310(int profile);
extern int ov00_021F3194(int error, int code, int profile);
extern void ov00_021EC24C(int error, int code);
extern void ov00_021F3398(int a, int b);
extern void ov00_021F04A0(int a, const void *b, int c);
extern void ov00_02210CBC(u32 t);
extern int ov00_021F0404(int profile);
extern void ov00_021F125C(void);
extern void ov00_021F2FA0(void);
extern void ov00_021EE800(int a);
void ov00_021EEE40(GTConn *conn, int event);
// @fn ov00_021EEE40
void ov00_021EEE40(GTConn *conn, int event) {
    u32 aid;
    int profile = 0;
    int code;
    ConnInfo *info;
    BOOL buddy;
    BOOL special = FALSE;
    int error;
    if (ov00_021F3804()) {
        return;
    }
    switch (event) {
    case 0:
    case 1:
        error = 0;
        break;
    case 2:
    case 3:
        error = 6;
        code = -7600;
        break;
    case 4:
        code = -7601;
        error = 9;
        break;
    }
    if (error == 0) {
        info = ov00_0220D2E0(conn);
        if (info == NULL) {
            return;
        }
        aid = info->aid;
        buddy = TRUE;
        if (!(ov00_0221A784->mask & (1 << aid))) {
            buddy = FALSE;
        }
        ov00_021F9C20(aid);
        if ((ov00_0221A784->mode == 2 && event == 0) || (ov00_0221A784->mode == 3 && aid == 0)) {
            special = TRUE;
        }
        profile = ov00_021EE820(aid);
        ov00_0221A78C[info->index] = 0;
        if (ov00_0221A784->count) {
            ov00_0221A784->count--;
        }
        if (ov00_0221A784->remaining) {
            ov00_0221A784->remaining--;
        }
    }
    if (ov00_0221A784->connected == 0 && ov00_0221A784->phase == 6 && buddy == FALSE) {
        if (ov00_0221A784->mode == 2 && error == 0) {
            ov00_021F36BC();
            ov00_021F3310(profile);
        }
        return;
    }
    if (ov00_021F3194(error, code, profile)) {
        return;
    }
    if (error) {
        ov00_021EC24C(error, code - 80000);
        return;
    }
    if (!ov00_0221A784->connected && (ov00_0221A784->mode == 2 || ov00_0221A784->mode == 3)) {
        int n = ov00_0221A784->count;
        if (ov00_0221A784->list[n + 2] != 0) {
            ov00_0221A784->aids[n + 1] = ov00_0221A784->aids[n + 2];
            ov00_021F3398(ov00_0221A784->count + 1, ov00_0221A784->count + 3);
        }
    }
    if (ov00_0221A784->mode == 2) {
        if (!ov00_0221A784->connected) {
            ov00_021F36BC();
        } else if (!ov00_0221A784->count) {
            ov00_021F04A0(1, ov00_022179F4, 0);
        }
    } else if (!ov00_0221A784->count) {
        ov00_021F04A0(1, ov00_022179F4, 0);
    }
    if (ov00_0221A784->mode == 0 || ov00_0221A784->mode == 1) {
        ov00_0221A784->active = ov00_0221A784->remaining;
        ov00_02210CBC(ov00_0221A784->timestamp);
    }
    {
        DwcWork *w = ov00_0221A784;
        if (w->callback && buddy) {
            ov00_0221A784->callback(error, event == 0, special, aid, ov00_021F0404(profile), w->param);
        }
    }
    if (!ov00_0221A784->connected && ov00_0221A784->mode == 2) {
        return;
    }
    if (ov00_0221A784->count) {
        return;
    }
    ov00_021F125C();
    ov00_021F2FA0();
    ov00_021EE800(3);
}
