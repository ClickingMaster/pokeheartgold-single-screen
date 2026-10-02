#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Matrix {s32 m[6];} Matrix;
typedef struct State {u16 count,pad;int translate;u8*renderer;u32 index;} State;
extern State _021D94D8;
extern Matrix _021D9668[],_021D9968[];
extern s32 _021D9678[],_021D967C[],_021D94E8[];
extern u8 _021109DC[],_021109E4[];
extern s16 FX_SinCosTable_[];
extern void NNS_G2dPushMtx(void),NNS_G2dPopMtx(int),NNS_G2dTranslate(s32,s32,s32),NNS_G2dRotZ(s32,s32),NNS_G2dScale(s32,s32,s32);
extern int NNS_G2dIsRndCoreFlipH(void*),NNS_G2dIsRndCoreFlipV(void*);
extern void DrawCellAnimationImpl_(void*);
void NNSi_G2dIdentity(void);void RndCoreCBFuncAfterCell_(void*,void*);void RndCoreCBFuncAfterOBJ_(void*,void*,void*);void DrawNode_(u8*);
static inline Matrix* current(void){u32 i=_021D94D8.index;if(!_021D94D8.translate)return &_021D9668[i];*(volatile s32*)(_021109DC+24)=*(s32*)((u8*)_021D9678+i*24);*(volatile s32*)(_021109DC+28)=*(s32*)((u8*)_021D967C+i*24);return (Matrix*)_021109E4;}
static inline void trans(s32 x,s32 y){if(NNS_G2dIsRndCoreFlipH(_021D94D8.renderer))x=-x;if(NNS_G2dIsRndCoreFlipV(_021D94D8.renderer))y=-y;NNS_G2dTranslate(x<<12,y<<12,0);}
void NNSi_G2dIdentity(void){u32 i=_021D94D8.index;if(_021D94D8.translate){*(s32*)((u8*)_021D9678+i*24)=0;*(s32*)((u8*)_021D967C+i*24)=0;}else{Matrix*p=&_021D9668[i];p->m[0]=4096;p->m[1]=0;p->m[2]=0;p->m[3]=4096;p->m[4]=0;p->m[5]=0;_021D9968[i]=*p;}_021D94E8[i]=0;}
