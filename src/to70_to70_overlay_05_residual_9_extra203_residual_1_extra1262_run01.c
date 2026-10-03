#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x130];void*options;}Data;typedef struct{Data*data;u8 p[32];int heap;}Parent;typedef struct{Parent*parent;u32 p;void*palette,*bg;u8 q[0xb88-16];u8 window1[16],window2[16];u32 flag;}State;
extern void GF_AssertFail(void),LoadUserFrameGfx2(void*,int,int,int,u8,int),LoadUserFrameGfx1(void*,int,int,int,u8,int),PaletteData_LoadPaletteSlotFromHardware(void*,int,int,int),LoadFontPal0(int,int,int),AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16);extern int Options_GetFrame(void*);
void ov05_0221D140(State*);
void ov05_0221D140(State*s){u8 frame;if(s->flag||!s->parent||!s->parent->data||!s->parent->data->options)GF_AssertFail();frame=Options_GetFrame(s->parent->data->options);LoadUserFrameGfx2(s->bg,0,1,15,frame,s->parent->heap);PaletteData_LoadPaletteSlotFromHardware(s->palette,0,240,32);LoadUserFrameGfx1(s->bg,0,31,14,0,s->parent->heap);PaletteData_LoadPaletteSlotFromHardware(s->palette,0,224,32);LoadFontPal0(0,0x1a0,s->parent->heap);PaletteData_LoadPaletteSlotFromHardware(s->palette,0,208,32);AddWindowParameterized(s->bg,s->window1,0,2,19,27,4,13,40);AddWindowParameterized(s->bg,s->window2,0,2,19,27,4,13,40);}
