#include "frontier/ov80_frontier_state_prepare.h"

int sub_020313C4(void *, int, int, int, int, int, int *, int *);
int sub_0205C0F4(int);
int sub_0205C11C(int);
int sub_0205C144(int);
int sub_0205C268(int);

void ov80_0223151C(void *work, u16 *out) {
    u8 *p = work;
    int mode = p[4];
    int local1;
    int local2;
    int stat1;
    int stat2;
    int stat3;

    if (mode != 3) {
        stat1 = sub_0205C0F4(mode);
        stat2 = sub_0205C11C(p[4]);
        stat3 = sub_0205C268(sub_0205C0F4(p[4]));
        *out = sub_020313C4(*(void **)(p + 0x6FC), stat1, stat2, stat3, sub_0205C144(mode), 11, &local1, &local2);
    } else {
        *out = 0;
    }
}
