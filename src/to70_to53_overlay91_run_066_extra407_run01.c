#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Entry{u8 pad[248];void*data;u8 tail[12];}Entry;typedef struct Bank{u8 pad[1936];Entry entries[96];}Bank;typedef struct Data{u8 mode,pad;u16 pad2,angle;u8 gap[38];VecFx32 pos;}Data;typedef struct W{void*sprite;u8 anim[120];u8 anim2[120];int field;Data*data;}W;typedef struct Point{s16 x,y;}Point;
extern void ov91_022604A4(VecFx32*,VecFx32*,int,int),ov91_0226042C(VecFx32*,Point*),Sprite_SetMatrix(void*,VecFx32*),Sprite_SetDrawFlag(void*,int),ModelInstance_Init(void*,void*),ModelInstance_SetVisible(void*,int);extern void*ov91_02260B48(Data*,void*);Entry*ov91_02260A50(Bank*);int ov91_02260A88(W*,int,int);void ov91_02260AF8(W*,u8*);
Entry*ov91_02260A50(Bank*b){int i;for(i=0;i<96;i++){if(b->entries[i].data==0)return &b->entries[i];}GF_AssertFail();return 0;}
int ov91_02260A88(W*w,int unused,int value){Point point;VecFx32 vec=w->data->pos;ov91_022604A4(&vec,&vec,w->data->angle,value);ov91_0226042C(&vec,&point);vec.x=point.x<<12;vec.y=(point.y<<12)+2097152;vec.z=0;Sprite_SetMatrix(w->sprite,&vec);if(point.x<0||point.y<0)return 0;return 1;}
void ov91_02260AF8(W*w,u8*bank){Sprite_SetDrawFlag(w->sprite,0);w->field=0;ModelInstance_Init(w->anim,ov91_02260B48(w->data,bank+1788));ModelInstance_SetVisible(w->anim,1);if(w->data->mode==1)ModelInstance_SetVisible(w->anim2,1);}
