#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Node {volatile u32 state;u32 id;u32 kind;void *sprite,*window;u32 extra[2];} Node;
typedef struct Work {Node nodes[12];u8 pad150[0x38];int selected;u8 pad18c[0x78];int count,active;u8 pad20c[0x90];u32 outState,outValue,outAction;u8 pad2a8[0x20];u32 field2c8;} Work;
extern int ov40_02244054(u32,u32);
extern void ManagedSprite_SetAnim(void*,int);
extern void ov40_0224320C(Work*,int);
void ov40_02243614(Work*);
void ov40_02243614(Work*w){if(w->active==0){w->selected=w->count-1;u32 id=w->nodes[w->selected].id;w->outState=1;w->outValue=id;w->outAction=1;return;}int index=w->selected;w->nodes[index].state=0;ManagedSprite_SetAnim(w->nodes[index].sprite,ov40_02244054(w->nodes[index].state,w->nodes[index].kind));u32 id=w->nodes[index].id;if(index>0){ManagedSprite_SetAnim(w->nodes[index-1].sprite,ov40_02244054(w->nodes[index-1].state,w->nodes[index-1].kind));u32 prev=w->nodes[index-1].id;if(id!=prev){w->outState=1;w->outValue=prev;w->outAction=1;}else{w->outState=2;w->outValue=index-1;}}else{w->field2c8=0;ov40_0224320C(w,3);}}
