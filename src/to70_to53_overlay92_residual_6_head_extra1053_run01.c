#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x320];u8 model[0x88];u8 clips[0x16c];int selected;} W;
extern void ModelInstance_RemoveAnimation(void*,void*);extern void ModelInstance_AddAnimation(void*,void*);
void ov92_0225DD48(W*,int);
void ov92_0225DD48(W*w,int selected) {if(selected!=w->selected){ModelInstance_RemoveAnimation(w->model,w->clips+w->selected*20);w->selected=selected;ModelInstance_AddAnimation(w->model,w->clips+selected*20);}}
