#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x44c];void*sprite;} Work;void sub_0208DAD4(Work*,int);void sub_0208D9A0(Work*,int);void sub_0208BB8C(Work*,int);void thunk_Sprite_SetDrawFlag(void*,int);void sub_0208A7F8(Work*,int);
void sub_0208A7F8(Work*w,int id){if(id==-1){sub_0208DAD4(w,id);thunk_Sprite_SetDrawFlag(w->sprite,0);return;}sub_0208D9A0(w,id);sub_0208BB8C(w,id);thunk_Sprite_SetDrawFlag(w->sprite,1);}
