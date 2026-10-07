#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 data[22]; } ChildPiece;
typedef struct { u8 data[0x5c4]; u16 f5c4, f5c6, f5c8, f5ca, f5cc, f5ce, f5d0, f5d2; } Game;
typedef struct {
    u8 prefix[0x1340];
    ChildPiece piece[15];   /* 0x1340 */
    u16 state[15];          /* 0x148a */
    u32 four[15];           /* 0x14a8 */
    u32 callback;           /* 0x14e4 */
    u32 request[15];        /* 0x14e8 */
    u8 unknown[2];          /* 0x1524 */
    s8 game[15];            /* 0x1526 */
    u8 connectedCount;      /* 0x1535 */
    u16 connected;          /* 0x1536 */
    u8 buffer[0x21c];       /* 0x1538 */
    u16 status[15];         /* 0x1754 */
    u8 pad[22];
    Game games[16];         /* 0x1788 */
} Parent;
typedef struct { u32 pad[3]; Parent *parent; } MbCtx;
extern MbCtx ov13_0224CF40;
extern void MBi_ParentCallback(void *);
extern int WM_Disconnect(void (*)(void *), u16);
extern void MBi_ClearParentPieceBuffer(u16);
void MB_DisconnectChild(u16 aid);
#define P (ov13_0224CF40.parent)
typedef struct {
    u8 pad0[0x1d4c];
    u16 f4c;
    u16 f4e;
    u16 f50;
} GameSlot;
#define GS(i) ((GameSlot *)((u8 *)P + (i) * 0x5d4))
// @fn MB_DisconnectChild
void MB_DisconnectChild(u16 aid) {
    WM_Disconnect(MBi_ParentCallback, aid);
    if (aid == 0 || aid >= 16) {
        return;
    }
        P->state[aid - 1] = 0;
    MI_CpuFill8(&P->four[aid - 1], 0, 4);
    MI_CpuFill8(&P->piece[aid - 1], 0, sizeof(ChildPiece));
    MBi_ClearParentPieceBuffer(aid);
    P->status[aid - 1] = 0;
    if (P->game[aid - 1] != -1) {
        u8 slot = P->game[aid - 1];
        GS(slot)->f4e &= ~(1 << aid);
        GS(slot)->f50 |= 1 << aid;
        P->game[aid - 1] = -1;
        GS(slot)->f4c &= ~(1 << aid);
    }
    if (P->connected & (1 << aid)) {
        P->connectedCount--;
        P->connected &= ~(1 << aid);
    }
    P->request[aid - 1] = 0;
}
