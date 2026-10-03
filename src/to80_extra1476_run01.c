#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {int x[6];} Matrix;
typedef struct Surface Surface;
typedef struct Renderer {u8 pad[0x74];Surface *surface;} Renderer;
struct Surface {u8 pad[0x34];int (*before)(void *,Matrix *,Surface *);void (*afterBefore)(Renderer *,Surface *,void *,Matrix *);void (*after)(Renderer *,Surface *,void *,Matrix *);void (*beforeObj)(Renderer *,Surface *,void *,int,Matrix *);void (*afterObj)(Renderer *,Surface *,void *,int,Matrix *);};
typedef struct {u16 index,pad;int mode;Renderer *renderer;int level;} State;
extern State _021D94D8;
extern Matrix _021D9668[];
extern u8 _021D9678[],_021D967C[];
typedef struct {int a,b,c,d,e,f,tx,ty;} Context;
extern Context _021109DC;
extern Matrix _021109E4;
void RndCoreCBFuncBeforeCell_(void *,void *);
void RndCoreCBFuncAfterCell_(void *,void *);
void RndCoreCBFuncAfterOBJ_(void *,void *,int);
static inline Matrix *current(void){int mode=_021D94D8.mode;int level=_021D94D8.level;Matrix *result;if(!mode)result=&_021D9668[level];else{_021109DC.tx=*(int *)(_021D9678+level*24);_021109DC.ty=*(int *)(_021D967C+level*24);result=&_021109E4;}return result;}
typedef struct {u8 pad[0x30];void *cell;int unknown;u32 transfer;int flags;int sx,sy;s16 tx,ty;u16 angle,bits;} Animation;
void DrawCellAnimationImpl_(Animation *);
void DrawCellImpl_(void *);
void NNS_G2dPushMtx(void);
void NNS_G2dPopMtx(int);
int NNS_G2dIsRndCoreFlipH(Renderer *);
int NNS_G2dIsRndCoreFlipV(Renderer *);
static inline BOOL hastransfer(u32 transfer){return (BOOL)(transfer!=0xffffffff);}
static inline void draw(Animation *a,void *cell){if(hastransfer(a->transfer)){_021109DC.b=a->transfer;DrawCellImpl_(cell);_021109DC.b=-1;}else DrawCellImpl_(cell);}
static inline void translate(int x,int y){if(NNS_G2dIsRndCoreFlipH(_021D94D8.renderer))x=-x;if(NNS_G2dIsRndCoreFlipV(_021D94D8.renderer))y=-y;NNS_G2dTranslate(x<<12,y<<12,0);}
void DrawCellAnimationImpl_(Animation *a){void *cell=a->cell;if(!a->bits){draw(a,cell);return;}NNS_G2dPushMtx();if(a->bits&8){translate(a->tx,a->ty);}if(a->bits&4){int n=a->angle>>4;NNS_G2dRotZ(FX_SinCosTable_[n*2],FX_SinCosTable_[n*2+1]);}if(a->bits&2)NNS_G2dScale(a->sx,a->sy,4096);draw(a,cell);NNS_G2dPopMtx(1);}
