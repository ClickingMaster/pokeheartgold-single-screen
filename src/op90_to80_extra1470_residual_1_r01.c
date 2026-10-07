#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
// clang-format off
#include <nitro/types.h>
#include <nitro/fx/fx.h>
#include <nitro/fx/fx_mtx43.h>
#include <nitro/gx/gxcommon.h>
#include <nitro/code32.h>
// clang-format on
#ifndef POKEHEARTGOLD_SPL_PARTICLE_H
#define POKEHEARTGOLD_SPL_PARTICLE_H


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

#endif // POKEHEARTGOLD_SPL_PARTICLE_H
#ifndef POKEHEARTGOLD_SPL_FIELD_H
#define POKEHEARTGOLD_SPL_FIELD_H


struct SPLEmitter;

typedef struct SPLField {
    void (*p_exec)(const void *, SPLParticle *, VecFx32 *, struct SPLEmitter *p_emtr);
    const void *p_obj;
} SPLField;

#endif // POKEHEARTGOLD_SPL_FIELD_H
#ifndef POKEHEARTGOLD_SPL_RESOURCE_H
#define POKEHEARTGOLD_SPL_RESOURCE_H


#define SPL_MAX_TEX_PTN_NUM 8

typedef union {
    u32 all;
    struct {
        u32 init_pos_type : 4;
        u32 draw_type : 2;
        u32 circle_axis : 2;
        u32 use_scl_anm : 1;
        u32 use_clr_anm : 1;
        u32 use_alp_anm : 1;
        u32 use_tex_anm : 1;
        u32 use_rtt_anm : 1;
        u32 use_init_rtt_rndm : 1;
        u32 self_dest : 1;
        u32 follow_emtr : 1;
        u32 use_chld : 1;
        u32 pol_rot_axis : 2;
        u32 pol_baseplane : 1;
        u32 ptcl_random_loop_anm : 1;
        u32 draw_child_first : 1;
        u32 draw_parent : 1;
        u32 camera_offset : 1;
        u32 use_fld_grvt : 1;
        u32 use_fld_rndm : 1;
        u32 use_fld_mgnt : 1;
        u32 use_fld_spin : 1;
        u32 use_fld_scfld : 1;
        u32 use_fld_cngc : 1;
        u32 polygonID_fixed : 1;
        u32 child_polygonID_fixed : 1;
    };
} SPLResBaseFlag;

typedef union {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLResInOut;

typedef union {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 reserved : 8;
    };
} SPLResInPeakOut;

typedef struct SPLResBase {
    SPLResBaseFlag flag;
    VecFx32 pos;
    fx32 gen_num;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb clr_n;
    fx32 init_vel_mag_pos;
    fx32 init_vel_mag_axis;
    fx32 base_scl;
    fx16 aspect;
    u16 start_offset;
    s16 rtt_min;
    s16 rtt_max;
    u16 init_rtt;
    u16 reserved0;
    u16 emtr_life;
    u16 ptcl_life;
    struct {
        u32 base_scl : 8;
        u32 ptcl_life : 8;
        u32 init_vel_mag : 8;
        u32 reserved0 : 8;
    } rndm;
    struct {
        u32 gen_intvl : 8;
        u32 base_alp : 8;
        u32 air_resist : 8;
        u32 tex_no : 8;
        u32 loop_frame : 8;
        u32 dbb_scale : 16;
        u32 tex_repeat_num_s : 2;
        u32 tex_repeat_num_t : 2;
        u32 scl_anm_direct : 3;
        u32 dpol_center : 1;
        u32 reverse_tex_s : 1;
        u32 reverse_tex_t : 1;
        u32 offset_pos : 3;
        u32 reserved1 : 27;
    } etc;
    fx16 offset_x;
    fx16 offset_y;
    struct {
        u32 flag : 8;
        u32 reserved : 24;
    } usr;
} SPLResBase;

typedef struct SPLResSclAnm {
    fx16 scl_s;
    fx16 scl_n;
    fx16 scl_e;
    SPLResInOut in_out;
    struct {
        u16 loop : 1;
        u16 reserved : 15;
    } etc;
    u16 reserved0;
} SPLResSclAnm;

