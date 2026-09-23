#include "to58_r2_ov07_mid0_private.h"
void *ov07_02222D90(int type);

void *ov07_02222D90(int type) {
    switch (type) {
    case 0: return (void *)0x04000010;
    case 1: return (void *)0x04000014;
    case 2: return (void *)0x04000018;
    case 3: return (void *)0x0400001C;
    }
    return (void *)type;
}
