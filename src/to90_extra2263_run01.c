#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u16 x,y,z,w;} Angle;typedef struct{u8 pad[24];Angle angle;} Data;extern const Data ov07_02236414;extern void*sub_02015524(void*);extern void sub_02015528(void*,int),Camera_SetAnglePos(const Angle*,void*);int ov07_02221D88(void*,void*);
int ov07_02221D88(void*unused,void*w){Angle angle=ov07_02236414.angle;void*camera=sub_02015524(*(void**)((u8*)w+4));sub_02015528(*(void**)((u8*)w+4),1);Camera_SetAnglePos(&angle,camera);return 1;}
