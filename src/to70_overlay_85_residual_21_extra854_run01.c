#pragma force_active on
#include "global.h"
typedef struct{int pos[3],result[3],gap[3],extra[3],offset[3],angle,a,b;u8 gap2[6];u16 rotation;}Data;extern void ov85_021E8530(int*,int);
void ov85_021E75C8(u8*w);
void ov85_021E75C8(u8*w){Data*d=(Data*)(w+0xd4);int angle=d->angle;ov85_021E8530(&angle,d->a);ov85_021E8530(&angle,d->b);d->rotation=(360-angle/4096)%360;d->result[0]=d->extra[0]+(d->pos[0]+d->offset[0]);d->result[1]=d->extra[1]+(d->pos[1]+d->offset[1]);d->result[2]=d->extra[2]+(d->pos[2]+d->offset[2]);}
