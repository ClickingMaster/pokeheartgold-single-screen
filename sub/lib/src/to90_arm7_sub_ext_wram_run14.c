#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern void *WMSP_WlRequest(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
u16* WMSP_WL_MlmeDeAuthenticate(u16 *p,const void*a,u16 b);
// @fn WMSP_WL_MlmeDeAuthenticate
u16* WMSP_WL_MlmeDeAuthenticate(u16 *p,const void*a,u16 b){ u16 *q;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=5;p[7]=4;__VENEER_MIi_CpuCopy16(a,p+8,6);p[11]=b;q=p+p[7];q[8]=p[6];q[9]=4;WMSP_WlRequest(p);return q+8;}
