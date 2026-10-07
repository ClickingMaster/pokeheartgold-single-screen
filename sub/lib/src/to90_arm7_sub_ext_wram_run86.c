#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u8 wmspW[];
typedef struct {u8 pad[0x54c];u8 *iface;u8 *state;} To90W35382Global;
#define G ((To90W35382Global *)(wmspW+0x1000))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void __VENEER_PM_SetLEDPattern(u32);
void WMSP_Disable(void *);
// @fn WMSP_Disable
void WMSP_Disable(void *arg) {
 u8 *s=G->state;u16 *out;
 if(H(s,0)!=1) {out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out[0]=4;out[1]=3;__VENEER_WMSP_ReturnResult2Wm9(out);return;}
 __VENEER_PM_SetLEDPattern(1);H(s,0)=0;
 out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out[0]=4;out[1]=0;__VENEER_WMSP_ReturnResult2Wm9(out);
}
