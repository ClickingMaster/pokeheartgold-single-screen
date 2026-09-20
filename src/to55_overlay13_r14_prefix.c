#include "to55_overlay13_r14_private.h"

int ov13_02222C1C(Ov13AllocCallback alloc, Ov13FreeCallback free) {
    OSMessage message;
    BOOL running = TRUE;

    OS_InitMessageQueue(&ov13_0224DDA0, ov13_0224DD90, 4);
    if (alloc == NULL || free == NULL) {
        return -1;
    }

    OSIntrMode enabled = OS_DisableInterrupts();
    ov13_0224DD80.alloc = alloc;
    ov13_0224DD80.free = free;
    OS_RestoreInterrupts(enabled);

    ov13_0224DD80.allocation = ov13_0224DD80.alloc(0x5890);
    if (ov13_0224DD80.allocation == NULL) {
        return -1;
    }
    if (!ov13_022236B8(ov13_02222BF4, ov13_0224DD80.allocation, 0x5890)) {
        running = FALSE;
    }

    while (running) {
        OS_ReceiveMessage(&ov13_0224DDA0, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case 6:
            return 0;
        case 4:
        case 5:
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            running = FALSE;
            break;
        }
    }

    ov13_0224DD80.free(ov13_0224DD80.allocation);
    return -1;
}

int ov13_02222CE0(void) {
    OSMessage message;
    BOOL running = TRUE;
    int result = -1;

    if (ov13_0224DD80.free == NULL) {
        return -1;
    }
    if (!ov13_022235DC()) {
        return -1;
    }

    while (running) {
        OS_ReceiveMessage(&ov13_0224DDA0, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case 4:
        case 5:
            break;
        case 20:
            running = FALSE;
            result = 0;
            ov13_0224DD80.free(ov13_0224DD80.allocation);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        default:
            running = FALSE;
            break;
        }
    }

    OSIntrMode enabled = OS_DisableInterrupts();
    ov13_0224DD80.alloc = NULL;
    ov13_0224DD80.free = NULL;
    OS_RestoreInterrupts(enabled);
    return result;
}

int ov13_02222D78(void) {
    OSMessage message;
    int result = -1;
    BOOL running;

    if (ov13_022235A4()) {
        do {
            OS_ReceiveMessage(&ov13_0224DDA0, &message, OS_MESSAGE_BLOCK);
            if ((u32)message == 14) {
                running = FALSE;
                result = running;
            } else {
                running = FALSE;
            }
        } while (running);
    }
    return result;
}
