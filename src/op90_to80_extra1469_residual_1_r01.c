#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct SPLResFlag {
    u32 p0 : 8;
    u32 has_scl : 1;
    u32 has_clr : 1;
    u32 has_alp : 1;
    u32 has_tex : 1;
    u32 p12 : 4;
    u32 has_chld : 1;
    u32 p17 : 7;
    u32 gravity : 1;
    u32 random : 1;
    u32 magnet : 1;
    u32 spin : 1;
    u32 scfield : 1;
    u32 convergence : 1;
    u32 p30 : 2;
} SPLResFlag;
typedef struct SPLResBase { SPLResFlag flag; u8 pad[0x54]; } SPLResBase;
typedef struct SPLField { void (*func)(void *, void *); const void *data; } SPLField;
typedef struct SPLResource {
    SPLResBase *p_base;
    void *p_scl_anm;
    void *p_clr_anm;
    void *p_alp_anm;
    void *p_tex_anm;
    void *p_chld;
    SPLField *fld_ary;
    u16 fld_num;
    u16 reserved;
} SPLResource;
typedef struct SPLTexParam { u32 fmt : 4, s : 4, t : 4, rest : 20; } SPLTexParam;
typedef struct SPLTexHeader { u32 magic; SPLTexParam param; u8 pad[0x14]; u32 size; } SPLTexHeader;
typedef struct SPLTexture { const SPLTexHeader *p_hdr; u32 addr; u32 pltt; SPLTexParam param; u16 width, height; } SPLTexture;
typedef struct SPLFileHeader { u32 magic; u32 ver; u16 res_num; u16 tex_num; } SPLFileHeader;
typedef struct SPLManager { void *(*alloc)(u32); u8 pad[0x24]; SPLResource *res; SPLTexture *tex; u16 res_num; u16 tex_num; } SPLManager;
extern void spl_calc_gravity(void *, void *), spl_calc_random(void *, void *), spl_calc_magnet(void *, void *), spl_calc_spin(void *, void *), spl_calc_scfield(void *, void *), spl_calc_convergence(void *, void *);
void MI_CpuFill8(void *, u8, u32);
void SPL_Load(SPLManager *mgr, const void *data);
void a1(SPLManager *mgr, const void *data);
// @fn SPL_Load
void SPL_Load(SPLManager *mgr, const void *data) {
    const u8 *p = (const u8 *)data;
    u32 offset = 0x20;
    int i;
    mgr->res_num = ((const SPLFileHeader *)data)->res_num;
    mgr->tex_num = ((const SPLFileHeader *)data)->tex_num;
    mgr->res = mgr->alloc(mgr->res_num * sizeof(SPLResource));
    MI_CpuFill8(mgr->res, 0, mgr->res_num * sizeof(SPLResource));
    for (i = 0; i < mgr->res_num; i++) {
        SPLResource *res = &mgr->res[i];
        SPLResFlag flag;
        res->p_base = (SPLResBase *)(p + offset);
        flag = res->p_base->flag;
        offset += sizeof(SPLResBase);
        if (flag.has_scl) {
            res->p_scl_anm = (void *)(p + offset);
            offset += 0xc;
        } else {
            res->p_scl_anm = NULL;
        }
        if (flag.has_clr) {
            res->p_clr_anm = (void *)(p + offset);
            offset += 0xc;
        } else {
            res->p_clr_anm = NULL;
        }
        if (flag.has_alp) {
            res->p_alp_anm = (void *)(p + offset);
            offset += 8;
        } else {
            res->p_alp_anm = NULL;
        }
        if (flag.has_tex) {
            res->p_tex_anm = (void *)(p + offset);
            offset += 0xc;
        } else {
            res->p_tex_anm = NULL;
        }
        if (flag.has_chld) {
            res->p_chld = (void *)(p + offset);
            offset += 0x14;
        } else {
            res->p_chld = NULL;
        }
        res->fld_num = flag.gravity + flag.random + flag.magnet + flag.spin + flag.scfield + flag.convergence;
        if (res->fld_num != 0) {
            SPLField *fld;
            res->fld_ary = mgr->alloc(res->fld_num * sizeof(SPLField));
            fld = res->fld_ary;
            if (flag.gravity) {
                fld->data = p + offset;
                fld->func = spl_calc_gravity;
                offset += 8;
                fld++;
            }
            if (flag.random) {
                fld->data = p + offset;
                fld->func = spl_calc_random;
                offset += 8;
                fld++;
            }
            if (flag.magnet) {
                fld->data = p + offset;
                fld->func = spl_calc_magnet;
                offset += 0x10;
                fld++;
            }
            if (flag.spin) {
                fld->data = p + offset;
                fld->func = spl_calc_spin;
                offset += 4;
                fld++;
            }
            if (flag.scfield) {
                fld->data = p + offset;
                fld->func = spl_calc_scfield;
                offset += 8;
                fld++;
            }
            if (flag.convergence) {
                fld->data = p + offset;
                fld->func = spl_calc_convergence;
                offset += 0x10;
                fld++;
            }
        } else {
            res->fld_ary = NULL;
        }
    }
    mgr->tex = mgr->alloc(mgr->tex_num * sizeof(SPLTexture));
    MI_CpuFill8(mgr->tex, 0, mgr->tex_num * sizeof(SPLTexture));
    for (i = 0; i < mgr->tex_num; i++) {
        SPLTexture *tex = &mgr->tex[i];
        const SPLTexHeader *th = (const SPLTexHeader *)(p + offset);
        tex->p_hdr = th;
        tex->width = 1 << (th->param.s + 3);
        tex->height = 1 << (th->param.t + 3);
        tex->param = th->param;
        offset += th->size;
    }
}