typedef struct SPLResClrAnm {
    GXRgb clr_s;
    GXRgb clr_e;
    SPLResInPeakOut in_peak_out;
    struct {
        u16 use_rndm : 1;
        u16 loop : 1;
        u16 interpolation : 1;
        u16 reserved0 : 13;
    } etc;
    u16 reserved0;
} SPLResClrAnm;

typedef struct SPLResAlpAnm {
    union {
        u16 all;
        struct {
            u16 s : 5;
            u16 n : 5;
            u16 e : 5;
            u16 reserved0 : 1;
        };
    } alp;
    struct {
        u16 flick : 8;
        u16 loop : 1;
        u16 reserved0 : 7;
    } etc;
    SPLResInOut in_out;
    u16 reserved0;
} SPLResAlpAnm;

typedef struct SPLResTexAnm {
    u8 tex_no[SPL_MAX_TEX_PTN_NUM];
    struct {
        u32 use_num : 8;
        u32 diff : 8;
        u32 use_rndm : 1;
        u32 loop : 1;
        u32 reserved0 : 14;
    } etc;
} SPLResTexAnm;

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

#endif // POKEHEARTGOLD_SPL_RESOURCE_H
#ifndef POKEHEARTGOLD_SPL_EMITTER_H
#define POKEHEARTGOLD_SPL_EMITTER_H



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

typedef struct SPLEmitterList {
    SPLEmitter *p_begin;
    int node_num;
    SPLEmitter *p_end;
} SPLEmitterList;

static inline void SPL_SetEmitterPosition(SPLEmitter *p_emtr, const VecFx32 *p_pos) {
    p_emtr->emtr_pos.x = p_pos->x + p_emtr->p_res->p_base->pos.x;
    p_emtr->emtr_pos.y = p_pos->y + p_emtr->p_res->p_base->pos.y;
    p_emtr->emtr_pos.z = p_pos->z + p_emtr->p_res->p_base->pos.z;
}

static inline void SPL_SetEmitterPositionX(SPLEmitter *p_emtr, fx32 x) {
    p_emtr->emtr_pos.x = x + p_emtr->p_res->p_base->pos.x;
}

static inline void SPL_SetEmitterPositionY(SPLEmitter *p_emtr, fx32 y) {
    p_emtr->emtr_pos.y = y + p_emtr->p_res->p_base->pos.y;
}

static inline void SPL_SetEmitterPositionZ(SPLEmitter *p_emtr, fx32 z) {
    p_emtr->emtr_pos.z = z + p_emtr->p_res->p_base->pos.z;
}

static inline void SPL_SetEmitterAxis(SPLEmitter *p_emtr, const VecFx16 *axis) {
    p_emtr->axis = *axis;
}

#endif // POKEHEARTGOLD_SPL_EMITTER_H
#ifndef POKEHEARTGOLD_SPL_TEXTURE_H
#define POKEHEARTGOLD_SPL_TEXTURE_H


typedef union {
    u32 all;
    struct {
        u32 fmt : 4;
        u32 s : 4;
        u32 t : 4;
        u32 rep : 2;
        u32 flp : 2;
        u32 plt0 : 1;
        u32 overlaped : 1;
        u32 sharedTexNo : 8;
        u32 reserved0 : 6;
    };
} SPLTexParam;

typedef struct SPLTexture {
    const void *p_obj;
    u32 tex_addr;
    u32 plt_addr;
    SPLTexParam tex_param;
    u16 wdt;
    u16 hgt;
} SPLTexture;

#endif // POKEHEARTGOLD_SPL_TEXTURE_H
#ifndef POKEHEARTGOLD_SPL_MANAGER_H
#define POKEHEARTGOLD_SPL_MANAGER_H


typedef void *(*SPLAlloc)(u32);

