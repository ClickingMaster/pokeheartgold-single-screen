#pragma force_active on
#include "global.h"
#pragma force_active on
void ov07_02231DE0(int,VecFx32 *,int,int);void ov07_02232BB0(int,VecFx32 *);
void ov07_02232BB0(int mode,VecFx32 *pos){int x,y;ov07_02231DE0(mode,pos,0,1);switch(mode){case 0:x=0;y=-16;break;case 1:x=-15;y=-25;break;case 2:x=-8;y=-12;break;case 3:x=-14;y=-25;break;case 4:x=-8;y=-12;break;case 5:default:x=-16;y=-29;break;}pos->x-=(x+10)*172;pos->y-=(y+32)*172;}
