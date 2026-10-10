#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Node {
    char pad[0x4b2];
    u8 flags, attribute, mode, param;
    u16 gap;
    u32 game;
    struct Node *next;
} Node;

typedef struct Beacon {
    char pad[0x18];
    Node *first, *node;
    char *cursor;
    u8 state, pad25, parameter, chunk, count, serial;
    char pad2a[0x16];
    u32 game;
    u8 flags, attribute, paramcopy, serialcopy;
    u16 checksum;
    u8 chunkcopy, countcopy, length;
} Beacon;

extern Beacon ov13_022474A0;
extern u8 bsendBuff[];
extern int WM_SetGameInfo(void *, void *, u16, u32, u16, u8);
extern u32 MBi_GetGgid(void);
extern u16 MBi_GetTgid(void);
extern void MBi_ClearSendStatus(void);
extern void MBi_InitSendFixedBeacon(void);
extern void MBi_InitSendVolatBeacon(void);
extern void MBi_SendFixedBeacon(u32, u16, u8);
extern void MBi_SendVolatBeacon(u32, u16, u8);
void MB_SendGameInfoBeacon(u32, u16, u8);
int MBi_ReadyBeaconSendStatus(void);

void MB_SendGameInfoBeacon(u32 ggid, u16 tgid, u8 attribute) {
    for (;;) {
        switch (ov13_022474A0.state) {
        case 0:
        case 1:
            if (!MBi_ReadyBeaconSendStatus()) {
                return;
            }
            break;
        case 2:
            MBi_InitSendFixedBeacon();
            break;
        case 3:
            MBi_SendFixedBeacon(ggid, tgid, attribute);
            return;
        case 4:
            MBi_InitSendVolatBeacon();
            break;
        case 5:
            MBi_SendVolatBeacon(ggid, tgid, attribute);
            return;
        case 6:
            break;
        }
    }
}

int MBi_ReadyBeaconSendStatus(void) {
    if (!ov13_022474A0.first) {
        u32 ggid = MBi_GetGgid();
        u16 tgid = MBi_GetTgid();
        WM_SetGameInfo(0, bsendBuff, 112, ggid, tgid, 8);
        return 0;
    }
    {
        Node *next;
        if (ov13_022474A0.node && ov13_022474A0.node->next) {
            next = ov13_022474A0.node->next;
        } else {
            next = *(Node *volatile *)&ov13_022474A0.first;
        }
        ov13_022474A0.node = next;
    }
    MBi_ClearSendStatus();
    ov13_022474A0.parameter = ov13_022474A0.node->mode;
    ov13_022474A0.state = 2;
    return 1;
}
