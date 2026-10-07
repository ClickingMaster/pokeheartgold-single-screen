#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
typedef struct W31686 {u8 a[0x66c];u32 choice;u8 b[0x180];u32 available[5];} W31686;
extern u8 ov15_02200528[],ov15_02200529[],ov15_0220052A[],ov15_0220052B[],ov15_02201468[];
extern void ov15_021FFECC(W31686 *,u32),PlaySE(int),ov15_021FD774(W31686 *,int);
extern int ov15_021FAC2C(W31686 *,int);
int ov15_021FB3F0(W31686 *);
int ov15_021FB3F0(W31686 *p){u32 old=p->choice,next=old;int selected;if(gSystem.newKeys&64)next=ov15_02200528[old*4];else if(gSystem.newKeys&128)next=ov15_02200529[old*4];else if(gSystem.newKeys&32)next=ov15_0220052A[old*4];else if(gSystem.newKeys&16)next=ov15_0220052B[old*4];if(next!=old){p->choice=next;ov15_021FFECC(p,ov15_02201468[next]);PlaySE(1500);return -1;}selected=ov15_021FAC2C(p,1);if(selected!=-1){ov15_021FD774(p,1);p->choice=selected;ov15_021FFECC(p,ov15_02201468[selected]);if(selected==4){PlaySE(2368);return -2;}if(p->available[selected]){PlaySE(1500);return selected;}}else if(gSystem.newKeys&1){if(p->available[next]){PlaySE(1500);ov15_021FD774(p,0);return next;}if(next==4){PlaySE(1500);return -2;}}else if(gSystem.newKeys&2){PlaySE(2368);return -2;}return -1;}
