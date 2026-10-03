#pragma force_active on
#include "global.h"
extern u8*_0221A680;extern void ov00_021E70E0(void);extern void ov00_021F12F0(void(*)(void),int);

int ov00_021E70F4(void);

int ov00_021E70F4(void){if(_0221A680[0x10e0]==0){_0221A680[0x10e0]=1;ov00_021F12F0(ov00_021E70E0,0);}if(_0221A680[0x10e0]==2)return 1;return 0;}