typedef struct SPLManager {
    SPLAlloc alloc;
    SPLEmitterList act_emtr_list;
    SPLEmitterList ina_emtr_list;
    SPLParticleList ina_ptcl_list;
    SPLResource *res_ary;
    SPLTexture *tex_ary;
    u16 res_num;
    u16 tex_num;
    u16 max_emtr_num;
    u16 max_ptcl_num;
    struct {
        u32 min : 6;
        u32 max : 6;
        u32 current : 6;
        u32 fix : 6;
        u32 drawOrder : 1;
        u32 reserved : 7;
    } polygonID;
    s32 misc_flag;
    struct {
        SPLEmitter *p_emtr;
        const MtxFx43 *p_cmr;
    } current_state;
    u16 uCurrentCycle;
    u16 reserved;
} SPLManager;

#endif // POKEHEARTGOLD_SPL_MANAGER_H
typedef char ParticleSizeCheck[(sizeof(SPLParticle)==0x44)?1:-1];
typedef char ChildSizeCheck[(sizeof(SPLResChld)==0x14)?1:-1];
typedef char ResourceSizeCheck[(sizeof(SPLResource)==0x20)?1:-1];
typedef char EmitterSizeCheck[(sizeof(SPLEmitter)==0x9c)?1:-1];
#pragma force_active on
typedef void (*Anim)(SPLParticle *,SPLResource *,int);
typedef struct {Anim fn;int phase;} Animation;
extern void spl_scl_in_out(SPLParticle *,SPLResource *,int),spl_clr_in_out(SPLParticle *,SPLResource *,int),spl_alp_in_out(SPLParticle *,SPLResource *,int),spl_tex_ptn_anm(SPLParticle *,SPLResource *,int),spl_chld_scl_out(SPLParticle *,SPLResource *,int),spl_chld_alp_out(SPLParticle *,SPLResource *,int);
extern void spl_gen_ptcl(SPLEmitter *,SPLParticleList *),spl_gen_child(SPLParticle *,SPLEmitter *,SPLParticleList *),spl_push_front(SPLParticleList *,SPLParticle *);
extern SPLParticle *spl_del(SPLParticleList *,SPLParticle *);
void spl_calc(SPLManager *,SPLEmitter *);
static inline void AdvanceParticleMotion(SPLParticle *p,SPLEmitter *e,int resist,const VecFx32 *acc){(p)->rtt+=(p)->rtt_vel;(p)->vel.x=(s32)((u32)(p)->vel.x*(u32)resist)>>9;(p)->vel.y=(s32)((u32)(p)->vel.y*(u32)resist)>>9;(p)->vel.z=(s32)((u32)(p)->vel.z*(u32)resist)>>9;(p)->vel.x=(s32)((u32)(p)->vel.x+(u32)acc->x);(p)->vel.y=(s32)((u32)(p)->vel.y+(u32)acc->y);(p)->vel.z=(s32)((u32)(p)->vel.z+(u32)acc->z);(p)->ptcl_pos.x=(s32)((u32)(p)->ptcl_pos.x+((u32)(p)->vel.x+(u32)e->emtr_vel.x));(p)->ptcl_pos.y=(s32)((u32)(p)->ptcl_pos.y+((u32)(p)->vel.y+(u32)e->emtr_vel.y));(p)->ptcl_pos.z=(s32)((u32)(p)->ptcl_pos.z+((u32)(p)->vel.z+(u32)e->emtr_vel.z));}

