#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define WMSP_REQUEST_COUNT 46
#define WMSP_REQUEST_DONE  0x8000 // set in the first word of a request once it has been run

#pragma opt_treetransformation off

// Runs the requests of wmspW.pxiQueue through WmspRequestFuncTable until it receives NULL.
void WMSP_RequestThread(void *arg) {
    WmspWork *work = &wmspW;
    WmspStatus *status = work->status;
    u16 *request;
    (void)arg;
    for (;;) {
        __VENEER_OS_ReceiveMessage(&work->pxiQueue, (OSMessage *)&request, OS_MESSAGE_BLOCK);
        if (!request) {
            __VENEER_OS_ExitThread();
            break;
        }
        {
            u16 command = request[0];
            if (command & WMSP_REQUEST_DONE) {
                command &= ~WMSP_REQUEST_DONE;
            }
            if (command < WMSP_REQUEST_COUNT) {
                status->requestBusy = 1;
                status->requestCommand = command;
                WmspRequestFuncTable[command](request);
                status->requestBusy = 0;
            }
            request[0] = command | WMSP_REQUEST_DONE;
        }
    }
}

#pragma opt_treetransformation reset

void WmspRequestFuncDummy(void *request) {
    (void)request;
}
