#pragma force_active on
#include "global.h"
#pragma force_active on
void MTX_RotX33_(MtxFx33*p,fx32 sine,fx32 cosine){volatile s32*m=(volatile s32*)p;m[0]=4096;m[1]=0;m[2]=0;m[3]=0;m[4]=cosine;m[5]=sine;m[6]=0;m[7]=(s32)(0u-(u32)sine);m[8]=cosine;}
