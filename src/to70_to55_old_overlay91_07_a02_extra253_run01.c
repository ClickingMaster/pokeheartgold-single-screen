#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Data{u8 kind;u8 pad[0x2b];s32 x,y,z;}Data;typedef struct W{u8 pad[4];u8 object[120],extra[120];int state;Data*data;}W;typedef struct Model{u8 pad[8];void*model;}Model;
extern int ov91_02260A88(W*,Data*),sub_020182A4(void*),ov91_0225E9AC(Data*),ov91_0225E990(Data*);extern Model*ov91_02260B48(Data*,void*);extern void ov91_02260AF8(W*,void*),NNS_G3dMdlSetMdlAmbAll(void*,u16),ov91_02260B5C(W*),sub_020182A8(void*,s32,s32,s32),sub_020181EC(void*),ov91_02260A10(W*),sub_020182C4(void*,s32,s32,s32);void ov91_022608A8(W*,void*),ov91_02260950(W*);
void ov91_022608A8(W*w,void*context){if(w->state==1&&ov91_02260A88(w,w->data)==0)ov91_02260AF8(w,context);if(w->state==0){Model*model=ov91_02260B48(w->data,(u8*)context+0x6fc);if(w->data->kind==5)NNS_G3dMdlSetMdlAmbAll(model->model,0x7fff);else NNS_G3dMdlSetMdlAmbAll(model->model,0x4a52);ov91_02260B5C(w);sub_020182A8(w->object,w->data->x,w->data->y,w->data->z);sub_020181EC(w->object);if(sub_020182A4(w->extra)==1){ov91_02260950(w);sub_020181EC(w->extra);ov91_02260A10(w);}}}
