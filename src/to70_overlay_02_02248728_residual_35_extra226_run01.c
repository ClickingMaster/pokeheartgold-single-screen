#pragma force_active on
#include "global.h"
#pragma force_active on
struct F{u8 pad[64];void*avatar;};struct E{int a,b,play,type;u8 pad[16];void*object;struct F*field;void*mon;int x;};
void*ov02_0224C660(int,int);void*PlayerAvatar_GetMapObject(void*);int PlayerAvatar_GetState(void*);int ov01_02206268(struct F*);int ov01_022062CC(struct F*);struct E*FieldMoveTask_CreateTeleportEnvironment(struct F*,void*,int,int);
struct E*FieldMoveTask_CreateTeleportEnvironment(struct F*f,void*m,int index,int heap){struct E*e=ov02_0224C660(heap,48);e->type=2;e->field=f;e->object=PlayerAvatar_GetMapObject(f->avatar);e->mon=m;if((u32)(PlayerAvatar_GetState(f->avatar)-1)<=1){e->play=0;}else{if(ov01_02206268(f)&&index==ov01_022062CC(f))e->play=1;else e->play=0;}return e;}
