#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[4];
    u32 *flags;
    u8 entries[12];
    u8 index, state;
} Work;

extern Work *ov13_0224F544;
extern int WifiSetupKeypad_GetInput(void);
extern int ov13_0223261C(void);
extern int ov13_02232168(int);
extern void WifiSetupSound_Play(int);
extern void WifiSetupKeypad_SetCanDelete(int);
extern void WifiSetupKeypad_SetCanType(int);
extern void WifiSetupKeypad_SetCanAdvance(int);
extern void WifiSetupApp_SetStateFunc(void *);
extern void ov13_02232200(void);
extern void ov13_02232440(void);
extern void ov13_022325A4(void);
extern void ov13_0223237C(void);
void ov13_02231E58(void);

void ov13_02231E58(void) {
    int key = WifiSetupKeypad_GetInput();
    switch (key) {
    case 0:
        return;
    case 16:
        if (ov13_0224F544->index != 0 || ov13_0224F544->entries[2] != 0) {
            WifiSetupSound_Play(3);
            if (ov13_0224F544->entries[ov13_0224F544->index * 3 + 2] == 0) {
                ov13_0224F544->index--;
            }
            MI_CpuFill8(ov13_0224F544->entries + ov13_0224F544->index * 3, 0, 3);
            if (ov13_0224F544->index == 0 && ov13_0224F544->entries[2] == 0) {
                WifiSetupKeypad_SetCanDelete(0);
            }
            WifiSetupKeypad_SetCanType(1);
            WifiSetupKeypad_SetCanAdvance(0);
        }
        break;
    case 17:
        if (ov13_0224F544->index < 3 && ov13_0224F544->entries[ov13_0224F544->index * 3 + 2] != 0) {
            WifiSetupSound_Play(1);
            ov13_0224F544->index++;
            WifiSetupKeypad_SetCanAdvance(0);
        }
        break;
    case 18:
        ov13_0224F544->state = 0;
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(ov13_02232200);
        return;
    case 19:
        if (ov13_0223261C()) {
            WifiSetupSound_Play(6);
            ov13_0224F544->state = 1;
        } else {
            ov13_0224F544->state = 2;
            WifiSetupSound_Play(9);
        }
        ov13_0224F544->index = 3;
        *ov13_0224F544->flags = (*ov13_0224F544->flags & 0xc1fffcff) | 0x200;
        ov13_02232440();
        ov13_022325A4();
        WifiSetupApp_SetStateFunc(ov13_02232200);
        return;
    default:
        if (ov13_0224F544->index == 3 && ov13_02232168(26)) {
            break;
        }
        WifiSetupSound_Play(1);
        {
            u8 *entry = ov13_0224F544->entries;
            int index = ov13_0224F544->index * 3;
            if (entry[index + 2] == 0) {
                entry[index + 2] = key;
            } else if (entry[index + 1] == 0) {
                entry[index + 1] = entry[index + 2];
                entry[index + 2] = key;
                if (ov13_02232168(26) && ov13_0224F544->index < 3) {
                    ov13_0224F544->index++;
                }
            } else {
                entry[index] = entry[index + 1];
                entry[index + 1] = entry[index + 2];
                entry[index + 2] = key;
                if (ov13_0224F544->index < 3) {
                    ov13_0224F544->index++;
                }
            }
        }
        WifiSetupKeypad_SetCanDelete(1);
        if (ov13_0224F544->index < 3) {
            WifiSetupKeypad_SetCanAdvance(1);
        } else {
            WifiSetupKeypad_SetCanAdvance(0);
        }
        if (ov13_0224F544->index == 3 && ov13_02232168(26)) {
            WifiSetupKeypad_SetCanType(0);
        }
        break;
    }
    ov13_0223237C();
    ov13_02232440();
}
