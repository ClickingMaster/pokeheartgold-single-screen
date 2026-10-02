#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x10];void *field;u8 q[0x470-0x14];struct {u8 value;u8 pad[7];} icons[7];u8 r[0x514-0x4a8];u8 flags[7],pad;u32 bit0:1,row:4;} State;
extern u8 ov27_0225CFC8[][8];extern struct {u16 a,flags;} ov27_0225CF94[];extern int FieldSystem_ShouldDrawStartMenuIcon(void *,int);
void ov27_0225C10C(State *);
void ov27_0225C10C(State *s){int i;for(i=0;i<7;i++){int kind=ov27_0225CFC8[s->row][i];if(kind!=13){if(kind<7)s->flags[i]=FieldSystem_ShouldDrawStartMenuIcon(s->field,kind);else s->flags[i]=((u32)ov27_0225CF94[kind].flags<<16)>>31;}s->icons[i].value=s->flags[i];}}
