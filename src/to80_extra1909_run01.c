#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int WM_SetWEPKey(void(*)(void*),int,void*);
typedef struct{u8 pad[0x13ac];int(*fn)(void*,void*);u8 pad2[16];u8 key[80];} S;extern struct{u32 a;S*s;}ov13_0224F5C0;extern void ov13_0223D6A8(int),ov13_0223D724(int),ov13_0223D858(void*);int ov13_0223D7F4(void);
#pragma force_active on
int ov13_0223D7F4(void){int key,result;int(*fn)(void*,void*);S*s;ov13_0223D6A8(3);s=ov13_0224F5C0.s;fn=s->fn;key=fn(s->key,s);result=WM_SetWEPKey(ov13_0223D858,key,ov13_0224F5C0.s->key);if(result==2)return 1;ov13_0223D724(result);ov13_0223D6A8(9);return 0;}
