#include "global.h"
#pragma force_active on
int ov45_0222A53C(void*);int ov45_0222ADA8(void*,int);void ov45_0222AE08(u32,u32*,u32*);int ov45_0222AD80(void*,int),ov49_02269430(u16*,void*,int);u32 ov49_0225E524(void*,int),ov49_0225E54C(void*,int);void ov49_0225E4CC(void*,int,int),ov49_02269178(u8*,int),ov49_022695C4(void*,int),ov49_02269240(void*,u32,BOOL,int,int);typedef struct{void*context,*first,*second,*aux;struct{u8 present,flag,value;} states[9];u8 gap;struct{u16 data[7];} details[9];u8 remaining[26];}Work;
typedef char WorkSizeProof[sizeof(Work)==0xc4?1:-1];
typedef char DetailOffsetProof[offsetof(Work,details)==0x2c?1:-1];
void ov49_02269098(void*);
void ov49_02269098(void*parent){Work*w=parent;int selected=ov45_0222ADA8(w->first,ov45_0222A53C(w->first));u32 detail;int present;int cached;int i;if(selected==-1)present=0;else{ov45_0222AE08(selected,(u32*)&selected,&detail);present=1;}for(i=0;i<9;i++){if(ov45_0222AD80(w->first,i)==1){ov49_0225E4CC(w->second,i,1);ov49_02269178((u8*)w,i);ov49_022695C4(w,i);if(present){if(selected==i)cached=1;else cached=0;}ov49_02269240(w,i,present,cached,detail);}else ov49_0225E4CC(w->second,i,0);}for(i=0;i<9;i++){u16*q=w->details[i].data;w->states[i].present=ov49_0225E524(w->second,i);w->states[i].flag=ov49_02269430(q,w->second,i);w->states[i].value=ov49_0225E54C(w->second,i);}}
