#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern void *WMSP_WlRequest(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
u16* WMSP_WL_MlmeJoin(u16 *p,u16 a,const void*b);
// @fn WMSP_WL_MlmeJoin
u16* WMSP_WL_MlmeJoin(u16 *p,u16 a,const void*b){ u16 *q;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=3;p[7]=34;p[8]=a;p[9]=0;__VENEER_MIi_CpuCopy16(b,p+10,68);q=p+p[7];q[8]=p[6];q[9]=5;WMSP_WlRequest(p);return q+8;}
