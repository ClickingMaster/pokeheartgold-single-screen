#pragma force_active on
#include "global.h"
typedef struct{int x,y,z;} Vec;typedef struct{void *sprites[2];s16 index,value;} Work;extern Vec ov90_0225C2B4[2];extern void Sprite_SetMatrix(void *,Vec *);extern void ov90_0225BD84(Work *);void ov90_0225BC28(Work *,int);
void ov90_0225BC28(Work *w,int delta){int value,i;Vec pos;w->value=w->index*23/8;if(w->index+delta<=8&&w->index+delta>=0)value=(w->index+delta)*23/8;else value=w->value;for(i=0;i<2;i++){pos=ov90_0225C2B4[i];if(i==0){float offset;if(value>0)offset=0.5f+(float)(value<<12);else offset=(float)(value<<12)-0.5f;pos.y-= (int)offset;}else{float offset;if(value>0)offset=0.5f+(float)(value<<12);else offset=(float)(value<<12)-0.5f;pos.y+=(int)offset;}Sprite_SetMatrix(w->sprites[i],&pos);}ov90_0225BD84(w);}
