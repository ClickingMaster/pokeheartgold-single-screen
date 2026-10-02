#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[6];s16 first;u8 p2,count;u8 p3[2];} Entry;
typedef struct {u32 pad;Entry entries[8];u8 index;} List;
typedef struct {u8 p[0x234];List*list;u8 p2[0x68];void*sprite;u8 p3[0x3ce];u8 selected;} Work;
extern u8 ov15_02200A34[],ov15_02200A35[],ov15_02200A36[],ov15_02200A37[];
extern void ManagedSprite_SetAnim(void*,int);
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void*,int,int,int);
extern void ManagedSprite_SetPaletteOverride(void*,int);
void ov15_021FFF34(Work*,int);
void ov15_021FFF34(Work*w,int index){if(index>=9)GF_AssertFail();if(index==8)ManagedSprite_SetAnim(w->sprite,ov15_02200A36[index*4]);else{List*l=w->list;Entry*e=&l->entries[l->index];int value=e->first+index;if(value==w->selected)ManagedSprite_SetAnim(w->sprite,10);else if(value>=e->count)ManagedSprite_SetAnim(w->sprite,40);else ManagedSprite_SetAnim(w->sprite,20);}ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprite,ov15_02200A34[index*4],ov15_02200A35[index*4],1<<20);ManagedSprite_SetPaletteOverride(w->sprite,ov15_02200A37[index*4]);}
