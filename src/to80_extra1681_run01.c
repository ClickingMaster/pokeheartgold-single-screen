#pragma force_active on
#define NITRO_CARD_BACKUP_BACKUP_H_
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "library/spl_emitter.h"
#pragma force_active on
void spl_gen_ptcl(SPLEmitter *, SPLParticleList *);
extern void VEC_Fx16CrossProduct(const VecFx16 *, const VecFx16 *, VecFx16 *);
extern u32 spl_rndm_seed;
extern SPLParticle *spl_pop_front(SPLParticleList *);
extern void spl_push_front(SPLParticleList *, SPLParticle *);
extern void spl_set_cross_to_axis(SPLEmitter *);
extern void spl_set_circle_axis(VecFx32 *, const VecFx32 *, const SPLEmitter *);
extern void spl_rndm_get_arb_vec_xyz(VecFx32 *);
extern void spl_rndm_get_arb_vec_xy(VecFx32 *);
#define RAND() (spl_rndm_seed = spl_rndm_seed * 0x5EEDF715 + 0x1B0CB173)
#define RND_SIGN() (((int)((RAND() >> 23) << 12) - 0x100000) >> 8)
#define RND_HALF() ((((int)((RAND() >> 23) << 12) - 0x100000) >> 9) + 0x800)
#define MUL(a,b) FX_Mul(a,b)
#define RANDOM_MAG(v,r) (((int)((r) + 255 - (((int)(RAND() >> 24) * (int)(r)) >> 7)) * (v)) >> 8)
void spl_gen_ptcl(SPLEmitter *emitter, SPLParticleList *freeList) {
    SPLResource *resource = emitter->p_res;
    SPLResBase *base = resource->p_base;
    SPLParticle *particle;
    int sum = emitter->gen_num + emitter->gen_num_dec;
    int i;
    int count = sum >> 12;
    fx32 speed1, speed2;
    int angle;
    VecFx16 cross8, cross9;
    u16 colors[3];
    VecFx32 circle2, circle3, circle5, vector8, vector9, circle6, circle7, normal, combined;
    emitter->gen_num_dec = sum & 0xfff;
    if (base->flag.init_pos_type == 2 || base->flag.init_pos_type == 3 || (u32)(base->flag.init_pos_type - 5) <= 4)
        spl_set_cross_to_axis(emitter);
    i = 0;
    if (count <= 0) return;
    angle = 0;
    do {
        particle = spl_pop_front(freeList);
        if (!particle) return;
        spl_push_front(&emitter->act_ptcl_list, particle);
        switch (base->flag.init_pos_type) {
        case 0: particle->ptcl_pos.x = particle->ptcl_pos.y = particle->ptcl_pos.z = 0; break;
        case 1:
            spl_rndm_get_arb_vec_xyz(&particle->ptcl_pos);
            particle->ptcl_pos.x = MUL(particle->ptcl_pos.x, emitter->radius);
            particle->ptcl_pos.y = MUL(particle->ptcl_pos.y, emitter->radius);
            particle->ptcl_pos.z = MUL(particle->ptcl_pos.z, emitter->radius);
            break;
        case 2:
            spl_rndm_get_arb_vec_xy(&circle2);
            circle2.x = MUL(circle2.x, emitter->radius);
            circle2.y = MUL(circle2.y, emitter->radius);
            circle2.z = 0;
            spl_set_circle_axis(&particle->ptcl_pos, &circle2, emitter);
            break;
        case 3: {
            int index = angle / count;
            fx16 sine, cosine;
            angle += 0x10000;
            index >>= 4;
            sine = FX_SinCosTable_[index * 2];
            cosine = FX_SinCosTable_[index * 2 + 1];
            circle3.x = MUL(sine, emitter->radius);
            circle3.y = MUL(cosine, emitter->radius);
            circle3.z = 0;
            spl_set_circle_axis(&particle->ptcl_pos, &circle3, emitter);
            break;
        }
        case 4:
            spl_rndm_get_arb_vec_xyz(&particle->ptcl_pos);
            particle->ptcl_pos.x = MUL(MUL(particle->ptcl_pos.x, emitter->radius), RND_SIGN());
            particle->ptcl_pos.y = MUL(MUL(particle->ptcl_pos.y, emitter->radius), RND_SIGN());
            particle->ptcl_pos.z = MUL(MUL(particle->ptcl_pos.z, emitter->radius), RND_SIGN());
            break;
        case 5:
            spl_rndm_get_arb_vec_xy(&circle5);
            { fx32 factor = RND_SIGN(); circle5.x = MUL(MUL(circle5.x, emitter->radius), factor); }
            { fx32 factor = RND_SIGN(); circle5.y = MUL(MUL(circle5.y, emitter->radius), factor); }
            spl_set_circle_axis(&particle->ptcl_pos, &circle5, emitter);
            break;
        case 8:
            spl_rndm_get_arb_vec_xyz(&particle->ptcl_pos);
            VEC_Fx16CrossProduct(&emitter->cross_to_axis_1, &emitter->cross_to_axis_2, &cross8);
            vector8.x = cross8.x; vector8.y = cross8.y; vector8.z = cross8.z;
            if (VEC_DotProduct(&vector8, &particle->ptcl_pos) <= 0) {
                particle->ptcl_pos.x = -particle->ptcl_pos.x;
                particle->ptcl_pos.y = -particle->ptcl_pos.y;
                particle->ptcl_pos.z = -particle->ptcl_pos.z;
            }
            particle->ptcl_pos.x = MUL(particle->ptcl_pos.x, emitter->radius);
            particle->ptcl_pos.y = MUL(particle->ptcl_pos.y, emitter->radius);
            particle->ptcl_pos.z = MUL(particle->ptcl_pos.z, emitter->radius);
            break;
        case 9:
            spl_rndm_get_arb_vec_xyz(&particle->ptcl_pos);
            VEC_Fx16CrossProduct(&emitter->cross_to_axis_1, &emitter->cross_to_axis_2, &cross9);
            vector9.x = cross9.x; vector9.y = cross9.y; vector9.z = cross9.z;
            if (VEC_DotProduct(&vector9, &particle->ptcl_pos) < 0) {
                particle->ptcl_pos.x = -particle->ptcl_pos.x;
                particle->ptcl_pos.y = -particle->ptcl_pos.y;
                particle->ptcl_pos.z = -particle->ptcl_pos.z;
            }
            { fx32 factor = RND_HALF(); particle->ptcl_pos.x = MUL(MUL(particle->ptcl_pos.x, emitter->radius), factor); }
            { fx32 factor = RND_HALF(); particle->ptcl_pos.y = MUL(MUL(particle->ptcl_pos.y, emitter->radius), factor); }
            { fx32 factor = RND_HALF(); particle->ptcl_pos.z = MUL(MUL(particle->ptcl_pos.z, emitter->radius), factor); }
            break;
        case 6:
            spl_rndm_get_arb_vec_xy(&particle->vel);
            circle6.x = MUL(particle->vel.x, emitter->radius);
            circle6.y = MUL(particle->vel.y, emitter->radius);
            circle6.z = ((int)(emitter->length * (int)(RAND() >> 23)) - (emitter->length << 8)) >> 8;
            spl_set_circle_axis(&particle->ptcl_pos, &circle6, emitter);
            break;
        case 7:
            spl_rndm_get_arb_vec_xy(&particle->vel);
            circle7.x = MUL(MUL(particle->vel.x, emitter->radius), RND_SIGN());
            circle7.y = MUL(MUL(particle->vel.y, emitter->radius), RND_SIGN());
            circle7.z = ((int)(emitter->length * (int)(RAND() >> 23)) - (emitter->length << 8)) >> 8;
            spl_set_circle_axis(&particle->ptcl_pos, &circle7, emitter);
            break;
        }
        speed1 = RANDOM_MAG(emitter->init_vel_mag_pos, base->rndm.init_vel_mag);
        speed2 = RANDOM_MAG(emitter->init_vel_mag_axis, base->rndm.init_vel_mag);
        if (base->flag.init_pos_type == 6) {
            combined.x = MUL(particle->vel.x, emitter->cross_to_axis_1.x) + MUL(particle->vel.y, emitter->cross_to_axis_2.x);
            combined.y = MUL(particle->vel.x, emitter->cross_to_axis_1.y) + MUL(particle->vel.y, emitter->cross_to_axis_2.y);
            combined.z = MUL(particle->vel.x, emitter->cross_to_axis_1.z) + MUL(particle->vel.y, emitter->cross_to_axis_2.z);
            VEC_Normalize(&combined, &normal);
        } else if (particle->ptcl_pos.x == 0 && particle->ptcl_pos.y == 0 && particle->ptcl_pos.z == 0)
            spl_rndm_get_arb_vec_xyz(&normal);
        else VEC_Normalize(&particle->ptcl_pos, &normal);
        particle->vel.x = emitter->emtr_init_vel.x + (MUL(normal.x, speed1) + MUL(emitter->axis.x, speed2));
        particle->vel.y = emitter->emtr_init_vel.y + (MUL(normal.y, speed1) + MUL(emitter->axis.y, speed2));
        particle->vel.z = emitter->emtr_init_vel.z + (MUL(normal.z, speed1) + MUL(emitter->axis.z, speed2));
        particle->emtr_pos = emitter->emtr_pos;
        particle->base_scl = RANDOM_MAG(emitter->base_scl, base->rndm.base_scl);
        particle->anm_scl = 0x1000;
        if (base->flag.use_clr_anm && resource->p_clr_anm->etc.use_rndm) {
            u32 index = (RAND() >> 20) % 3;
            colors[0] = resource->p_clr_anm->clr_s;
            colors[1] = base->clr_n;
            colors[2] = resource->p_clr_anm->clr_e;
            particle->clr = colors[index];
        } else particle->clr = base->clr_n;
        particle->alp.base_alp = emitter->etc.base_alp;
        particle->alp.anm_alp = 31;
        if (base->flag.use_init_rtt_rndm) particle->rtt = RAND();
        else particle->rtt = emitter->init_rtt;
        if (base->flag.use_rtt_anm) particle->rtt_vel = (u32)((base->rtt_min << 12) + (base->rtt_max - base->rtt_min) * (RAND() >> 20)) >> 12;
        else particle->rtt_vel = 0;
        particle->life = (((255 - (((int)(RAND() >> 24) * (int)base->rndm.ptcl_life) >> 8)) * emitter->ptcl_life) >> 8) + 1;
        particle->age = 0;
        if (base->flag.use_tex_anm && resource->p_tex_anm->etc.use_rndm) {
            u32 index = (RAND() >> 20) % resource->p_tex_anm->etc.use_num;
            particle->etc.tex_no = resource->p_tex_anm->tex_no[index];
        } else if (base->flag.use_tex_anm && !resource->p_tex_anm->etc.use_rndm)
            particle->etc.tex_no = resource->p_tex_anm->tex_no[0];
        else particle->etc.tex_no = base->etc.tex_no;
        particle->life_rate_diff_loop = 65535 / (int)resource->p_base->etc.loop_frame;
        particle->life_rate_diff_noloop = 65535 / particle->life;
        particle->etc.life_rate_offset = 0;
        if (base->flag.ptcl_random_loop_anm) particle->etc.life_rate_offset = RAND() >> 24;
        i++;
    } while (i < count);
}
