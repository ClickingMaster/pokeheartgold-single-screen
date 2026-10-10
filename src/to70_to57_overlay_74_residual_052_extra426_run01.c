#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a,b,c,d,active;} State;
typedef struct {u8 pad[0x1650];u32 value;u8 pad2[0x1014];int ready;s32 token;s32 x,y;u8 pad3[4];int tick;} Work;
extern State ov74_0223E304;
extern void ov00_021EC11C(s32 *,s32 *);extern s32 WifiConnection_ClassifyErrorCode(u32,u32);extern void ov74_02229E60(Work *);extern int ov00_021EC724(void);extern void ov00_021EC9E0(void *);extern void ov74_02236ABC(void *);extern int ov00_021ED308(void (*)(void *));
static inline void clearTick(Work *w){w->tick=0;}
int ov74_022369D8(Work *);int ov74_02236A2C(void);void ov74_02236A54(Work *,u32 *,u32);void ov74_02236A78(Work *,u32,u32 *,u32,u32);
int ov74_02236A2C(void){u8 data[60];switch(ov00_021EC724()){case 4:ov00_021EC9E0(data);return 1;case 7:break;case 8:break;}return 0;}
void ov74_02236A54(Work *w,u32 *state,u32 value){ov74_0223E304.a=0;ov74_0223E304.d=0;w->value=value;*state=0x1012;}
void ov74_02236A78(Work *w,u32 value,u32 *state,u32 property,u32 fallback){ov74_0223E304.b=0;ov74_0223E304.c=value;w->value=property;*state=0x1013;if(!ov00_021ED308(ov74_02236ABC))*state=fallback;}
