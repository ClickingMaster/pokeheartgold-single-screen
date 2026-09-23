#include "global.h"
extern void *ov74_022311E8(void);
extern void *ov74_02231100(void);
extern void ov74_022305E4(void);
extern int WM_StartConnectEx(void *, void *, void *, int, int);
extern void ov74_02231448(void);
BOOL ov74_02231638(void);
BOOL ov74_02231638(void) {
    void *data = ov74_022311E8();
    void *info = ov74_02231100();
    if (WM_StartConnectEx(ov74_022305E4, info, data, 1, 0) != 2) {
        ov74_02231448();
        return FALSE;
    }
    return TRUE;
}

extern void ov74_022308E0(void);
extern void ov74_022306C8(void);
extern int WM_SetPortCallback(int, void *, int);
extern int WM_StartMP(void *, void *, u16, void *, u16, u16);
extern u8 *ov74_02231054(void);
extern void *ov74_022310F4(void);
extern int ov74_022310DC(void);
extern void *ov74_022310E8(void);
extern int ov74_022310D0(void);
BOOL ov74_02231670(void);
BOOL ov74_02231670(void) {
    int flag;
    void *a, *b;
    int c;
    if (WM_SetPortCallback(4, ov74_022308E0, 0) != 0) {
        ov74_02231448();
        return FALSE;
    }
    flag = *(u16 *)(ov74_02231054() + 0x16) == 0;
    a = ov74_022310F4();
    c = ov74_022310DC();
    b = ov74_022310E8();
    if (WM_StartMP(ov74_022306C8, a, (u16)c, b, (u16)ov74_022310D0(), (u16)flag) != 2) {
        ov74_02231448();
        return FALSE;
    }
    return TRUE;
}
