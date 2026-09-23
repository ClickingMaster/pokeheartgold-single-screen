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

void ov74_02236168(int value) {
    void (*callback)(const void *, void *) = ((void (**)(const void *, void *))&ov74_0223E2FC)[1];
    if (callback != NULL) {
        int index = U32(ov74_0223E2FC, 0x1150);
        callback(ov74_0223D00C, ov74_0223CFE4[index]);
    }
    U32(ov74_0223E2FC, 0x1150) = value;
    callback = ((void (**)(const void *, void *))&ov74_0223E2FC)[1];
    if (callback != NULL) {
        int index = U32(ov74_0223E2FC, 0x1150);
        callback(ov74_0223D014, ov74_0223CFE4[index]);
    }
}

void ov74_022361B8(int value) {
    u8 *state = ov74_0223E2FC;
    if ((u32)(U32(state, 0x1150) - 9) > 1) {
        U32(state, 0x1154) = value;
    }
}

int ov74_022361D4(void) {
    if (U32(ov74_0223E2FC, 0x1150) == 4) {
        OS_Terminate();
    }
    ov74_02236168(2);
    U16(ov74_0223E2FC, 0xfcc) = 0xffff;
    U16(ov74_0223E2FC, 0xfca) = 0xffff;
    U16(ov74_0223E2FC, 0xfc8) = 0xffff;
    if (ov74_02236258() == 0) {
        ov74_02236168(9);
        void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
        if (callback != NULL) {
            callback(4);
        }
        return 0;
    }
    U16(ov74_0223E2FC, 0x1158) = 2;
    void (*callback)(int) = *(void (**)(int))(ov74_0223E2FC + 0x117c);
    if (callback != NULL) {
        callback(0);
    }
    return 1;
}
