#pragma force_active on
#include "global.h"
typedef struct {u32 a;u8 indices[4];u8 vals[4];u8 pad[20];void*profiles[4];}Data;
typedef struct {void*save;u8 p[5];u8 player;u8 q;u8 mode;Data*data;}Work;
extern u8 ov90_0225886C(Work*,u32);extern void*Save_PlayerData_GetProfile(void*),*sub_02034818(u32);
u32 ov90_022588A4(Work*,u32);void*ov90_022588CC(Work*,u32);
u32 ov90_022588A4(Work*w,u32 n){int i;if(n>=4){GF_ASSERT(n<4);return 0;}i=w->data->indices[n];if(i==-1)return 0;return w->data->vals[i];}
