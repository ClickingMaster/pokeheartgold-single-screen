#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x1a4];int a,b,c;} Inner;
typedef struct {u8 pad[8];int state;u8 pad2[8];void *narc;u8 pad3[0x24-0x18];void *bg,*palette;u8 pad4[0x58-0x2c];int color;u8 pad5[0x6f4-0x5c];void *icon;u8 pad6[0x860-0x6f8];Inner *inner;} State;
extern void sub_020879E0(void *,int),ov40_0222DA84(int *,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int),ov40_02236F38(State *),ov40_022368EC(State *,int),PaletteData_BlendPalettes(void *,int,int,u8,u16),GfGfx_EngineBTogglePlanes(int,int),ov40_0222BF80(State *,int),ov40_02237030(void *,int);extern int ov40_0222DA00(int *,int *,int,int);
int ov40_02236968(State *);
int ov40_02236968(State *s){Inner *i=s->inner;switch(s->state){case 0:sub_020879E0(s->icon,0);ov40_0222DA84(&i->c,1);if(ov40_0222DA00(&i->a,&i->b,1,2)){GfGfxLoader_LoadScrnDataFromOpenNarc(s->narc,79,s->bg,7,0,0,0,109);ov40_02236F38(s);ov40_022368EC(s,1);s->state++;}PaletteData_BlendPalettes(s->palette,3,12,i->c,s->color);break;case 1:ov40_0222DA84(&i->c,0);if(ov40_0222DA00(&i->a,&i->b,0,2)){GfGfx_EngineBTogglePlanes(4,1);ov40_0222BF80(s,6);ov40_02237030(s,272);}PaletteData_BlendPalettes(s->palette,3,12,i->c,s->color);break;}return 0;}
