#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 words[32];
} User;

typedef struct {
    u16 event, err, reason;
} EventHeader;

typedef struct {
    u16 event, err, reason;
    u8 pad6[2];
    u16 state;
    u8 mac[6];
    u8 pad10[0x26];
    u16 extra;
    User user;
} ScanEvent;

typedef struct {
    u16 event, err, reason;
    u8 pad6[2];
    u16 state, aid;
} ConnectEvent;

typedef struct {
    u16 event, err, reason;
    u8 pad6[2];
    u16 state;
    u8 mac[6];
    u8 pad10[0x26];
    u16 extra;
    User user;
} StoredScan;

typedef struct {
    u8 scanImage[0xc0];
    StoredScan scan;
    u8 tail[8];
} ScanRecord;

typedef void (*Callback)(void *);

typedef struct {
    u8 pad0[0x40];
    u8 sendBuffer[0x400];
    u8 scanImage[0xc0];
    u32 x500;
    void *mpbuf;
    u32 x508;
    u8 sending;
    u8 pad50d[0xb];
    u16 sendSize, recvSize;
    void (*notify)(int, void *);
    void *connect;
    u16 x524, ending, active, port, continuous;
    u8 pad52e[0xb2];
    u16 count;
    u16 aid;
    int scanning, changing, index;
    u8 pad5f0[0x10];
    ScanRecord records[16];
} Child;

typedef char EventSize[(sizeof(ScanEvent) == 0xb8) ? 1 : -1];
typedef char RecordSize[(sizeof(ScanRecord) == 0x180) ? 1 : -1];
typedef char ChildSize[(sizeof(Child) == 0x1e00) ? 1 : -1];

typedef struct {
    u16 dma, x2;
    u32 x4;
    Child *child;
    u8 padc[0x14];
    void *scanbuf;
    u16 channel, time;
    u8 mac[6];
} Global;

extern Global ov13_0224CF40;
extern u16 ov13_0224598C[4];
extern u8 ov13_0224CF60[];
extern int WM_SetLifeTime(Callback, u16, u16, u16, u16);
extern int WM_StartScan(Callback, void *);
extern int WM_StartConnectEx(Callback, void *, void *, u16, u16);
extern int WM_StartMPEx(void *, void *, u32, void *, u16, u16, u16, u32, u32, u32, u32);
extern int WM_End(Callback);
extern int WM_SetPortCallback(u16, void (*)(u16 *), void *);
extern void MBi_CheckWmErrcode(int, int);
extern void MBi_CommEnd(void);
extern void MBi_EndCommon(void *);
extern void MBi_ChildPortCallback(u16 *);
extern int changeScanChannel(void *);
extern int MBi_IsSendEnabled(void);
void MBi_ChildCallback(void *);
#define NOTIFY(code, arg)  c->notify((code), (arg))
#define CHECK(event, call) MBi_CheckWmErrcode(event, call)
#define RESTART()                                                  \
    if (c->scanning) {                                             \
        if (c->changing && !changeScanChannel(ov13_0224CF60))      \
            MBi_CommEnd();                                         \
        CHECK(10, WM_StartScan(MBi_ChildCallback, ov13_0224CF60)); \
    }
#define ERR()             \
    if (e->err) {         \
        NOTIFY(0x100, e); \
        return;           \
    }

enum {
    MB_F0,
    MB_F1
};

enum {
    MB_F,
    MB_T
};

static inline int StartChildMultiplayer_j101(Child *c) {
    u16 frequency = c->continuous ? MB_F0 : MB_F1;
    return WM_StartMPEx(MBi_ChildCallback, c->mpbuf, c->recvSize, c->sendBuffer, c->sendSize, frequency, 0, 0, 0, 1, 1);
}

#pragma push
#pragma optimization_level 2
#pragma opt_propagation off
#pragma opt_common_subs off

static inline void PublishScan_j101(Child *c, ScanEvent *scan) {
    int i;
    int p01, p2, p3, p4, p5;
    ScanRecord *records;
    records = c->records;
    for (i = 0; i < c->count; i++) {
        p5 = 0;
        p01 = 0;
        p4 = 0;
        p3 = 0;
        p2 = 0;
        if (scan->mac[0] == records[i].scan.mac[0] && scan->mac[1] == records[i].scan.mac[1]) {
            p01 = 1;
        }
        if (p01 && scan->mac[2] == records[i].scan.mac[2]) {
            p2 = 1;
        }
        if (p2 && scan->mac[3] == records[i].scan.mac[3]) {
            p3 = 1;
        }
        if (p3 && scan->mac[4] == records[i].scan.mac[4]) {
            p4 = 1;
        }
        if (p4 && scan->mac[5] == records[i].scan.mac[5]) {
            p5 = 1;
        }
        if (p5) {
            records[i].scan.extra = scan->extra;
            records[i].scan.user = scan->user;
            DC_InvalidateRange(c->records[i].scanImage, 0xc0);
            MI_DmaCopy16(ov13_0224CF40.dma, c->scanImage, c->records[i].scanImage, 0xc0);
            c->index = i;
            return;
        }
    }
    if (i < 16) {
        c->count = i + 1;
        MIi_CpuCopy16((u16 *)scan, (u16 *)&records[i].scan, 0xb8);
        DC_InvalidateRange(c->records[i].scanImage, 0xc0);
        MI_DmaCopy16(ov13_0224CF40.dma, c->scanImage, c->records[i].scanImage, 0xc0);
        c->index = i;
    }
}

