#include "global.h"
extern int Sprite_GetDrawFlag(void *);
extern void ov91_0225D8E0(void *, int);
extern void ov91_0225D884(void *, int);
void ov91_0225D84C(u8 *data);
void ov91_0225D84C(u8 *data) {
    int i;
    u8 *entry;
    i = 0;
    entry = data;
    while (i < 3) {
        if (Sprite_GetDrawFlag(*(void **)entry) == 1) {
            u32 n = ++*(u32 *)(entry + 0xC);
            if (n > 16) ov91_0225D8E0(data, i);
            else ov91_0225D884(data, i);
        }
        i++;
        entry += 4;
    }
}
