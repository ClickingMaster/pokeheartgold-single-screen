#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct { int a, b, c, state; } Conn;
typedef enum { FLAG_OFF = 0, FLAG_ON = 1 } Flag;
int ov00_0220D35C(Conn *c, int kind, Flag flag);
int ov00_0220ED98(Conn *c);
int ov00_0220D9A4(Conn *c);

// @fn ov00_0220D9A4
int ov00_0220D9A4(Conn *c) {
    int r;
    if (ov00_0220ED98(c) == 0) return 0;
    r = ov00_0220D35C(c, 2, c->state == 6 ? FLAG_OFF : FLAG_ON);
    if (r == 0) return 0;
    return 1;
}
