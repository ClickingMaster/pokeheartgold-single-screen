#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[8];int mode;u8 pad2[12];int frame;} Options;
typedef struct {u8 pad[4];void *messages,*format;u8 window[16];u8 pad2[0x6c-0x1c];void *str;} UI;
typedef struct {u8 pad[0x98];void *names[5];u8 pad2[0xcc-0xac];Options *options;u8 pad3[0xc9c-0xd0];UI ui;u8 pad4[0xd84-0xc9c-sizeof(UI)];void *bg;u8 pad5[0xd9c-0xd88];void *palette;} State;
extern u8 ov85_021EA4FC[];
extern void LoadUserFrameGfx1(void *,int,int,int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),PaletteData_LoadNarc(void *,int,int,int,int,int,int),AddWindow(void *,void *,void *),ov85_021E730C(State *,int);extern int sub_0200E640(int);extern void *NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*String_New(int,int);
void ov85_021E705C(State *);
void ov85_021E705C(State *s){UI *u=&s->ui;int i;LoadUserFrameGfx1(s->bg,1,1,15,0,102);LoadUserFrameGfx2(s->bg,1,10,14,s->options->frame,102);PaletteData_LoadNarc(s->palette,38,sub_0200E640(s->options->frame),102,0,32,224);PaletteData_LoadNarc(s->palette,16,8,102,0,32,240);u->messages=NewMsgDataFromNarc(0,27,206,102);u->format=MessageFormat_New(102);AddWindow(s->bg,u->window,ov85_021EA4FC);u->str=String_New(256,102);for(i=0;i<5;i++)s->names[i]=String_New(8,102);ov85_021E730C(s,s->options->mode);}
