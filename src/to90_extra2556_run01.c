#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 prefix[0x2668];int ready;s32 token,x,y;u32 reserved;int tick;} Work;
extern u32 ov74_0223E304[5];
extern int ov00_021EC11C(s32 *,s32 *);extern s32 WifiConnection_ClassifyErrorCode(s32,u32);extern void ov74_02229E60(void *,int);extern int ov00_021EC724(void);extern void ov00_021EC9E0(void *);extern void ov74_02236ABC(void *);extern int ov00_021ED308(void (*)(void *));
static inline Work* clearTick(Work *w){w->tick=0;return w;}
int ov74_022369D8(void *);int ov74_02236A2C(void);void ov74_02236A54(Work *,u32 *,u32);void ov74_02236A78(Work *,u32,u32 *,u32,u32);
int ov74_022369D8(void *data){Work *w=data;s32 pos[2];ov00_021EC11C(&pos[0],&pos[1]);w->token=WifiConnection_ClassifyErrorCode((s32)(0u-(u32)pos[0]),pos[1]);w->x=(s32)(0u-(u32)pos[0]);w->y=pos[1];w->ready=1;ov74_0223E304[4]=1;w->tick=0;ov74_02229E60(w,0);return 0x100f;}
