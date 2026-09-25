#include "global.h"

extern void GF_AssertFail(void);
extern void ov96_0220B148(void *, s16 *, s16 *);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
void ov96_0220B068(u8 *);

void ov96_0220B068(u8 *p) {
    s16 pos[2];
    if (p == NULL) {
        GF_AssertFail();
    }
    ov96_0220B148(p, &pos[1], &pos[0]);
    pos[1] += *(s16 *)((u8 *)p + 0xA);
    pos[0] += *(s16 *)((u8 *)p + 0xC);
    ManagedSprite_SetPositionXY(*(void **)((u8 *)p + 4), pos[1], pos[0]);
}
