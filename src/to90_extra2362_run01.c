#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 type;u8 pad[121];} Area;
typedef struct {u8 pad[0x10];int touch;u8 pad2[8];Area areas[6];u8 pad3[0xcc];u8 window[16];u8 pad4[0xec];void *prompt;u8 pad5[0x1801a];u8 row,selected,column;u8 pad6[7];int modified;} Work;
extern int YesNoPrompt_HandleInput(void *);extern int YesNoPrompt_IsInTouchMode(void *);extern void YesNoPrompt_Reset(void *);extern void SafariZone_InitAreaInSet(void *,int,u32);extern void ov108_021E7BB4(Work *,int,int);extern void ov108_021E61E8(Work *);extern void ov108_021E6238(Work *);extern void ClearWindowTilemapAndCopyToVram(void *);extern void ov108_021E767C(Work *,int);extern void ov108_021E7700(Work *,int,int,int);extern void ov108_021E79A8(void *, u8, int, int);
int ov108_021E5F38(Work *);
int ov108_021E5F38(Work *w){int yes;int touch;switch(YesNoPrompt_HandleInput(w->prompt)){case 1:{u8 chosen,current;yes=1;chosen=(u8)(w->row*6+w->column);current=w->areas[w->selected].type;if(chosen!=current){w->modified=1;SafariZone_InitAreaInSet(w->areas,w->selected,chosen);}break;}case 2:ov108_021E7BB4(w,w->selected,w->areas[w->selected].type);yes=0;break;default:return 1;}touch=YesNoPrompt_IsInTouchMode(w->prompt);if(touch!=w->touch){if(!touch)ov108_021E61E8(w);else ov108_021E6238(w);}w->touch=touch;YesNoPrompt_Reset(w->prompt);ClearWindowTilemapAndCopyToVram(w->window);ov108_021E767C(w,0);ov108_021E7700(w,0,1,1);ov108_021E79A8(w,1,0,0);if(yes)return 2;return 0;}
