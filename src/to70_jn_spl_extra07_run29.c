#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Node {struct Node *next,*prev;} Node;
typedef struct List {Node *head;u32 count;Node *tail;} List;
typedef struct Vec {s32 x,y,z;} Vec;typedef struct V16 {s16 x,y,z;} V16;
typedef struct Particle {Node node;Vec pos,vel;u32 a;u16 age,time;u32 b;u8 tex,pad;struct {u16 a:5,b:5,c:6;} alpha;u32 c;s16 scl;u16 clr;u32 d;Vec old;} Particle;
typedef struct Res {u8 *data;u8 *scl,*clr,*alp,*tex,*child;} Res;
typedef struct Mode {u16 mode:2,rest:14;} Mode;
typedef struct Force {Vec target;s16 power;u16 pad;} Force;
extern u32 spl_rndm_seed;extern void VEC_Normalize(Vec*,Vec*),VEC_Fx16CrossProduct(V16*,V16*,V16*),VEC_Fx16Normalize(V16*,V16*);
extern s16 FX_SinCosTable_[];extern void MTX_RotX33_(s32*,s32,s32),MTX_RotY33_(s32*,s32,s32),MTX_RotZ33_(s32*,s32,s32),MTX_MultVec33(Vec*,s32*,Vec*);
#define FXM(a,b) ((s32)(((s64)(a)*(b)+0x800)>>12))

void rotTypeY(s32 s,s32 c,s32*m);
void rotTypeXYZ(s32 s,s32 c,s32*m);
void spl_set_circle_axis(Vec*out,Vec*v,u8*em);
void spl_chld_alp_out(Particle*p,Res*r,int t);
void spl_chld_scl_out(Particle*p,Res*r,int t);
void spl_tex_ptn_anm(Particle*p,Res*r,int t);
void spl_alp_in_out(Particle*p,Res*r,int t);
void spl_scl_in_out(Particle*p,Res*r,int t);
void spl_calc_convergence(Force*f,Particle*p);
void spl_calc_scfield(u8*f,Particle*p,Vec*acc,u8*em);
void spl_calc_spin(u16*f,Particle*p);
void spl_calc_magnet(Force*f,Particle*p,Vec*acc);
void spl_calc_random(s16*f,Particle*p,Vec*acc);
void spl_calc_gravity(s16*f,Particle*p,Vec*acc);
Node *spl_del(List*l,Node*n);
Node *spl_pop_front(List*l);
void spl_push_front(List*l,Node*n);
void spl_rndm_get_arb_vec_xy(Vec*v);
void spl_rndm_get_arb_vec_xyz(Vec*v);

void spl_rndm_get_arb_vec_xyz(Vec*v){spl_rndm_seed=spl_rndm_seed*0x5eedf715+0x1b0cb173;v->x=(s32)spl_rndm_seed>>8;spl_rndm_seed=spl_rndm_seed*0x5eedf715+0x1b0cb173;v->y=(s32)spl_rndm_seed>>8;spl_rndm_seed=spl_rndm_seed*0x5eedf715+0x1b0cb173;v->z=(s32)spl_rndm_seed>>8;VEC_Normalize(v,v);}
