#pragma force_active on
#include "global.h"
#pragma force_active on
void ov74_02235414(void *,void *,int,int,int,int);
void ov74_02235464(void *,int,int,int);
void ov74_0223546C(void *,int,int);
void ov74_02235568(void *,void *,int,int,int);
int ov74_02235634(void *);
void ov74_0223563C(void);
void ov74_02235690(void);
void ov74_02235728(int,int,int,int,int,int);
void *ov74_02235930(int,void *,int,int,int);
void ov74_02229084(void *);
void ov74_02228F8C(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
#define W(p,o) (*(int *)((p)+(o)))
void ov74_02228F8C(u8 *w){
 ov74_02235414(w+0x48,w+0x28,0,247,10,2);
 ov74_02235464(w+0x48,27,4,W(w,0x144));
 ov74_0223546C(w+0x48,1,1);
 ov74_02235568(P(w,4),w+0x48,2,19,72);
 W(w,0x144)+=ov74_02235634(w+0x48);
 ov74_02235414(w+0x9c,w+0x18,0,247,1,1);
 ov74_02235464(w+0x9c,30,4,W(w,0x144));
 ov74_0223546C(w+0x9c,0,1);
 ov74_02235568(P(w,4),w+0x9c,1,1,11);
 W(w,0x144)+=ov74_02235634(w+0x9c);
 ov74_0223563C();ov74_02235690();
 ov74_02235728(113,14,11,13,12,0);
 P(w,0x3170)=ov74_02235930(0,P(w,0x3170),128,76,1);
 ov74_02229084(w);
}
