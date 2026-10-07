#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/fx/fx.h>
#include <nitro/gx/gxcommon.h>
#include <nitro/code32.h>
// clang-format on
typedef struct SPLParticle {
    struct SPLParticle *p_next;
    struct SPLParticle *p_prev;
    VecFx32 ptcl_pos;
    VecFx32 vel;
    u16 rtt;
    s16 rtt_vel;
    u16 life;
    u16 age;
    u16 life_rate_diff_loop;
    u16 life_rate_diff_noloop;
    struct {
        u16 tex_no : 8;
        u16 life_rate_offset : 8;
    } etc;
    struct {
        u16 base_alp : 5;
        u16 anm_alp : 5;
        u16 current_polygonID : 6;
    } alp;
    fx32 base_scl;
    fx16 anm_scl;
    GXRgb clr;
    VecFx32 emtr_pos;
} SPLParticle;

typedef struct SPLParticleList {
    SPLParticle *p_begin;
    int node_num;
    SPLParticle *p_end;
} SPLParticleList;

typedef struct SPLResBase SPLResBase;
typedef struct SPLResSclAnm SPLResSclAnm;
typedef struct SPLResClrAnm SPLResClrAnm;
typedef struct SPLResAlpAnm SPLResAlpAnm;
typedef struct SPLResTexAnm SPLResTexAnm;
typedef struct SPLField SPLField;
typedef union {
    u16 all;
    struct {
        u16 affect_fld : 1;
        u16 use_scl_anm : 1;
        u16 use_alp_anm : 1;
        u16 rtt_type : 2;
        u16 follow_emtr : 1;
        u16 use_chld_clr : 1;
        u16 draw_type : 2;
        u16 pol_rot_axis : 2;
        u16 pol_baseplane : 1;
        u16 reserved0 : 4;
    };
} SPLResChldFlag;

typedef struct SPLResChld {
    SPLResChldFlag flag;
    fx16 init_vel_mag_rndm;
    fx16 scl_e;
    u16 life;
    struct {
        u16 vel_ratio : 8;
        u16 scl_ratio : 8;
    } ratio;
    GXRgb clr;
    struct {
        u32 gen_num : 8;
        u32 gen_start : 8;
        u32 gen_intvl : 8;
        u32 tex_no : 8;
        u32 tex_repeat_num_s : 2;
        u32 tex_repeat_num_t : 2;
        u32 reverse_tex_s : 1;
        u32 reverse_tex_t : 1;
        u32 dpol_center : 1;
        u32 reserved0 : 25;
    } etc;
} SPLResChld;

typedef struct SPLResource {
    SPLResBase *p_base;
    SPLResSclAnm *p_scl_anm;
    SPLResClrAnm *p_clr_anm;
    SPLResAlpAnm *p_alp_anm;
    SPLResTexAnm *p_tex_anm;
    SPLResChld *p_chld;
    SPLField *fld_ary;
    u16 fld_num;
    u16 reserved;
} SPLResource;

typedef struct SPLEmitterFlag {
    union {
        u32 all;
        struct {
            u32 terminate : 1;
            u32 stop_generate : 1;
            u32 stop_calc : 1;
            u32 stop_draw : 1;
            u32 started : 1;
            u32 reserved0 : 27;
        };
    };
} SPLEmitterFlag;

typedef struct SPLEmitter {
    struct SPLEmitter *p_next;     // 0
    struct SPLEmitter *p_prev;     // 4
    SPLParticleList act_ptcl_list; // 8
    SPLParticleList act_chld_list; // 14
    SPLResource *p_res;            // 20
    SPLEmitterFlag flag;           // 24
    VecFx32 emtr_pos;              // 28
    VecFx32 emtr_vel;              // 34
    VecFx32 emtr_init_vel;
    u16 age;
    fx16 gen_num_dec;
    VecFx16 axis;
    u16 init_rtt;
    fx32 gen_num;
    fx32 radius;
    fx32 length;
    fx32 init_vel_mag_pos;
    fx32 init_vel_mag_axis;
    fx32 base_scl;
    u16 ptcl_life;
    GXRgb clr;
    fx32 simple_collision_y;
    fx16 tex_s;
    fx16 tex_t;
    fx16 chld_tex_s;
    fx16 chld_tex_t;
    struct {
        u32 gen_intvl : 8;
        u32 base_alp : 8;
        u32 calc_cycle : 3;
        u32 reserved : 13;
    } etc;
    VecFx16 cross_to_axis_1;
    VecFx16 cross_to_axis_2;
    void (*callbackfunc)(struct SPLEmitter *, unsigned int uFlag);
    void *emtr_userdata;
    union {
        u32 uw_u32;
        u16 uw_u16[2];
        u8 uw_u8[4];
    } userwork;
} SPLEmitter;

