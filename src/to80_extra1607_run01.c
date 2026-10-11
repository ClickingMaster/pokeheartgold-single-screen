#pragma force_active on
#include "global.h"
typedef struct {void *ctx;u8 pad4[0xc];int side;u32 x14;int state;u8 pad1c[8];int first,last;}Work;
typedef struct {u8 pad0[0x50];VecFx16 pos;}Out;
typedef struct {int v[4];}Row;
typedef struct {u16 x,y,z;}Pos;
typedef struct {Row r[7];}Grid;
typedef struct {int p[6][2];}Lookup;
extern const Grid ov07_022355B0;
extern const Row ov07_022354D0;
extern const Lookup ov07_02235510;
typedef struct {int v[3];}Triple;
extern const Triple ov07_02235CE0[6][6],ov07_02235CE4[6][6],ov07_02235CE8[6][6],ov07_02235620[6][6],ov07_02235624[6][6],ov07_02235628[6][6],ov07_022357D0[6][6],ov07_02235B30[6][6],ov07_02235B34[6][6],ov07_02235B38[6][6],ov07_02235E90[6][6],ov07_02235E94[6][6],ov07_02235E98[6][6],ov07_02235980[6][6],ov07_02235984[6][6],ov07_02235988[6][6],ov07_02236040[6][6],ov07_02236044[6][6],ov07_02236048[6][6],ov07_022361F0[6][6],ov07_022361F4[6][6],ov07_022361F8[6][6];
extern int BattleAnimUtil_GetBattlerType(void *,int),BattleAnimUtil_GetBattlerSide(void *,int),ov07_0221BFC0(void *);
void ov07_02220CAC(Out *,Work *);
#define POS(xv,yv,zv) pos.x=(xv);pos.y=(yv);pos.z=(zv)
#define TABLE(x,y,z,half,neg) POS((neg)*x[first][last].v[0],y[first][last].v[0],z[first][last].v[0]/(half))
#define ROW(q) POS((q).v[0],(q).v[1],(q).v[2])
#define NEGROW(q) POS(-(q).v[0],-(q).v[1],-(q).v[2])
void ov07_02220CAC(Out *out,Work *w) {
 Grid table;Row a,b,c,d,e,f,g,base;Lookup lookup;volatile VecFx16 pos;
 int first=BattleAnimUtil_GetBattlerType(w->ctx,w->first),last=BattleAnimUtil_GetBattlerType(w->ctx,w->last),i,index;
 switch(w->state){
 case 0:POS(0,0,0);break;
 case 1:case 2:if(ov07_0221BFC0(w->ctx)==1){if(w->side==2){POS(-3336,1840,736);}else{POS(2336,-1440,736);}}else{TABLE(ov07_02235CE0,ov07_02235CE4,ov07_02235CE8,2,1);}break;
 case 4:
 if(ov07_0221BFC0(w->ctx)==1){TABLE(ov07_02235CE0,ov07_02235CE4,ov07_02235CE8,2,-1);}
 else if(BattleAnimUtil_GetBattlerSide(w->ctx,w->first)==BattleAnimUtil_GetBattlerSide(w->ctx,w->last)){TABLE(ov07_02235CE0,ov07_02235CE4,ov07_02235CE8,2,1);}
 else if(BattleAnimUtil_GetBattlerSide(w->ctx,w->first)==3){POS(3776,2112,1532);}else{POS(-4228,-2728,1532);}break;
 case 5:
 if(ov07_0221BFC0(w->ctx)==1){TABLE(ov07_02235CE0,ov07_02235CE4,ov07_02235CE8,2,-1);}
 else if(BattleAnimUtil_GetBattlerSide(w->ctx,w->first)==BattleAnimUtil_GetBattlerSide(w->ctx,w->last)){TABLE(ov07_02235CE0,ov07_02235CE4,ov07_02235CE8,2,1);}
 else if(BattleAnimUtil_GetBattlerSide(w->ctx,w->first)==3){POS(3776,2112,1532);}else{POS(-4228,-2728,1532);}break;
 case 8:case 9:if(ov07_0221BFC0(w->ctx)==1){if(w->side==2){POS(-2336,1440,736);}else{POS(2336,-1440,736);}}else{TABLE(ov07_02235620,ov07_02235624,ov07_02235628,2,1);}break;
 case 10:case 11:if(ov07_0221BFC0(w->ctx)==1){POS(-2336,1440,736);}else{TABLE(ov07_022357D0,ov07_02235624,ov07_02235628,2,1);}break;
 case 12:case 13:if(ov07_0221BFC0(w->ctx)==1){POS(-2936,1440,736);}else{TABLE(ov07_02235B30,ov07_02235B34,ov07_02235B38,1,1);}break;
 case 14:case 15:if(ov07_0221BFC0(w->ctx)==1){POS(-2336,1440,736);}else{TABLE(ov07_02235E90,ov07_02235E94,ov07_02235E98,1,1);}break;
 case 16:case 17:if(ov07_0221BFC0(w->ctx)==1){POS(-2336,1440,736);}else{TABLE(ov07_02235980,ov07_02235984,ov07_02235988,1,1);}break;
 case 18:case 19:if(ov07_0221BFC0(w->ctx)==1){POS(-2336,1440,736);}else{TABLE(ov07_02236040,ov07_02236044,ov07_02236048,1,1);}break;
 case 20:case 21:if(ov07_0221BFC0(w->ctx)==1){POS(-4336,1440,736);}else{TABLE(ov07_022361F0,ov07_022361F4,ov07_022361F8,1,1);}break;
 case 6:case 7:table=ov07_022355B0;base=ov07_022354D0;for(i=0;i<4;i++){a.v[i]=table.r[0].v[i];b.v[i]=table.r[1].v[i];c.v[i]=table.r[2].v[i];d.v[i]=table.r[3].v[i];e.v[i]=table.r[4].v[i];f.v[i]=table.r[5].v[i];g.v[i]=table.r[6].v[i];}if(ov07_0221BFC0(w->ctx)==1)for(i=0;i<4;i++){a.v[i]=base.v[i];b.v[i]=base.v[i];c.v[i]=base.v[i];d.v[i]=base.v[i];e.v[i]=base.v[i];f.v[i]=base.v[i];g.v[i]=base.v[i];}
 switch(first){
 default:case 0:ROW(a);break;
 case 1:if(a.v[3]==1){ROW(a);}else{NEGROW(a);}break;
 case 2:if(last==3){ROW(b);}else if(last==5){ROW(d);}else{ROW(c);}break;
 case 3:if(last==2){if(b.v[3]==1){ROW(b);}else{NEGROW(b);}}else if(last==5){if(c.v[3]==1){NEGROW(e);}else{ROW(e);}}else{if(f.v[3]==1){ROW(f);}else{NEGROW(f);}}break;
 case 4:if(last==3){ROW(f);}else if(last==5){ROW(g);}else{ROW(e);}break;
 case 5:if(last==3){if(b.v[3]==1){ROW(e);}else{NEGROW(e);}}else if(last==2){if(b.v[3]==1){ROW(d);}else{NEGROW(d);}}else{if(b.v[3]==1){ROW(g);}else{NEGROW(g);}}break;
 }break;
 case 3:POS(-800,1200,500);break;
 case 22:POS(-3410,-2644,0);break;
 case 25:POS(-3440,1952,0);break;
 case 26:if(BattleAnimUtil_GetBattlerSide(w->ctx,w->first)==3){POS(3776,2112,0);}else{POS(-6000,-2200,0);}break;
 case 24:lookup=ov07_02235510;index=BattleAnimUtil_GetBattlerType(w->ctx,w->first);POS(lookup.p[index][0],lookup.p[index][1],0);break;
 }
 out->pos.x=((volatile Pos *)&pos)->x;out->pos.y=((volatile Pos *)&pos)->y;out->pos.z=((volatile Pos *)&pos)->z;
}
