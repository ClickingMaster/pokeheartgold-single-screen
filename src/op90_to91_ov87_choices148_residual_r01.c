#pragma force_active on
#include "global.h"
typedef struct State {
    u8 pad0[0x32C];
    void *sprites[29];
    u8 flag : 1;
    u8 counter : 7;
} State;
extern const s8 ov87_021E838C[];
extern const s8 ov87_021E827C[][8];
void ov87_021E8014(void *, int *, int *);
void ov87_021E7FEC(void *, int, int);
int ov87_021E73E0(State *, int);
// @fn ov87_021E73E0
int ov87_021E73E0(State *s, int i) {
    int x, y, nx, ny;
    int n;
    s8 dy;
    if (s->counter >= 8) {
        return 1;
    }
    ov87_021E8014(s->sprites[i], &x, &y);
    n = s->counter;
    dy = ov87_021E838C[n];
    nx = x + ov87_021E827C[i][n];
    ny = y + dy;
    s->counter = n + 1;
    ov87_021E7FEC(s->sprites[i], nx, ny);
    return 0;
}