typedef char ParticleSizeCheck[(sizeof(SPLParticle)==0x44)?1:-1];
typedef char ChildSizeCheck[(sizeof(SPLResChld)==0x14)?1:-1];
typedef char ResourceSizeCheck[(sizeof(SPLResource)==0x20)?1:-1];
typedef char EmitterSizeCheck[(sizeof(SPLEmitter)==0x9c)?1:-1];
extern u32 spl_rndm_seed;
extern s32 _s32_div_f(s32,s32);
SPLParticle *spl_pop_front(SPLParticleList *list);
void spl_push_front(SPLParticleList *list,SPLParticle *particle);
void spl_gen_child(SPLParticle *parent,SPLEmitter *emitter,SPLParticleList *freeList);
#define FXM(a,b) ((s32)(((s64)(a)*(b)+0x800)>>12))
#pragma force_active on
#pragma opt_common_subs off
void spl_gen_child(SPLParticle *parent,SPLEmitter *emitter,SPLParticleList *freeList){
 SPLParticle *p;VecFx32 *outPos;
 SPLResChld *res=emitter->p_res->p_chld;int ratio=FXM(res->ratio.vel_ratio<<12,16);int i; s32 velocity,scaled;
 for(i=0;i<res->etc.gen_num;i++){
  p=spl_pop_front(freeList);if(!p)return;spl_push_front(&emitter->act_chld_list,p);
  outPos=&p->emtr_pos;{VecFx32*dst=&p->ptcl_pos;const VecFx32*src=&parent->ptcl_pos;*dst=*src;s32 x;x=parent->vel.x;scaled=FXM(x,ratio);spl_rndm_seed=spl_rndm_seed*0x5eedf715U+0x1b0cb173U;p->vel.x=scaled+((res->init_vel_mag_rndm*(int)(spl_rndm_seed>>23)-(res->init_vel_mag_rndm*256))>>8);}
  {velocity=parent->vel.y;scaled=FXM(velocity,ratio);spl_rndm_seed=spl_rndm_seed*0x5eedf715U+0x1b0cb173U;p->vel.y=scaled+((res->init_vel_mag_rndm*(int)(spl_rndm_seed>>23)-(res->init_vel_mag_rndm*256))>>8);}
  {velocity=parent->vel.z;scaled=FXM(velocity,ratio);spl_rndm_seed=spl_rndm_seed*0x5eedf715U+0x1b0cb173U;p->vel.z=scaled+((res->init_vel_mag_rndm*(int)(spl_rndm_seed>>23)-(res->init_vel_mag_rndm*256))>>8);}
  *outPos=parent->emtr_pos;
  scaled=(s32)((u32)parent->base_scl*(u32)(s32)parent->anm_scl)>>12;p->base_scl=(scaled*(res->ratio.scl_ratio+1))>>6;
  p->anm_scl=4096;
  if(res->flag.use_chld_clr)p->clr=res->clr;else p->clr=parent->clr;
  p->alp.base_alp=(u16)((parent->alp.base_alp*(parent->alp.anm_alp+1))>>5);p->alp.anm_alp=31;
  switch(res->flag.rtt_type){case 0:p->rtt=0;p->rtt_vel=0;break;case 1:p->rtt=parent->rtt;p->rtt_vel=0;break;case 2:p->rtt=parent->rtt;p->rtt_vel=parent->rtt_vel;break;}
  p->life=res->life;p->age=0;p->etc.tex_no=res->etc.tex_no;
  p->life_rate_diff_loop=_s32_div_f(65535,(int)((u32)parent->life>>1));p->life_rate_diff_noloop=_s32_div_f(65535,parent->life);p->etc.life_rate_offset=0;
 }
}
