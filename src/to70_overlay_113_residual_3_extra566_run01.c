#pragma force_active on
#include "global.h"
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);
extern int IsPaletteFadeFinished(void);
typedef struct { int heap; u8 pad[12]; u16 state; } W;
extern int ov113_021E5EC4(W *);
int ov113_021E5BC4(W *w);
int ov113_021E5C0C(W *w);
int ov113_021E5BC4(W *w) { switch(w->state) {case 0: BeginNormalPaletteFade(0,1,1,0,6,1,w->heap); w->state++; break;case 1: if(IsPaletteFadeFinished()) {w->state=0;return 2;}break;}return 0; }
int ov113_021E5C0C(W *w) { switch(w->state) {case 0: if(ov113_021E5EC4(w)==0) {BeginNormalPaletteFade(0,0,0,0,6,1,w->heap);w->state++;}break;case 1:if(IsPaletteFadeFinished()) {w->state=0;return 4;}break;}return 1; }