void MBi_ChildCallback(void *argument) {
    EventHeader *e = argument;
    ScanEvent *scan = argument;
    ConnectEvent *connect = argument;
    Child *c = ov13_0224CF40.child;
    switch (e->event) {
    case 0:
        if (e->err) {
            NOTIFY(0x100, e);
            return;
        }
        NOTIFY(21, e);
        CHECK(29, WM_SetLifeTime(MBi_ChildCallback, ov13_0224598C[3], ov13_0224598C[1], ov13_0224598C[0], ov13_0224598C[2]));
        return;
    case 29:
        ERR();
        ov13_0224CF40.scanbuf = c->scanImage;
        if (!ov13_0224CF40.channel) {
            ov13_0224CF40.channel = 1;
        }
        if (!ov13_0224CF40.time) {
            ov13_0224CF40.time = 200;
        }
        ov13_0224CF40.mac[0] = 255;
        ov13_0224CF40.mac[1] = 255;
        ov13_0224CF40.mac[2] = 255;
        ov13_0224CF40.mac[3] = 255;
        ov13_0224CF40.mac[4] = 255;
        ov13_0224CF40.mac[5] = 255;
        c->scanning = 1;
        c->changing = 1;
        CHECK(10, WM_StartScan(MBi_ChildCallback, ov13_0224CF60));
        return;
    case 10:
        ERR();
        switch (scan->state) {
        case 3:
            return;
        case 5:
            PublishScan_j101(c, scan);
            NOTIFY(4, e);
            RESTART();
            return;
        case 4:
            NOTIFY(5, e);
            RESTART();
            return;
        default:
            NOTIFY(0x100, e);
            return;
        }
    case 11:
        ERR();
        {
            void *cn = c->connect;
            CHECK(12, WM_StartConnectEx(MBi_ChildCallback, cn, 0, 1, 0));
        }
        return;
    case 12:
        if (e->err) {
            c->count = 0;
            NOTIFY(11, e);
            return;
        }
        switch (connect->state) {
        case 6:
            c->port = 0;
            c->active = 1;
            return;
        case 7:
            c->aid = connect->aid;
            NOTIFY(6, e);
            c->port = 1;
            if (WM_SetPortCallback(1, MBi_ChildPortCallback, 0)) {
                return;
            }
            CHECK(14, StartChildMultiplayer_j101(c));
            return;
        case 9:
            NOTIFY(10, e);
            c->port = 0;
            c->active = 0;
            return;
        case 26:
            return;
        default:
            NOTIFY(0x100, e);
            return;
        }
    case 14:
        switch (e->reason) {
        case 10:
            c->active = 1;
            if (MBi_IsSendEnabled()) {
                NOTIFY(25, 0);
            }
            return;
        case 12:
            return;
        case 13:
            return;
        default:
            NOTIFY(0x100, e);
            return;
        }
    case 15:
        c->sending = 0;
        if (e->err == 0) {
            NOTIFY(8, e);
        } else if (e->err == 9) {
            NOTIFY(41, e);
        } else {
            NOTIFY(18, e);
        }
        if (!ov13_0224CF40.child->ending) {
            NOTIFY(25, 0);
        }
        return;
    case 1:
        if (e->err) {
            c->ending = 0;
            NOTIFY(0x100, e);
            return;
        }
        c->port = 0;
        ov13_0224CF40.child->active = 0;
        CHECK(2, WM_End(MBi_ChildCallback));
        return;
    case 2:
        if (e->err) {
            c->ending = 0;
            NOTIFY(0x100, e);
            return;
        }
        MBi_EndCommon(argument);
        return;
    case 21:
        if (MBi_IsSendEnabled()) {
            NOTIFY(25, 0);
        }
        return;
    case 128:
        switch (e->reason) {
        case 22:
            OS_Terminate();
            return;
        case 23:
            return;
        default:
            return;
        }
    default:
        NOTIFY(0x100, e);
        return;
    }
}

#pragma pop
