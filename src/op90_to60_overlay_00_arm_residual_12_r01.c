#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct { int a, b, c, state; } Conn;
typedef enum { FLAG_OFF = 0, FLAG_ON = 1 } Flag;
int ov00_0220D35C(Conn *c, int kind, Flag flag);
int ov00_0220E0F0(Conn *c);

// @fn ov00_0220E0F0
int ov00_0220E0F0(Conn *c) {
    int r;
    if (c->state == 7) return 1;
    r = ov00_0220D35C(c, 2, c->state == 6 ? FLAG_OFF : FLAG_ON);
    if (r == 0) return 0;
    return 1;
}
