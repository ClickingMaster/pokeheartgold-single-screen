#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u16 state;          /* 0x00 */
    u8 pad2[0x18 - 2];
    u32 x18;            /* 0x18 */
    u8 pad1c[0xa2 - 0x1c];
    u8 xa2[6];          /* 0xa2 */
    u16 *xa8;           /* 0xa8 */
    u16 xac;            /* 0xac */
    u8 padae[0xe0 - 0xae];
    u8 macAddr[6];      /* 0xe0 */
    u8 pade6[0x18a - 0xe6];
    u8 bssid[6];        /* 0x18a */
    u8 pad190[0x1ec - 0x190];
    u16 x1ec;           /* 0x1ec */
} WmspStatus;
typedef struct {
    u8 pad[0x1550];
    WmspStatus *status;
} WmspWork;
extern WmspWork wmspW;
typedef struct {
    u16 x0;
    u16 x2, x4;
    u16 x6;
    u8 x8[6];
    u8 rate;     /* 0xe */
    u8 xf;
    u8 x10[8];
    u8 dst[6];   /* 0x18 */
    u8 src[6];   /* 0x1e */
    u8 x24[8];
    u16 *data;   /* 0x2c */
} MaDataReq;
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *src, void *dst, u32 size);
extern void __VENEER_MIi_CpuClear16(u16 data, void *dst, u32 size);
extern u16 *WMSP_WL_MlmePowerManagement(u16 *buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims);
extern u16 *WMSP_WL_MaData(u16 *buf, MaDataReq *req);
void WMSP_SetPowerSaveMode(void *msg);
// @fn WMSP_SetPowerSaveMode
void WMSP_SetPowerSaveMode(void *msg) {
    MaDataReq req;
    u16 bufArr[256];
    u16 *buf = bufArr;
    u32 *arg = msg;
    WmspStatus *status = wmspW.status;
    u16 *cb;
    u16 *rsp;

    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x28;
    if (status->state != 11) {
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    rsp = WMSP_WL_MlmePowerManagement(buf, arg[1] == 1, 0, 1);
    if (rsp[2] != 0) {
        cb[1] = 1;
        cb[2] = 1;
        cb[3] = rsp[2];
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status = wmspW.status;
    __VENEER_MI_CpuCopy8(status->bssid, status->xa2, 6);
    status->xa8 = bufArr;
    status->xac = 0;
    status->x18 = 1;
    __VENEER_MIi_CpuClear16(0, &req, sizeof(req));
    req.x0 = 0;
    req.x6 = 0;
    req.rate = (status->x1ec == 2) ? 20 : 10;
    __VENEER_MI_CpuCopy8(status->bssid, req.dst, 6);
    __VENEER_MI_CpuCopy8(status->macAddr, req.src, 6);
    req.data = bufArr;
    rsp = WMSP_WL_MaData(bufArr, &req);
    if (rsp[2] != 0) {
        cb[1] = 1;
        cb[2] = 0x100;
        cb[3] = rsp[2];
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}
