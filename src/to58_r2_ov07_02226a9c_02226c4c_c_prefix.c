#include "global.h"
void *ov07_022324D8(void *, int);
void ov07_02231FE4(void *, void *);
int ov07_0221C470(void *);
int ov07_02221F80(void *, int, int);
int ov07_0221BFC0(void *);
int ov07_0223192C(void *, int);
void ManagedSprite_SetPositionXY(void *, int, int);
void ManagedSprite_SetAnim(void *, int);
int ov07_0221FB04(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ov07_0221C410(void *, void *, void *);
void ov07_02226A68(void);
void ov07_02226A9C(void *, int, int, void *);

void ov07_02226A9C(void *context, int ignored1, int ignored2, void *sprite) {
    u8 *work = ov07_022324D8(context, 0x38);
    ov07_02231FE4(context, work);
    *(void **)(work + 0x1C) = sprite;
    int arg = ov07_0221C470(context);
    int x = ov07_02221F80(context, arg, 0);
    ov07_02221F80(context, arg, 1);
    int anim;
    int y;
    if (ov07_0221BFC0(context) == 1) {
        x = 0x48;
        y = 0x20;
        anim = 0;
    } else if (ov07_0223192C(*(void **)(work + 4), arg) == 3) {
        y = 0x7E;
        anim = 1;
    } else {
        y = 0x20;
        anim = 0;
    }
    ManagedSprite_SetPositionXY(*(void **)(work + 0x1C), x, y);
    ManagedSprite_SetAnim(*(void **)(work + 0x1C), anim);
    int priority = ov07_0221FB04(*(void **)(work + 4), 2);
    ManagedSprite_SetPriority(*(void **)(work + 0x1C), priority);
    ov07_0221C410(*(void **)(work + 4), (void *)ov07_02226A68, work);
}
