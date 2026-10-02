#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[0x2c];int state;u8 more[0xfc];int selected;u8 more2[0xdb4];void*sprites[8];}W;
extern void Sprite_SetAnimCtrlSeq(void*,int),ov70_02238E50(W*,int,int),ov70_02241330(W*,int,int),PlaySE(int);void ov70_0223AFA8(W*,int);
void ov70_0223AFA8(W*w,int selection){if(selection!=w->selected&&selection>=0){Sprite_SetAnimCtrlSeq(w->sprites[selection+1],selection*4+16);w->state=2;ov70_02238E50(w,3,17);w->selected=selection;ov70_02241330(w,selection,0);PlaySE(1500);}}
