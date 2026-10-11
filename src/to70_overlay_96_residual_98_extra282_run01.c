#pragma force_active on
#include "global.h"
#pragma force_active on
typedef VecFx32 Vec;
void *PokeathlonCourseMonObject_GetGfx(void *);int PokeathlonCourseMonGfx_GetTopRow(void *);u8 *PokeathlonCourseMonGfx_GetInfo(void *);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(void *,int),PokeathlonCourseMonObject_SetAffineMatrix(void *,Vec *);
void PokeathlonCourseObject_GetCollisionCircle(void *,int,int,fx32 *,fx32 *,int *);

u8 *ov96_0220F378(void *,u8,u8);void PokeathlonCourse_AddMonCounter(void *,u8,u8,int,int);int ov96_0220F3B4(void *);
void ov96_0220D554(void *,int);int ov96_0220D5D0(u8 *,u8 *);void ov96_0220D630(u8 *);
#define P(w,o) (*(void **)((w)+(o)))
#define W(w,o) (*(int *)((w)+(o)))
void ov96_0220D554(void *w,int flag){void *p=PokeathlonCourseMonObject_GetGfx(w);int a=PokeathlonCourseMonGfx_GetTopRow(p),n;Vec v={0,0,0};if(*(u16 *)(PokeathlonCourseMonGfx_GetInfo(p)+4)){n=64;PokeathlonCourseMonObject_SetAffineOverwriteMode(w,1);}else{n=32;PokeathlonCourseMonObject_SetAffineOverwriteMode(w,2);}if(flag){v.x=(n/2)<<12;v.y=(a+(n-a)/2)<<12;}else{v.x=(n/2)<<12;v.y=(n-1)<<12;}PokeathlonCourseMonObject_SetAffineMatrix(w,&v);}
int ov96_0220D5D0(u8 *w,u8 *other){Vec d,a,b;int r1,r2,sum;PokeathlonCourseObject_GetCollisionCircle(P(w,4),W(w,28),W(w,32),&a.x,&a.y,&r1);PokeathlonCourseObject_GetCollisionCircle(P(other,4),W(other,28),W(other,32),&b.x,&b.y,&r2);sum=(r1+r2)<<12;a.z=0;b.z=0;VEC_Subtract(&a,&b,&d);if(VEC_Mag(&d)<=sum)return 1;return 0;}
void ov96_0220D630(u8 *w){if(ov96_0220F378(P(w,0),w[69],w[70])[2]){PokeathlonCourse_AddMonCounter(P(w,0),w[69],w[70],2,1);W(w,64)=(W(w,64)&~255)|(u8)ov96_0220F3B4(P(w,8));W(w,12)=1;}W(w,64)=(W(w,64)&0xffff00ff)|0xa00;}
