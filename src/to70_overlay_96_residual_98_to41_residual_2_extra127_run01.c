#pragma force_active on
#include "global.h"
#pragma force_active on
extern const VecFx32 ov96_0221CF00;
u16 CalcAngleBetweenVecs(const VecFx32 *,const VecFx32 *);
int ov96_0220D6CC(const fx32 *);
int ov96_0220D6CC(const fx32 *input){
 VecFx32 a=ov96_0221CF00;
 VecFx32 b={0};
 u16 angle;
 b.x=input[1];b.z=input[0];b.y=0;
 angle=CalcAngleBetweenVecs(&a,&b);
 if(angle<=0x2000 || angle>=0xe000)return 4;
 if(angle>0x2000 && angle<0x6000)return 2;
 if(angle>=0x6000 && angle<=0xa000)return 3;
 return 1;
}
