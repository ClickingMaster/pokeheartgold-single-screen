#pragma force_active on
#include "global.h"
#include "touchscreen_list_menu.h"
#include "unk_0202C034.h"
#include "brightness.h"
#pragma force_active on
typedef struct {u8 other[0xe4];void *primary,*secondary,*menu;u8 tail[16];} MenuState;
typedef struct {u32 mode;void *save;u8 other8[16];u8 slots[0x40];u32 value;} Config;
typedef struct {void *bg;u8 other4[0x58];void *spawner;u8 tail[0x210-0x60];} Context;
typedef struct {u8 a[0xc8];u8 window[16];void *str;u32 printer;u32 speed;} PrinterWork;
typedef struct {u8 a[0x50];void *format;void *msg;} PrinterOther;
extern const struct {u32 first;TouchscreenListMenuTemplate template;} ov43_0222ED9C;
extern int ov43_0222C620(s16*);
extern void ov43_0222C630(TouchscreenListMenu*,u8,void*,int);
extern void ov43_0222AB20(void*,void*,int,int),ov43_0222C550(PrinterWork*,PrinterOther*,int,int),ov43_0222AD00(u8*,int);
void ov43_0222C148(void*,void*,void*,int);
void ov43_0222C148(void *state,void *configuration,void *context,int heap){
 MenuState *s=state;Config *config=configuration;Context *ctx=context;
 UnkStruct_021D2230 *saved;int index,kind;TouchscreenListMenuHeader header;u8 *choices;
 saved=sub_0202C6F4(config->save);index=ov43_0222C620(state);choices=config->slots;
 kind=sub_0202C090(saved,choices[index],8);
 MI_CpuFill8(&header,0,sizeof(header));header.template=ov43_0222ED9C.template;
 if(kind==2)header.listMenuItems=s->secondary;else header.listMenuItems=s->primary;
 header.bgConfig=ctx->bg;header.numWindows=4;
 s->menu=TouchscreenListMenu_CreateWithCallback(ctx->spawner,&header,(u8)config->mode,13,5,17,0,ov43_0222C630,NULL,TRUE);
 ov43_0222AB20(context,config->save,choices[index],heap);
 ov43_0222C550(state,context,12,heap);ov43_0222AD00(context,0);
 StartBrightnessTransition(4,8,0,(GXBlendPlaneMask)61,SCREEN_MASK_MAIN);
}
