#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {void *first,*pad,*obj,*objs[2],*pad2,*items[7],*extra,*pad4,*last;u8 mode;} Inner;
typedef struct {u8 pad,flag;u16 pad2,value,pad3;Inner *inner;} Root;
extern Root ov13_0224F54C;extern u32 ov13_0224F554[];extern u8 ov13_022464CC[];
extern int ov13_02240394(int),DWCi_BACKUPlConvMaskCidr(void *);extern u8 *ov13_02239FF0(void);
extern void ov13_022425F0(int,void *),ov13_02241D98(void *),ov13_02239514(void),ov13_02240B04(int),ov13_02227D18(void),ov13_0222788C(int),ov13_02239288(void),ov13_0223FA50(void *),ov13_02227924(void *,void (*)(const void *,u32,u32)),ov13_02240EFC(int,int),ov13_0222B934(int,int),ov13_0222B8F8(int,int),ov13_0222B8E8(void (*)(void)),ov13_0223A414(int),ov13_022412B0(void *);
extern void ov13_02230FD8(void),ov13_02231B18(void),ov13_02237DD0(void),ov13_022352D0(void),ov13_022368D8(void),ov13_02237618(void);
void ov13_02233AD8(void);
void ov13_02233AD8(void) {
 if(ov13_02240394(1))return;if(ov13_02240394(0))return;
 ov13_022425F0(0,ov13_0224F54C.inner->first);ov13_022425F0(1,ov13_0224F54C.inner->last);
 int i;for(i=0;i<7;i++)ov13_02241D98(ov13_0224F54C.inner->items[i]);if(ov13_0224F54C.inner->extra)ov13_02241D98(ov13_0224F54C.inner->extra);
 ov13_02239514();ov13_02240B04(0);ov13_02227D18();if(ov13_0224F54C.inner->mode!=12)ov13_0222788C(ov13_0224F54C.inner->mode);
 ov13_02239288();ov13_0223FA50(ov13_0224F54C.inner->obj);for(i=0;i<2;i++)ov13_0223FA50(ov13_0224F54C.inner->objs[i]);
 ov13_02227924(ov13_022464CC,GX_LoadOBJPltt);ov13_02240EFC(1,1);ov13_02240EFC(0,29);
 *(vu32 *)0x04000010=0;*(vu32 *)0x04000018=0;*(vu16 *)0x0400000c=(*(vu16 *)0x0400000c & 0x43)|0xe10;
 int mode=ov13_0224F54C.inner->mode;
 switch(mode){case 0:case 1:ov13_0222B934(mode,0);ov13_0222B8F8(2,0);ov13_0222B8E8(ov13_02230FD8);break;
 case 4:case 5:case 6:case 9:case 10:{int value=mode-4;if((u32)mode>=9)value-=2;ov13_0222B8F8(2,0);ov13_0222B934(value,0);ov13_0222B8E8(ov13_02231B18);break;}
 case 11:{u8 *data=ov13_02239FF0();data[0xd0]=DWCi_BACKUPlConvMaskCidr(data+0xf0);if(data[0xf5]){MI_CpuFill8(data+0xc0,0,4);MI_CpuFill8(data+0xc4,0,4);MI_CpuFill8(data+0xf0,0,4);data[0xd0]=0;}if(data[0xf6])MI_CpuFill8(data+0xc8,0,8);ov13_0222B8F8(2,0);ov13_0222B934(0,0);ov13_0222B8E8(ov13_02237DD0);break;}
 case 12:ov13_0222B8F8(0,0);ov13_0222B8E8(ov13_022352D0);break;
 case 13:if(ov13_0224F54C.flag==0){ov13_0222B8F8(2,1);ov13_0222B8E8(ov13_022368D8);}else{ov13_0223A414(ov13_02239FF0()[0xf4]);ov13_0222B8F8(0,1);ov13_0222B8E8(ov13_02237618);}break;
 }
 ov13_022412B0(ov13_0224F554);
}
