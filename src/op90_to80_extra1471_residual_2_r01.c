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
typedef struct SPLResBaseX { u8 pad[0x22]; GXRgb clr; } SPLResBaseX;
typedef struct SPLResClrAnm {
    GXRgb clr_s;
    GXRgb clr_e;
    u8 in, peak, out, pad;
    struct { u16 use_rndm : 1; u16 loop : 1; u16 interp : 1; u16 reserved : 13; } etc;
} SPLResClrAnm;
typedef struct Res2 { SPLResBaseX *p_base; void *p_scl_anm; SPLResClrAnm *p_clr_anm; } Res2;
void spl_clr_in_out(SPLParticle *p, Res2 *res, int t);
void c3(SPLParticle *p, Res2 *res, int t);
// @fn spl_clr_in_out
void spl_clr_in_out(SPLParticle *p, Res2 *res, int t) {
    SPLResClrAnm *anm = res->p_clr_anm;
    SPLResBaseX *base = res->p_base;
    int in = anm->in, peak = anm->peak, out = anm->out;
    if (t < in) {
        p->clr = anm->clr_s;
    } else if (t < peak) {
        GXRgb e = base->clr, s = anm->clr_s;
        int er = e & 0x1f; int sr = s & 0x1f; int eg = (e >> 5) & 0x1f; int sg = (s >> 5) & 0x1f; int eb = (e >> 10) & 0x1f; int sb = (s >> 10) & 0x1f;
        if (!anm->etc.interp) {
            p->clr = GX_RGB(er, eg, eb);
        } else {
            int d = t - in, span = peak - in;
            int db = d * (eb - sb) / span;
            int dr = d * (er - sr) / span;
            int dg = d * (eg - sg) / span;
            p->clr = GX_RGB(sr + dr, sg + dg, sb + db);
        }
    } else if (t < out) {
        GXRgb e = anm->clr_e, s = base->clr;
        int sr = s & 0x1f; int er = e & 0x1f; int sg = (s >> 5) & 0x1f; int eg = (e >> 5) & 0x1f; int sb = (s >> 10) & 0x1f; int eb = (e >> 10) & 0x1f;
        if (!anm->etc.interp) {
            p->clr = GX_RGB(er, eg, eb);
        } else {
            int d = t - peak, span = out - peak;
            int db = d * (eb - sb) / span;
            int dr = d * (er - sr) / span;
            int dg = d * (eg - sg) / span;
            p->clr = GX_RGB(sr + dr, sg + dg, sb + db);
        }
    } else {
        p->clr = anm->clr_e;
    }
}
