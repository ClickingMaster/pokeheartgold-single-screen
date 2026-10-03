#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {s32 level;s16 coefficient;u16 mode:2,padmode:14;} Field;
typedef struct {u8 pad0[12];s32 motion;u8 pad10[8];s32 velocity;u8 pad1c[8];u16 current,saved;u8 pad28[20];s32 position;} Particle;
typedef struct {u8 pad0[0x74];s32 level;} System;
void spl_calc_scfield(Field*,Particle*,void*,System*);
static inline void reflect(Field *field,Particle *p,s32 level,s32 position) {
 p->motion=level-position;p->velocity=-(s32)(((s64)p->velocity*field->coefficient+0x800)>>12);
}
void spl_calc_scfield(Field *field,Particle *p,void *unused,System *system) {
 s32 level=field->level;
 if(system->level!=(s32)0x80000000)level=system->level;
 switch(field->mode) {
 case 0:
  {s32 position=p->position;
   if(position<level) {if(position+p->motion>level) {p->motion=level-position;p->saved=p->current;return;}}
   if(position>=level) {if(position+p->motion<level) {p->motion=level-position;p->saved=p->current;}}
  }break;
 case 1:
  {s32 position=p->position;
   if(position<level && position+p->motion>level) {reflect(field,p,level,position);return;}
   if(position>=level && position+p->motion<level)reflect(field,p,level,position);
  }break;
 }
}
