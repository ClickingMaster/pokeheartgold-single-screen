#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x320];u8 model[0x88];u8 clips[0x16c];int selected;} W;
extern void sub_020181E0(void*,void*);extern void sub_020181D4(void*,void*);
void ov92_0225DD48(W*,int);
void ov92_0225DD48(W*w,int selected) {if(selected!=w->selected){sub_020181E0(w->model,w->clips+w->selected*20);w->selected=selected;sub_020181D4(w->model,w->clips+selected*20);}}