#define POLYGON(p,childflag) if(e->p_res->p_base->flag.childflag)(p)->alp.current_polygonID=(u16)m->polygonID.fix;else{(p)->alp.current_polygonID=(u16)m->polygonID.current;m->polygonID.current++;if(m->polygonID.current>m->polygonID.max)m->polygonID.current=m->polygonID.min;}
static inline int BuildChildAnimations(const SPLResChld*child,Animation*program){int count=0;if(child->flag.use_scl_anm){program[count].fn=spl_chld_scl_out;program[count].phase=0;count++;}if(child->flag.use_alp_anm){program[count].fn=spl_chld_alp_out;program[count].phase=0;count++;}return count;}
void y1(SPLManager *m,SPLEmitter *e);
// @fn spl_calc
void spl_calc(SPLManager *m,SPLEmitter *e){
 int i;SPLResource *res=e->p_res;SPLParticle *p=(SPLParticle*)res->p_base,*next;SPLResChld *child=res->p_chld;SPLResBaseFlag flags=((SPLResBase*)p)->flag;int resist=((SPLResBase*)p)->etc.air_resist+0x180;int fields=res->fld_num;Animation anim[4];Animation chAnim[4];VecFx32 acc;u8 phases[2];struct Traversal {struct ParentPolicy {u32 follow,child;} parent;SPLParticle *nextChild;} traversal;
 int count=0;
 if(e->callbackfunc)e->callbackfunc(e,0);
 if((!((SPLResBase*)p)->emtr_life || e->age<((SPLResBase*)p)->emtr_life)&&e->age%e->etc.gen_intvl==0&&!e->flag.terminate&&!e->flag.stop_generate&&e->flag.started)spl_gen_ptcl(e,&m->ina_ptcl_list);
 if(flags.use_scl_anm){anim[count].fn=spl_scl_in_out;anim[count].phase=res->p_scl_anm->etc.loop;count++;}
 if(flags.use_clr_anm&&!res->p_clr_anm->etc.use_rndm){anim[count].fn=spl_clr_in_out;anim[count++].phase=res->p_clr_anm->etc.loop;}
 if(flags.use_alp_anm){anim[count].fn=spl_alp_in_out;anim[count++].phase=res->p_alp_anm->etc.loop;}
 if(flags.use_tex_anm&&!res->p_tex_anm->etc.use_rndm){anim[count].fn=spl_tex_ptn_anm;anim[count].phase=res->p_tex_anm->etc.loop;count++;}
 p=e->act_ptcl_list.p_begin;if(p){traversal.parent.follow=flags.follow_emtr;traversal.parent.child=flags.use_chld;
 do{next=p->p_next;phases[0]=(s32)((u32)p->life_rate_diff_noloop*p->age)>>8;phases[1]=p->etc.life_rate_offset+((s32)((u32)p->life_rate_diff_loop*p->age)>>8);
 {int n;for(n=0;n<count;n++)anim[n].fn(p,res,phases[anim[n].phase]);}
 acc.x=acc.y=acc.z=0;if(traversal.parent.follow)p->emtr_pos=e->emtr_pos;
 for(i=0;i<fields;i++)res->fld_ary[i].p_exec(res->fld_ary[i].p_obj,p,&acc,e);
 AdvanceParticleMotion(p,e,resist,&acc);
 if(traversal.parent.child){int remain=(p->age<<12)-((FX_Mul(p->life<<12,child->etc.gen_start<<12))>>8);if(remain>=0 && (remain>>12)%child->etc.gen_intvl==0)spl_gen_child(p,e,&m->ina_ptcl_list);}
 POLYGON(p,polygonID_fixed);
 p->age++;if(p->age>p->life)spl_push_front(&m->ina_ptcl_list,spl_del(&e->act_ptcl_list,p));
 }while((p=next)!=NULL);
 }
 if(flags.use_chld){int childCount=BuildChildAnimations(child,chAnim);SPLParticle *cp;
 if(!child->flag.affect_fld)fields=0;
 for(cp=e->act_chld_list.p_begin;cp;cp=traversal.nextChild){traversal.nextChild=cp->p_next;phases[0]=(cp->age<<8)/cp->life;
 i=0;if(childCount>0){u8 phase=phases[0];do{chAnim[i].fn(cp,res,phase);i++;}while(i<childCount);}
 acc.x=acc.y=acc.z=0;if(child->flag.follow_emtr)cp->emtr_pos=e->emtr_pos;
 for(i=0;i<fields;i++)res->fld_ary[i].p_exec(res->fld_ary[i].p_obj,cp,&acc,e);
 AdvanceParticleMotion(cp,e,resist,&acc);POLYGON(cp,child_polygonID_fixed);cp->age++;if(cp->age>cp->life)spl_push_front(&m->ina_ptcl_list,spl_del(&e->act_chld_list,cp));
 }}
 e->age++;if(e->callbackfunc)e->callbackfunc(e,1);
}
