#pragma force_active on
#include "global.h"
extern u8 ov70_02245ED0[],ov70_02245F28[],ov70_02245E3E[],ov70_02245DD0[],ov70_02245E5E[],ov70_02245DE4[];
extern int TouchscreenHitbox_FindRectAtTouchNew(void*);
int ov70_02242164(int,u32);
int ov70_02242164(int value,u32 mode) {switch(mode){case 4:return TouchscreenHitbox_FindRectAtTouchNew(ov70_02245ED0);case 6:return TouchscreenHitbox_FindRectAtTouchNew(ov70_02245F28);case 0:case 2:return TouchscreenHitbox_FindRectAtTouchNew(ov70_02245E3E);case 1:return TouchscreenHitbox_FindRectAtTouchNew(ov70_02245DD0);case 3:return TouchscreenHitbox_FindRectAtTouchNew(ov70_02245E5E);case 5:case 7:value=TouchscreenHitbox_FindRectAtTouchNew(ov70_02245DE4);}return value;}
