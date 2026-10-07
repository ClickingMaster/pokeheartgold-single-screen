#include "global.h"
#pragma force_active on
#include "overlay96_course_entity_motion_r17_private.h"
#include "r40_overlay_96_residual_29_private.h"
typedef PokeathlonCourseEntityMotionR17 Work;
typedef struct Pair {s16 x,y;} Pair;
extern const Pair ov96_0221D6D4[];

extern fx32 FX_Div(fx32,fx32);
BOOL ov96_02218934(Work*);
static inline void BuildBoundary(VecFx32 *points){VecFx32 *cursor;const Pair*source=ov96_0221D6D4;int i=0;cursor=points;for(;i<4;i++){cursor->x=(s32)((u32)(s32)source->x<<12);cursor->y=(s32)((u32)(s32)source->y<<12);source++;cursor++;}}
BOOL ov96_02218934(Work*w){VecFx32 points[4];int x,y;if(w->vec2C.x<=0||w->vec2C.y<=0)return 1;ov96_021EB06C(*(Ov96PositionObject **)w->unk04,w->vec2C.x>>12,w->vec2C.y>>12,&x,&y);if(y>=64&&y<=160){int yy=FX32_CONST(x);int xx=FX32_CONST(y);BuildBoundary(points);int first=FX_Div(FX_Mul(points[1].y-points[0].y,yy-points[0].x),points[1].x-points[0].x);first+=points[0].y;int second=FX_Div(FX_Mul(points[3].y-points[2].y,yy-points[2].x),points[3].x-points[2].x);second+=points[2].y;if(xx>=first&&xx>=second)return 0;}return 1;}
