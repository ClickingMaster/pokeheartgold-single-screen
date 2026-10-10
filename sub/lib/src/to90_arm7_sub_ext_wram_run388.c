#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380FFF4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

typedef struct {
    u8 pad[12];
    u16 command;
    volatile u16 length;
} Packet37024;

typedef struct {
    u8 pad[16];
    u16 command, length, result;
} Confirm37024;

typedef struct {
    Packet37024 *request;
    u16 busy;
} Control37024;

typedef struct {
    u16 req, confirm;
    u16 (*call)(Packet37024 *, void *);
} Entry37024;

extern const Entry37024 WlibCmdTbl_MLME[];
extern const Entry37024 WlibCmdTbl_MA[];
extern const Entry37024 WlibCmdTbl_PARAMSET[];
extern const Entry37024 WlibCmdTbl_PARAMSET2[];
extern const Entry37024 WlibCmdTbl_PARAMGET[];
extern const Entry37024 WlibCmdTbl_PARAMGET2[];
extern const Entry37024 WlibCmdTbl_DEV[];
extern void SendMessageToWmDirect(void *, void *);
extern void AddTask(int, int);
void RequestCmdTask(void);

void RequestCmdTask(void) {
    Confirm37024 *confirm;
    Control37024 *p;
    const Entry37024 *table;
    u16 index;
    u16 error = 0;
    u16 mask = 0;
    u16 command, limit;
    u8 *base = BASE;
    Packet37024 *req;
    p = (Control37024 *)(base + 0x424);
    if (p->busy) {
        return;
    }
    req = *(Packet37024 **)(base + 0x200);
    p->request = req;
    if (req == (Packet37024 *)-1) {
        return;
    }
    base = BASE;
    confirm = (Confirm37024 *)((u8 *)req + req->length * 2);
    if (H(base, 0x33e)) {
        confirm->length = 1;
        confirm->result = 6;
        goto send;
    }
    command = req->command;
    if (command != confirm->command) {
        confirm->result = 13;
        goto send;
    }
    switch (command & 0xff00) {
    case 0:
        table = WlibCmdTbl_MLME;
        index = command & 255;
        mask = 1;
        limit = 11;
        if (p->busy & 1) {
            error = 2;
        } else if (H(base, 0x34c) < 32) {
            error = 1;
        }
        break;
    case 0x100:
        table = WlibCmdTbl_MA;
        index = command & 255;
        mask = 2;
        limit = 5;
        if (H(base, 0x34c) != 64) {
            error = 1;
        }
        break;
    case 0x200:
        index = command & 255;
        if (index < 64) {
            u16 state = *(volatile u16 *)(base + 0x34c);
            mask = 4;
            if (state < 16) {
                error = 1;
            }
            table = WlibCmdTbl_PARAMSET;
            limit = 23;
        } else if (index < 128) {
            u16 state = *(volatile u16 *)(base + 0x34c);
            mask = 8;
            if (state != 64) {
                error = 1;
            }
            index -= 64;
            table = WlibCmdTbl_PARAMSET2;
            limit = 6;
        } else {
            u16 state = *(volatile u16 *)(base + 0x34c);
            if (index < 192) {
                if (state < 16) {
                    error = 1;
                }
                index -= 128;
                table = WlibCmdTbl_PARAMGET;
                mask = 16;
                limit = 23;
            } else {
                if (state < 16) {
                    error = 1;
                }
                index -= 192;
                table = WlibCmdTbl_PARAMGET2;
                mask = 32;
                limit = 6;
            }
        }
        break;
    case 0x300:
        table = WlibCmdTbl_DEV;
        index = command & 255;
        mask = 64;
        limit = 11;
        break;
    default:
        index = 1;
        limit = 0;
        break;
    }
    if (index > limit) {
        error = 3;
    } else if (req->length < table[index].req || confirm->length < table[index].confirm) {
        error = 4;
    }
    if (error) {
        confirm->length = 1;
        confirm->result = error;
        goto send;
    }
    p->busy |= mask;
    confirm->result = table[index].call(req, (u8 *)confirm + 16);
    switch (confirm->result) {
    case 0x80:
        return;
    case 0x81:
        p->busy &= ~mask;
        break;
    default:
        goto send;
    }
    goto next;
send:
    p->busy &= ~mask;
    SendMessageToWmDirect(BASE + 0x200, p->request);
next:
    if (H(BASE, 0x208)) {
        AddTask(2, 11);
    }
}
