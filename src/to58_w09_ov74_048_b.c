#include "global.h"

#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))

extern u8 *ov74_0223E2FC;
extern const u8 ov74_0223D00C[];
extern const u8 ov74_0223D014[];
extern void *ov74_0223CFE4[];
extern int WM_EndScan(void *);
extern int WM_Initialize(void *, void *, int);
extern int WM_SetIndCallback(void *);
extern int WM_End(void *);
extern int WM_Reset(void *);
extern void OS_Terminate(void);
extern void ov74_02236168(int);
extern void ov74_022361B8(int);
extern int ov74_022361D4(void);
extern int ov74_022365AC(void);
extern void ov74_022365FC(void);
extern int ov74_0223653C(void);
extern int ov74_022364F0(void);
extern int ov74_0223648C(void);
extern void ov74_022364A8(void *);
extern void ov74_022364C0(void *);
extern void ov74_02236514(void *);
extern void ov74_02236570(void *);
extern int ov74_02236620(void);
extern void ov74_02236644(void *);
extern int ov74_02236258(void);
int ov74_02236680(void *, void *, void *);
void ov74_022366E8(u8 *);
u16 ov74_022368A4(u16 *, u32);
int ov74_022368D4(void);

int ov74_0223648C(void) {
    int result = WM_EndScan(ov74_022364A8);
    if (result != 2) {
        ov74_022361B8(result);
        return 0;
    }
    return 1;
}

void ov74_022364A8(void *event) {
    int result = U16(event, 2);
    if (result != 0) {
        ov74_022361B8(result);
    } else {
        ov74_02236168(1);
    }
}

void ov74_022364C0(void *event) {
    if (U16(event, 2) != 0) {
        ov74_02236168(10);
    } else {
        ov74_02236168(0);
        void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
        if (callback != NULL) {
            callback(3);
        }
    }
}

int ov74_022364F0(void) {
    U32(ov74_0223E2FC, 0x1154) = 0;
    if (ov74_0223653C() != 0) {
        return 1;
    }
    return 0;
}

void ov74_02236514(void *event) {
    if (U16(event, 2) == 8) {
        ov74_02236168(9);
        void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
        if (callback != NULL) {
            callback(4);
        }
    }
}

int ov74_0223653C(void) {
    ov74_02236168(3);
    int result = WM_Initialize(ov74_0223E2FC, ov74_02236570, 2);
    if (result != 2) {
        ov74_022361B8(result);
        ov74_02236168(10);
        return 0;
    }
    return 1;
}

void ov74_02236570(void *event) {
    int result = U16(event, 2);
    if (result != 0) {
        ov74_022361B8(result);
        ov74_02236168(10);
    } else {
        result = WM_SetIndCallback(ov74_02236514);
        if (result != 0) {
            ov74_022361B8(result);
            ov74_02236168(10);
        } else {
            ov74_02236168(1);
            ov74_022361D4();
        }
    }
}

int ov74_022365AC(void) {
    if (U32(ov74_0223E2FC, 0x1150) != 1) {
        OS_Terminate();
    }
    ov74_02236168(3);
    int result = WM_End(ov74_022364C0);
    if (result != 2) {
        ov74_02236168(9);
        void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
        if (callback != NULL) {
            callback(4);
        }
        return 0;
    }
    return 1;
}

void ov74_022365FC(void) {
    if (U32(ov74_0223E2FC, 0x1150) == 1) {
        ov74_022365AC();
    } else {
        ov74_02236620();
    }
}

int ov74_02236620(void) {
    ov74_02236168(3);
    int result = WM_Reset(ov74_02236644);
    if (result != 2) {
        ov74_022361B8(result);
        return 0;
    }
    return 1;
}

void ov74_02236644(void *event) {
    if (U16(event, 2) != 0) {
        ov74_02236168(9);
        ov74_022361B8(U16(event, 2));
        void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
        if (callback != NULL) {
            callback(4);
        }
    } else {
        ov74_02236168(1);
        ov74_022365AC();
    }
}

int ov74_02236680(void *callback, void *other, void *memory) {
    ov74_0223E2FC = (u8 *)(((u32)memory + 31) & ~31);
    U32(ov74_0223E2FC, 0x1150) = 0;
    U16(ov74_0223E2FC, 0x1158) = 1;
    U32(ov74_0223E2FC, 0x1178) = (u32)callback;
    U16(ov74_0223E2FC, 0x115c) = 0;
    U16(ov74_0223E2FC, 0x115e) = 0;
    U32(ov74_0223E2FC, 0x117c) = (u32)other;
    U16(ov74_0223E2FC, 0x1176) = 16;
    U32(ov74_0223E2FC, 0x1048) = 0;
    U32(ov74_0223E2FC, 0x116c) = 0xffffffff;
    return ov74_022364F0();
}

extern u32 _u32_div_f(u32, u32);
extern void CRYPTO_RC4Init(void *, const void *, u32);
extern void CRYPTO_RC4Encrypt(void *, void *, u32, void *);
void ov74_022366E8(u8 *packet) {
    U32(ov74_0223E2FC, 0x1048) = 0;
    U16(ov74_0223E2FC, 0x1158) = 3;
    U16(ov74_0223E2FC, 0x115a) = U16(packet, 0);
    U32(ov74_0223E2FC, 0x1160) = U32(packet, 4);
    u8 *state = ov74_0223E2FC;
    U16(state, 0x115c) = _u32_div_f(U32(state, 0x1160) - 1, 0x68) + 1;
    U16(ov74_0223E2FC, 0x115e) = 0;
    U32(ov74_0223E2FC, 0x1164) = 0;
    for (int i = 0; i < U16(ov74_0223E2FC, 0x115c); i++) {
        ov74_0223E2FC[0x1180 + i] = 0;
    }
}
