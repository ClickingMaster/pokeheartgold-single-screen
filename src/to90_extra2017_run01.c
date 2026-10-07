#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 pad,b,pad8,a;} Reply;
extern int ov40_0223D5CC(void*),ov40_0222DA00(void*,void*,int,int),ov39_02227420(void*,u32,u32,u32,u32),ov39_02227D44(void*,Reply**);
extern void *ov40_0223D540(void*);
extern void ov40_02236FE0(void*),ov40_02237564(void*),ov40_02230964(void*,int),ov40_02237410(void*),ov40_022371A0(void*),ov40_0222DA84(void*,int),ov40_0223077C(void*,void*,int,int),ov40_0222DED0(void*,int),ov40_0222DFB0(void*),ov40_02230CDC(void*,int,u32,u32),ov40_0222FB28(void*,int),ov40_0222DD8C(void*,int),ov40_0222BF80(void*,int);
extern void ov40_0222D980(void*,void*,int,int,int,int,int),sub_020879E0(void*,int),sub_02087A08(void*,int,int),GfGfx_EngineBTogglePlanes(int,int),GfGfx_EngineATogglePlanes(int,int),PaletteData_BlendPalettes(void*,int,int,u8,u16),PlaySE(int),StopSE(int,int);
int ov40_02236A70(u8*);
#define WORD(p,o) (*(u32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
int ov40_02236A70(u8 *s){u8 *work=PTR(s,0x860);Reply *reply;if(!ov40_0223D5CC(s))return 0;switch(WORD(s,8)){
case 0:ov40_02236FE0(s);sub_020879E0(PTR(s,0x6f4),0);GfGfx_EngineBTogglePlanes(4,0);WORD(s,8)++;
case 1:ov40_0222D980(work+0x1a4,work+0x1a8,8,18,8,18,1);ov40_02237564(s);ov40_02230964(s,1);ov40_02237410(s);ov40_02230964(s,0);ov40_022371A0(s);GfGfx_EngineATogglePlanes(4,0);WORD(s,8)++;break;
case 2:ov40_0222DA84(work+0x1ac,1);if(ov40_0222DA00(work+0x1a4,work+0x1a8,1,0))WORD(s,8)++;PaletteData_BlendPalettes(PTR(s,0x28),3,12,(u8)WORD(work,0x1ac),(u16)WORD(s,0x58));break;
case 3:WORD(s,8)++;
case 4:ov40_0223077C(s,PTR(s,0x6f0),128,96);sub_020879E0(PTR(s,0x6f0),1);sub_02087A08(PTR(s,0x6f0),24,24);ov40_0222DED0(s,0x120);PlaySE(0x57d);WORD(s,8)++;break;
case 5:if(ov39_02227420(ov40_0223D540(s),WORD(work,0x380),WORD(work,0x384),WORD(work,0),WORD(work,0x1a0))==1)WORD(s,8)++;break;
case 6:ov40_0222DFB0(s);if(ov39_02227D44(ov40_0223D540(s),&reply)==1){StopSE(0x57d,0);ov40_02230CDC(s,2,reply->a,reply->b);WORD(s,8)++;}else{StopSE(0x57d,0);ov40_0222FB28(s,37);ov40_0222DD8C(s,1);PlaySE(0x577);WORD(s,8)++;}break;
case 7:sub_020879E0(PTR(s,0x6f0),0);sub_02087A08(PTR(s,0x6f0),0,0);WORD(s,8)++;break;
default:ov40_0222BF80(s,10);break;}return 0;}
