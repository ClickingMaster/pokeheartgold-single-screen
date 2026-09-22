#include "to57_overlay87_match_logic_private.h"
typedef struct Bits {
    u8 bit0 : 1;
    u8 rest : 7;
} Bits;
typedef struct StateView {
    u8 pad[0x360];
    u8 cells[0x40];
    Bits flags;
    u8 count;
    u8 selected[3];
    u8 flags2[0x3a5 - 0x3a5];
} StateView;
int ov87_021E7734(State *p) {
    StateView *q = (StateView *)p;
    int flag = 0;
    u8 a, b;
    if (q->count <= 1) {
        return 0;
    }
    a = q->cells[q->selected[1]];
    b = q->cells[q->selected[0]];
    if (b == a) {
        q->flags.bit0 = 1;
    } else if (b == 4) {
        q->flags.bit0 = 1;
    } else if (a == 4) {
        q->flags.bit0 = 1;
    }
    if (q->count <= 2) {
        return 0;
    }
    b = q->cells[q->selected[1]];
    a = q->cells[q->selected[0]];
    if (a == b && a == q->cells[q->selected[2]]) {
        flag = 1;
    }
    if (a == 4 && b == 4) {
        flag = 1;
    }
    if (a == 4 && q->cells[q->selected[2]] == 4) {
        flag = 1;
    }
    if (b == 4 && q->cells[q->selected[2]] == 4) {
        flag = 1;
    }
    if (a == 4 && b == q->cells[q->selected[2]]) {
        flag = 1;
    }
    if (b == 4 && a == q->cells[q->selected[2]]) {
        flag = 1;
    }
    if (q->cells[q->selected[2]] == 4 && a == b) {
        flag = 1;
    }
    if (flag == 1) {
        int i;
        for (i = 0; i < 3; i++) {
            b = q->cells[q->selected[i]];
            if (b != 4) {
                U8(p, 0x39F) = b;
                break;
            }
        }
        return 1;
    }
    return 0;
}
