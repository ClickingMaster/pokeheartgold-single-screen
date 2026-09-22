#include "to55_overlay13_r14_private.h"

typedef struct Ov13DB0Local {
    BOOL running;
    int result;
    OSMessage message;
    OSAlarm alarm;
} Ov13DB0Local;

int ov13_02222DB0(void **output) {
    Ov13DB0Local local;
    local.result = -1;
    local.running = TRUE;
    u8 *buffer;
    int state = 0;
    int records = 0;
    int count = 0;
    u8 *base;

    if (ov13_0224DD80.alloc == NULL || ov13_0224DD80.free == NULL) {
        return -1;
    }

    buffer = ov13_0224DD80.alloc(0x3000);
    if (buffer == NULL) {
        return -1;
    }
    base = buffer;
    if (ov13_02223478(0, 0, 0, 0x0030BFFE) != 0) {
        goto started;
    }
    goto cleanup;

started:

    OS_CreateAlarm(&local.alarm);
    OS_SetAlarm(&local.alarm, 0x003FEC42, (OSAlarmHandler)ov13_02222C08, (void *)0x13);
    do {
        OS_ReceiveMessage(&ov13_0224DDA0, &local.message, OS_MESSAGE_BLOCK);
        switch ((u32)local.message) {
        case 19:
            if (state == 0) {
                if (count != 0) {
                    records = ov13_02223434(buffer, 0x40);
                }
                if (ov13_02223568()) {
                    state = 1;
                } else {
                    goto finish;
                }
            }
            break;
        case 5:
            if (state == 0) {
                if (count < 8) {
                    count++;
                } else {
                    records = ov13_02223434(buffer, 0x40);
                    if (ov13_02223568()) {
                        state = 1;
                    } else {
                        goto finish;
                    }
                }
            }
            break;
        case 10:
            local.running = FALSE;
            local.result = 0;
            break;
        case 4:
        case 8:
        case 18:
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 9:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        default:
            goto finish;
        }
    } while (local.running);

    void *allocation;
    if (records != 0) {
        allocation = ov13_0224DD80.alloc((records - 1) * 0x54 + 0x58);
        if (allocation != NULL) {
            goto allocated;
        }
        goto finish;
    } else {
        allocation = ov13_0224DD80.alloc(0x58);
        if (allocation == NULL) {
            goto finish;
        }
    }
allocated: {
    u8 *dst;
    int i = 0;

    *output = allocation;
    *(u32 *)allocation = records;
    if (records > 0) {
        dst = (u8 *)allocation + 4;
        do {
            ov13_02222B34(buffer, dst);
            i++;
            buffer += 0xc0;
            dst += 0x54;
        } while (i < records);
    }
}

finish:
    OS_CancelAlarm(&local.alarm);
    do {
    } while (OS_ReceiveMessage(&ov13_0224DDA0, &local.message, OS_MESSAGE_NOBLOCK) == TRUE);

cleanup:
    ov13_0224DD80.free(base);
    return local.result;
}
