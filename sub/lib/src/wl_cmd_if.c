#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern const WlCmdEntry WlibCmdTbl_MLME[];
extern const WlCmdEntry WlibCmdTbl_MA[];
extern const WlCmdEntry WlibCmdTbl_PARAMSET[];
extern const WlCmdEntry WlibCmdTbl_PARAMSET2[];
extern const WlCmdEntry WlibCmdTbl_PARAMGET[];
extern const WlCmdEntry WlibCmdTbl_PARAMGET2[];
extern const WlCmdEntry WlibCmdTbl_DEV[];

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Task 11: executes the next request from the WMSP through the command table of its group. The handler
// returns the result (0x80: it completes later and sends the confirm itself, 0x81: no confirm now);
// otherwise the confirm goes back to the WMSP at once.
void RequestCmdTask(void) {
    WlCmdConfirm *confirm;
    WlCmdIf *cmdIf;
    const WlCmdEntry *table;
    u16 index;
    u16 error = 0;
    u16 mask = 0;
    u16 command, limit;
    WlWork *w = wlWorkPlain;
    WlCmdRequest *req;
    cmdIf = &w->cmdIf;
    if (cmdIf->busy) {
        return;
    }
    req = (WlCmdRequest *)w->heap.bufMan[10].first;
    cmdIf->request = req;
    if (req == (WlCmdRequest *)-1) {
        return;
    }
    w = wlWorkPlain;
    confirm = (WlCmdConfirm *)((u8 *)req + req->length * 2);
    if (w->param.errors) {
        confirm->cfm.length = 1;
        confirm->cfm.result = 6;
        goto send;
    }
    command = req->command;
    if (command != confirm->cfm.command) {
        confirm->cfm.result = 13;
        goto send;
    }
    switch (command & 0xFF00) {
    case 0:
        table = WlibCmdTbl_MLME;
        index = command & 255;
        mask = 1;
        limit = 11;
        if (cmdIf->busy & 1) {
            error = 2;
        } else if (w->bss.state < 32) {
            error = 1;
        }
        break;
    case 0x100:
        table = WlibCmdTbl_MA;
        index = command & 255;
        mask = 2;
        limit = 5;
        if (w->bss.state != 64) {
            error = 1;
        }
        break;
    case 0x200:
        index = command & 255;
        if (index < 64) {
            u16 state = *(vu16 *)&w->bss.state;
            mask = 4;
            if (state < 16) {
                error = 1;
            }
            table = WlibCmdTbl_PARAMSET;
            limit = 23;
        } else if (index < 128) {
            u16 state = *(vu16 *)&w->bss.state;
            mask = 8;
            if (state != 64) {
                error = 1;
            }
            index -= 64;
            table = WlibCmdTbl_PARAMSET2;
            limit = 6;
        } else {
            u16 state = *(vu16 *)&w->bss.state;
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
    } else if (req->length < table[index].reqLength || confirm->cfm.length < table[index].cfmLength) {
        error = 4;
    }
    if (error) {
        confirm->cfm.length = 1;
        confirm->cfm.result = error;
        goto send;
    }
    cmdIf->busy |= mask;
    confirm->cfm.result = table[index].func(req, &confirm->cfm);
    switch (confirm->cfm.result) {
    case 0x80:
        return;
    case 0x81:
        cmdIf->busy &= ~mask;
        break;
    default:
        goto send;
    }
    goto next;
send:
    cmdIf->busy &= ~mask;
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[10], cmdIf->request);
next:
    if (wlWorkPlain->heap.bufMan[10].count) {
        AddTask(2, 11);
    }
}

// Handler of the unused command numbers.
int CMD_ReservedReqCmd(WlCmdRequest *req, WlCmdCfm *cfm) {
    (void)req;
    (void)cfm;
    return 3;
}

// Sends a message to the WMSP; when the queue is full (or older messages wait) it waits in
// WlHeap.bufMan[9] for SendMessageToWmTask (task 19).
void SendMessageToWmDirect(WlHeapBufMan *man, void *msg) {
    WlWork *w = wlWorkPlain;
    if (w->heap.bufMan[9].count) {
        MoveHeapBuf(man, &w->heap.bufMan[9], msg);
        AddTask(2, 0x13);
    } else if (__VENEER_OS_SendMessage(w->indicateQueue, msg, 0)) {
        DeleteHeapBuf(man, msg);
    } else {
        MoveHeapBuf(man, &wlWorkPlain->heap.bufMan[9], msg);
    }
}

void SendMessageToWmTask(void) {
    WlWork *w = wlWorkPlain;
    WlHeapBuf *msg = w->heap.bufMan[9].first;
    while (msg != (WlHeapBuf *)-1) {
        if (!__VENEER_OS_SendMessage(w->indicateQueue, msg, 0)) {
            break;
        }
        DeleteHeapBuf(&wlWorkPlain->heap.bufMan[9], msg);
        w = wlWorkPlain;
        msg = w->heap.bufMan[9].first;
    }
}

void InitializeCmdIf(void) {
    wlWorkPlain->cmdIf.busy = 0;
}

#pragma dont_inline reset
