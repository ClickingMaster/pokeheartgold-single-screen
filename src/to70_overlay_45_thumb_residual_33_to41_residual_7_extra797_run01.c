#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x69c];u16 index;u8 enabled;} Work;
extern Work *_022577C0;
extern void ov45_02232BB0(void *);
extern int ov45_0222F74C(void);
void ov45_0222F1BC(void);
void ov45_0222F1BC(void){if(!_022577C0)GF_AssertFail();if(_022577C0->enabled){*(int*)((u8*)_022577C0+_022577C0->index*0x4c+0x5c4)=1;ov45_02232BB0((u8*)_022577C0+0x5b8+_022577C0->index*0x4c);if(!ov45_0222F74C())GF_AssertFail();}}
