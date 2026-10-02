#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[22];u16 base;} Inner;
typedef struct {Inner *inner;u8 pad[0x1b8-4];u8 count;u8 pad2[0x1c8-0x1b9];void *input;u16 key;} State;
extern u32 GridInputHandler_HandleInput_AllowHold(void *);extern int GridInputHandler_GetNextInput(void *),System_GetTouchNew(void),ov68_021E7B94(State *),ov68_021E7A90(State *,int),ov68_021E7AB4(State *,int);extern void PlaySE(int),ov68_021E7A18(State *,int),ov68_021E7898(State *,int);
int ov68_021E5F68(State *);
int ov68_021E5F68(State *s){u32 input;s->key=GridInputHandler_GetNextInput(s->input);input=GridInputHandler_HandleInput_AllowHold(s->input);switch(input){case 0:case 1:case 2:case 3:if(!System_GetTouchNew()){if(s->inner->base+input<s->count){PlaySE(1501);ov68_021E7A18(s,input);return ov68_021E7B94(s);}}else if(s->inner->base+input<s->count){PlaySE(1501);ov68_021E7A18(s,input);}else ov68_021E7A18(s,5);break;case 6:case (u32)-2:PlaySE(1501);ov68_021E7A18(s,5);ov68_021E7898(s,0);return ov68_021E7A90(s,14);case 7:PlaySE(1501);return ov68_021E7AB4(s,15);case 4:case 5:case (u32)-1:case (u32)-3:case (u32)-4:break;}return 1;}
