#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int ov00_021EE568(void);extern void ov00_021EF3C0(int,int);extern void ov00_021F0238(int,int);extern void ov00_021F2EE4(int,int);extern void ov00_021EC24C(int,int);
int ov00_021F8380(int);
int ov00_021F8380(int error){int code,kind;if(!error)return 0;switch(error){case 1:kind=6;code=kind-0x38;break;case 2:kind=6;code=kind-0x42;break;case 3:kind=6;code=kind-0x24;break;case 4:kind=6;code=kind-0x56;break;case 5:kind=6;code=kind-0x1a;break;}switch(ov00_021EE568()){case 2:ov00_021EF3C0(kind,code-64000);break;case 4:ov00_021F0238(kind,code-74000);break;case 5:ov00_021F2EE4(kind,code-84000);break;default:ov00_021EC24C(kind,code-94000);break;}return error;}
