#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x12d];u8 value,gap,state;}Data;extern Data*_022577C0;extern void*ov45_0222F7B0(int);extern void*ov45_02231C8C(void*);extern int ov45_0222F74C(void*);

int ov45_0222EB38(int value);

int ov45_0222EB38(int value){int result;if(_022577C0==0)GF_AssertFail();result=ov45_0222F74C(ov45_02231C8C(ov45_0222F7B0(value)));if(result==1){_022577C0->value=value;_022577C0->state=0;}return result;}
