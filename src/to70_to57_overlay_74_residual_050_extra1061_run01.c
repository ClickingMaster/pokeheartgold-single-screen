#pragma force_active on
#include "global.h"
typedef struct {u8 p[0xfe0];u8 a[0x68];int flag;} W;extern W *ov74_0223E2FC;
extern void *Heap_Alloc(int,u32);void *ov74_02236988(void);void *ov74_022369A8(void*,int);
void *ov74_02236988(void) {if(ov74_0223E2FC->flag)return ov74_0223E2FC->a;return 0;}
