#ifndef TO55_W04_PRIVATE_H
#define TO55_W04_PRIVATE_H
#include "global.h"
typedef struct Window {
    u8 raw[0x10];
} Window;
typedef struct Ov112Windows {
    u8 pad[0x1ea68];
    Window lower[20];
    Window upper[10];
    u8 lowerCount;
    u8 upperCount;
    u8 lowerState;
    u8 upperState;
} Ov112Windows;
void GF_AssertFail(void);
void RemoveWindow(Window *);
extern const u8 ov112_021FEC80[];
extern const u8 *ov112_021FF504[];
u8 ov112_021E98F8(const u8 *, int, int);
void ov112_021E9934(u8 *, int, int, int);
void ov112_021E9998(u8 *, const u8 *, u32, int, int);
void ov112_021E9A30(void *, const void *, int);
void ov112_021E9C10(Ov112Windows *, int);
#endif
