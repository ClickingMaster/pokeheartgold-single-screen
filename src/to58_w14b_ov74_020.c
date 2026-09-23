#include "global.h"
extern void *ov74_02233F5C(void);
u32 ov74_02233F68(void);
u32 ov74_02233F68(void) { u8 *p = ov74_02233F5C(); return (p[0xD] << 24) | (p[0xC] << 16) | (p[0xB] << 8) | p[0xA]; }
