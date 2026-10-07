#pragma force_active on
#include <nitro/types.h>
#include <nitro/fx/fx.h>
#include <nitro/gx/gxcommon.h>
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
typedef struct SPLResTexAnm {
    u8 tex_no[8];
    struct {
        u32 use_num : 8;
        u32 diff : 8;
        u32 use_rndm : 1;
        u32 loop : 1;
        u32 reserved0 : 14;
    } etc;
} SPLResTexAnm;
typedef struct SPLResource {
    void *p_base;
    void *p_scl_anm;
    void *p_clr_anm;
    void *p_alp_anm;
    SPLResTexAnm *p_tex_anm;
    void *p_chld;
    void *fld_ary;
    u16 fld_num;
    u16 reserved;
} SPLResource;

#include <nitro/code32.h>
void spl_tex_ptn_anm(SPLParticle *p, SPLResource *res, int t);
// @fn spl_tex_ptn_anm
void spl_tex_ptn_anm(SPLParticle *p, SPLResource *res, int t) {
    SPLResTexAnm *anm = res->p_tex_anm;
    int i;
    for (i = 0; i < anm->etc.use_num; i++) {
        if (t < anm->etc.diff * (i + 1)) {
            p->etc.tex_no = anm->tex_no[i];
            return;
        }
    }
}
