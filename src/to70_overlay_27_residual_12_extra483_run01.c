#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
extern s32 FontID_String_GetWidth(u32,String *,u32);
#include "sprite.h"
typedef struct {BgConfig *bg;void *phone;Sprite *sprite;Window window;int shown;} Work;
extern int GearPhoneRingManager_IsRinging(void *);extern String *GetPhoneBookEntryName(void *,int);extern void ov27_0225B630(Window *,int);extern void ov27_0225BB38(Window *,String *,int);extern void ov27_0225BEB0(Work *);
void ov27_0225BDFC(Work *);
void ov27_0225BDFC(Work *w){int ringing=GearPhoneRingManager_IsRinging(w->phone);if(w->shown==0){if(ringing==1&&w->shown==0){String *name=GetPhoneBookEntryName(w->phone,8);int width=FontID_String_GetWidth(0,name,0)/8+2;if(width>9)width=9;AddWindowParameterized(w->bg,&w->window,5,11,19,(u8)width,2,4,192);ov27_0225B630(&w->window,1);ov27_0225BB38(&w->window,name,1);String_Delete(name);Sprite_TryChangeAnimSeq(w->sprite,5);w->shown=1;}}else{if(ringing){Sprite_TryChangeAnimSeq(w->sprite,5);}else{Sprite_TryChangeAnimSeq(w->sprite,0);ov27_0225BEB0(w);w->shown=0;}}}
