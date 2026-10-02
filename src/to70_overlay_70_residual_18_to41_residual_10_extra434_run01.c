#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 pad[2],a,b,c,d;u8 pad2[0x20-12];int height;} Inner;
typedef struct {u8 pad[0x2c];int active;u8 pad2[0xb8a-0x30];u16 species;s8 selected;u8 pad3[0xba0-0xb8d];void *msgA,*msgB;u8 pad4[0x1068-0xba8];u8 windowA[16];u8 pad5[16];u8 windowB[16];u8 pad6[0x11a8-0x1098];void *input;u8 pad7[0x11c4-0x11ac];Inner *inner;u8 pad8[0x1234-0x11c8];u8 text[16];} State;
extern int ov70_02242144(void *),GetMonBaseStat(int,int),ov70_0223EDE4(u16 *,int);extern void ov70_0223CD28(int),ov70_0223F960(void *,int,int,int),FillWindowPixelBuffer(void *,int),ov70_0223F1D8(void *,void *,int,int,int,u32),ov70_0223F2BC(void *,void *,int,int,int,int,u32);
int ov70_0223C52C(State *);
int ov70_0223C52C(State *s){int selection=ov70_02242144(s->input);switch(selection){case -2:ov70_0223CD28(0);s->active=0;ov70_0223F960(s->text,s->inner->b+s->inner->a,s->inner->d,s->inner->c);break;case -1:break;default:ov70_0223CD28(0);s->species=selection;s->active=0;FillWindowPixelBuffer(s->windowA,0);ov70_0223F1D8(s->windowA,s->msgB,selection,0,0,0x10200);s->inner->height=GetMonBaseStat(selection,18);ov70_0223F960(s->text,s->inner->b+s->inner->a,s->inner->d,s->inner->c);if(ov70_0223EDE4(&s->species,s->inner->height)){FillWindowPixelBuffer(s->windowB,0);ov70_0223F2BC(s->windowB,s->msgA,s->selected,1,0,0,0x10200);}break;}return 3;}
