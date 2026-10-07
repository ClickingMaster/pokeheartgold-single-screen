#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int DWC_CheckUserData(void*),DWC_CheckHasProfile(void*),ov45_02231144(int,int,int,void*,int,int,int,int);
int ov45_022310C0(int a,int b,int c,void*d,u8*user,int e,int f);
#pragma force_active on
#pragma opt_pointer_analysis off
int ov45_022310C0(int a,int b,int c,void*d,u8*user,int e,int f){u8*p=(u8*)(u32)user;if(!DWC_CheckUserData(p)||!DWC_CheckHasProfile(p))return 3;return ov45_02231144(a,b,c,d,*(int*)(p+28),e,f,1);}
