#pragma force_active on
#include "global.h"
#include "sound_playback.h"
#include "field_system.h"
#include "field/overlay_01_021FB878.h"
#include "field/field_3d_object_task.h"
typedef struct {Field3dObject object;Field3dModel model;Field3DModelAnimation animation[4];u32 count;} Object;
typedef struct {Object objects[2];NNSFndAllocator allocator;u16 index,state;} State;
typedef struct { u8 v[2]; } Pair;
extern Pair ov02_022538EC[];
extern void ov02_0224D0C8(void*,int,int,int,void*),ov02_0224D0AC(void*,void*),ov02_0224D1AC(void*);
extern BOOL ov02_0224D178(void*);
void ov02_0224D488(Field3dObjectTask*,FieldSystem*,State*);
// @fn ov02_0224D488
void ov02_0224D488(Field3dObjectTask *unused,FieldSystem *c,State *s){
Pair a; Pair b; u8 n; BOOL done; u16 *state = &s->state; switch(*state){case 0: a=ov02_022538EC[1]; b=ov02_022538EC[2]; n=s->index; ov02_0224D0C8(&s->objects[n],a.v[n],b.v[n],2,&s->allocator);ov02_0224D0AC(c->playerAvatar,&s->objects[n]);s->index++;if(s->index>=2)s->state++;break; case 1: Field3dObject_SetActiveFlag(&s->objects[1].object,0);PlaySE(SEQ_SE_DP_FW463);s->state++;
case 2: done=ov02_0224D178(&s->objects[0]);ov02_0224D0AC(c->playerAvatar,&s->objects[0]);if(done==1){Field3dObject_SetActiveFlag(&s->objects[1].object,1);Field3dObject_SetActiveFlag(&s->objects[0].object,0);ov02_0224D0AC(c->playerAvatar,&s->objects[1]);s->state++;}break; case 3: ov02_0224D1AC(&s->objects[1]);ov02_0224D0AC(c->playerAvatar,&s->objects[1]);break;}
}
