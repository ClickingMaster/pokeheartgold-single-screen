#ifndef W08_OV49_PRIVATE_H
#define W08_OV49_PRIVATE_H
#include "global.h"
typedef struct W08State {
    s16 timer;
    u8 active;
    u8 selected;
    u16 previous;
    u16 disabled;
    u16 flag;
    u16 resource;
    u8 window[0x10];
    void *touch;
} W08State;
BOOL TouchHitboxController_IsTriggered(void *);
void ov49_0225CAD4(W08State *, void *, int, void *, int);
u16 ov45_0222AAC8(void *);
void ov49_0225BB14(void *, void *, int, int, int, int);
void *ov49_0225B388(void *, int, int);
void FillWindowPixelBuffer(void *, int);
u8 AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void GF_AssertFail(void);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
u32 GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
int ov49_0225C8D4(W08State *, void *, void *, int);
void ov49_0225CA30(W08State *, void *, int, void *, int);
void ov49_0225C970(W08State *, void *, void *, void *, void *, int);
#endif
