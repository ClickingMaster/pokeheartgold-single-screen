#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern void *WMSP_WlRequest(void *);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
u16* WMSP_WL_MaData(u16 *p,u16*a);
// @fn WMSP_WL_MaData
u16* WMSP_WL_MaData(u16 *p,u16*a){ u16 *q;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=256;p[7]=24;__VENEER_MIi_CpuCopy16(a,p+8,48);a[1]=0;a[2]=0;a[4]=0;a[5]=0;a[6]=0;a[8]=0;a[9]=0;a[10]=0;a[11]=0;a[18]=0;a[19]=0;a[20]=0;a[21]=0;q=p+p[7];q[8]=p[6];q[9]=2;WMSP_WlRequest(p);return q+8;}
