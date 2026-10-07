#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern void *WMSP_WlRequest(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
u16* WMSP_WL_MlmeReset(u16 *p,u16 a);
// @fn WMSP_WL_MlmeReset
u16* WMSP_WL_MlmeReset(u16 *p,u16 a){ u16 *q;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=0;p[7]=1;p[8]=a;q=p+p[7];q[8]=p[6];q[9]=1;WMSP_WlRequest(p);return q+8;}
