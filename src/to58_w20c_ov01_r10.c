#include "global.h"

extern int ov01_021F5A80(int, int, int, int);
extern int ov01_021F5A04(int, int, int);
extern int ov01_021F6328(int, int);
extern void ov01_021F562C(int, int, void *);
extern void ov01_021F5B24(int, int, u8 *);
void ov01_021F613C(int direction, void *manager);

#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define B(p, n) (*(u8 *)((u8 *)(p) + (n)))

void ov01_021F613C(int direction, void *manager) {
    int next = ov01_021F5A80(direction, W(manager, 0xc8), W(manager, 0xcc), W(manager, 0xa8));
    int slot = ov01_021F5A04(next, W(manager, 0xc4), W(manager, 0xcc));
    int orient = ov01_021F6328(next, W(manager, 0xcc));
    if (W(manager, 0xa4) == slot && B(manager, 0xad) != orient) {
        ov01_021F562C(slot, direction, manager);
        ov01_021F5B24(direction, B(manager, 0xad), (u8 *)manager + 0xac);
    }
}
