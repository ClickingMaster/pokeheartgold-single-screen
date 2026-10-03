#pragma force_active on
#include "global.h"
typedef void(*Fn)(void);
typedef struct{Fn f[6];u8 pad[0x17c];u8 flag,side,mode;}W;
extern u32 BattleSystem_GetBattleType(void*);extern u32 BattleSystem_GetBattleSpecial(void*);extern int ov12_0223B688(void*);extern int ov12_0223BFC0(void*);extern int ov12_0223AB0C(void*,int);
extern void ov12_0225DAD4(void);
extern void ov12_0225E104(void);
extern void ov12_0225E134(void);
extern void ov12_0225E154(void);
extern void ov12_0225E1D4(void);
extern void ov12_0225E1FC(void);
extern void ov12_0225E250(void);
extern void ov12_0225E404(void);
extern void ov12_0225E4CC(void);
extern void ov12_0225E4EC(void);
extern void ov12_0225E568(void);
extern void ov12_0225E6FC(void);
extern void ov12_0225E740(void);
extern void ov12_0225E760(void);
extern void ov12_0225E830(void);
extern void ov12_0225F3A4(void);
extern void ov12_0225F3FC(void);
extern void ov12_0225F434(void);
extern void ov12_0225F4E0(void);
extern void ov12_0225F8AC(void);
extern void ov12_0225F960(void);
extern void ov12_0225F980(void);
extern void ov12_0225FA44(void);
extern void ov12_0225FC80(void);
extern void ov12_0225FCA0(void);
extern void ov12_0225FCC0(void);
#define G0() w->f[0]=ov12_0225E104;w->f[1]=ov12_0225E404;w->f[2]=ov12_0225E6FC;w->f[3]=ov12_0225F3A4;w->f[4]=ov12_0225F8AC;w->f[5]=ov12_0225FC80
#define G1() w->f[0]=ov12_0225E1FC;w->f[1]=ov12_0225E4EC;w->f[2]=ov12_0225E760;w->f[3]=ov12_0225F434;w->f[4]=ov12_0225F980;w->f[5]=ov12_0225FCC0
#define G2() w->f[0]=ov12_0225DAD4;w->f[1]=ov12_0225E250;w->f[2]=ov12_0225E568;w->f[3]=ov12_0225E830;w->f[4]=ov12_0225F4E0;w->f[5]=ov12_0225FA44
#define G3() w->f[0]=ov12_0225E134;w->f[1]=ov12_0225E4CC;w->f[2]=ov12_0225E740;w->f[3]=ov12_0225F3FC;w->f[4]=ov12_0225F960;w->f[5]=ov12_0225FCA0
void ov12_02260EA4(void*,W*);
void ov12_02260EA4(void*bs,W*w){u32 type=BattleSystem_GetBattleType(bs);u32 special=BattleSystem_GetBattleSpecial(bs);
 if(type&64){if(w->flag){G0();w->mode=1;}else if(special&16){G1();w->mode=0;}else{G2();w->mode=0;}return;}
 if((type&136)==136){if(w->side&1){if(ov12_0223B688(bs)){G0();w->mode=1;}else{G3();w->mode=2;}return;}if(special&16){G1();w->mode=0;return;}if(w->side!=ov12_0223AB0C(bs,ov12_0223BFC0(bs)*2)){G3();w->mode=2;}else{G2();w->mode=0;}return;}
 if(type&8){if(special&16){G1();w->mode=0;return;}if(w->side!=ov12_0223AB0C(bs,ov12_0223BFC0(bs))){G3();w->mode=2;}else{G2();w->mode=0;}return;}
 if(type&4){if(special&16){G1();w->mode=0;return;}if(w->side&1){G3();w->mode=2;}else{G2();w->mode=0;}return;}
 if(type&512){if(w->side&1){w->f[0]=ov12_0225E1D4;w->f[1]=0;w->f[2]=0;w->f[3]=0;w->f[4]=0;w->f[5]=0;w->mode=1;}else{w->f[0]=ov12_0225DAD4;w->f[1]=0;w->f[2]=0;w->f[3]=0;w->f[4]=0;w->f[5]=0;w->mode=0;}return;}
 if(type&32){if(w->side&1){w->f[0]=ov12_0225E154;w->f[1]=0;w->f[2]=0;w->f[3]=0;w->f[4]=0;w->f[5]=0;w->mode=1;}else{w->f[0]=ov12_0225DAD4;w->f[1]=0;w->f[2]=0;w->f[3]=0;w->f[4]=0;w->f[5]=ov12_0225FA44;w->mode=0;}return;}
 if(w->side&1){G0();w->mode=1;}else if(special&16){G1();w->mode=0;}else{G2();w->mode=0;}}
