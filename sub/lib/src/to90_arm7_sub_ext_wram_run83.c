#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u8 wmspW[];
typedef struct {u8 pad[0x54c];u8 *iface;u8 *state;} To90W35378Global;
#define G ((To90W35378Global *)(wmspW+0x1000))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern void WMSP_CopyParentParam(void *,void *);
extern u16 *WMSP_WL_ParamSetGameInfo(void *,u16,void *);
void WMSP_SetEntry(u32 *);
// @fn WMSP_SetEntry
void WMSP_SetEntry(u32 *arg) {
 u32 buf[128],parent[32];u8 *s=G->state;u16 *resp,*out;
 H(s,0xf6)=arg[1];WMSP_CopyParentParam(parent,s+0xe8);
 resp=WMSP_WL_ParamSetGameInfo(buf,(u16)(H(s,0xec)+16),parent);
 out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out[0]=0x21;
 if(!resp[2])out[1]=0;else{out[1]=1;out[2]=0x245;out[3]=resp[2];}
 __VENEER_WMSP_ReturnResult2Wm9(out);
}
