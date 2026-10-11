#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 value;unsigned a:24;unsigned b:8;} Output;
extern u32 *PokeathlonCourse_GetParticipantData(void*,int);extern u8 *PokeathlonCourse_GetMonStats(void*,int,int);extern u32 ov96_021E8448(void*,u8);

void ov96_021EE1D8(Output*out,void*w,int idx);

typedef struct {volatile int index;u32 *volatile participant;} Frame;
void ov96_021EE1D8(Output*out,void*w,int idx){Frame f;f.index=idx;int sum,i;u32 total=0;f.participant=PokeathlonCourse_GetParticipantData(w,f.index);for(i=0;i<3;i++){u8*a=PokeathlonCourse_GetMonStats(w,f.index,i);total+=a[2]+a[0]+a[1]+a[3]+a[4]+5;}sum=(int)total/3-8;if(sum>12)sum=12;else if(sum<0)sum=0;out->value=ov96_021E8448(w,(u8)*f.participant);out->a=0;out->b=sum;}
