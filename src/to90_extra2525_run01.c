#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s32 x,y,z;}Vector;typedef struct{u32 low:20,angle:2,high:10;}Flags;typedef struct{u8 prefix[44];u8 object[52];Flags flags;}Work;
typedef struct{s16 value,tail[2];}Sample;extern const Sample ov96_0221D708[],ov96_0221D70A[],ov96_0221D70C[];int ov96_02218688(Work*),PokeathlonRingDrop_IsWithinDistance(void*,const Vector*,s32);int ov96_0221862C(Work*);
int ov96_0221862C(Work*w){if(ov96_02218688(w)){Vector v={0,0,0};u8 index=w->flags.angle;v.x=ov96_0221D708[index].value*4096;v.y=ov96_0221D70A[index].value*4096;return PokeathlonRingDrop_IsWithinDistance(w->object,&v,ov96_0221D70C[index].value*4096);}return 0;}
