#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Ov13Pos2 {
    u16 x, y;
} Ov13Pos2;

typedef struct Pos16 {
    u16 x, y;
} Pos16;

typedef struct Rect16 {
    u16 x1, y1, x2, y2;
} Rect16;

typedef struct Ov13Select {
    void *obj0, *obj1, *objs[2], *unk10, *task;
    u16 unk18;
    s8 unk1A, unk1B;
    u8 kind, unk1D, unk1E;
} Work;

typedef char WorkSize[(sizeof(Work) == 32) ? 1 : -1];
typedef char SelectedOffset[(offsetof(Work, unk1B) == 27) ? 1 : -1];
extern Work *ov13_0224F5B8;
extern u8 ov13_02245934[];
extern u8 ov13_0224588A[][2];
extern Ov13Pos2 ov13_02245904[5][2];
extern Ov13Pos2 ov13_022458A8[5];
extern s8 ov13_02245894[][2];
extern s8 ov13_02245895[][2];
extern void ov13_02240E74(Pos16 *, Pos16 *, Rect16 *);
extern void ov13_0223C73C(int);
extern BOOL ov13_022417DC(Rect16 *);
extern BOOL ov13_02241694(u16);
void ov13_0223C340(void *);
#pragma force_active on

void ov13_0223C340(void *task) {
    Rect16 temp;
    int i;
    register Work *w;
    int j;
    u8 *counts;
    for (i = 0; i < ov13_02245934[ov13_0224F5B8->kind]; i++) {
        ov13_02240E74((Pos16 *)&ov13_02245904[ov13_0224F5B8->kind][ov13_0224588A[ov13_0224F5B8->kind][i]], (Pos16 *)&ov13_022458A8[ov13_0224F5B8->kind], &temp);
        if (ov13_022417DC(&temp)) {
            ov13_0224F5B8->unk1B = i;
            break;
        }
    }
    if (ov13_02241694(1)) {
        ov13_0224F5B8->unk1B = ov13_02245894[ov13_0224F5B8->kind][0];
    }
    if (ov13_02241694(2)) {
        ov13_0224F5B8->unk1B = ov13_02245895[ov13_0224F5B8->kind][0];
    }
    counts = ov13_02245934;
    w = ov13_0224F5B8;
    for (j = 0; j < counts[((volatile Work *)w)->kind]; j++) {
        if (j == w->unk1B) {
            ov13_0223C73C(j);
            return;
        }
    }
    w->unk1B = -1;
}
