#pragma force_active on
#include "global.h"
#include "math_util.h"
#include "sprite_system.h"
typedef struct Ov07R2Work Ov07R2Work;
#pragma opt_propagation off
struct Ov07R2Work {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[4];
    struct SpriteSystem *spriteSystem;
    u8 pad10[0x0C];
    ManagedSprite *sprite;
    u32 value20;
    int duration;
    u32 value28;
    u32 value2C;
    u32 enabled;
    u32 value34;
    u32 counter;
    u32 threshold;
};
void ov07_02226CB0(Ov07R2Work *);
void ov07_02226CB0(Ov07R2Work *s){s16 x,y;switch(s->state){case 0:{if(s->value34==0){GX_SetVisibleWnd(GX_WNDMASK_W0);G2_SetWnd0InsidePlane(15,TRUE);G2_SetWndOutsidePlane(31,TRUE);*(vu16*)0x04000040=128;*(vu16*)0x04000044=0xa0c0;s->threshold=LCRandom()%5+35;}else{GX_SetVisibleWnd(GX_WNDMASK_W0);G2_SetWnd0InsidePlane(15,TRUE);G2_SetWndOutsidePlane(31,TRUE);*(vu16*)0x04000040=0x8000;*(vu16*)0x04000044=0x56c0;s->threshold=LCRandom()%5+35;}break;}case 5:case 6:if(LCRandom()%2&&s->counter==0){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 7:if(s->counter!=1){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 10:case 11:if(LCRandom()%2&&s->counter==1){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 12:if(s->counter!=2){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 15:case 16:if(LCRandom()%2&&s->counter==2){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 17:if(s->counter!=3){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,4);}break;case 22:case 23:if(LCRandom()%2&&s->counter==3){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,8);}break;case 24:if(s->counter!=4){s->counter++;ManagedSprite_OffsetPositionXY(s->sprite,0,8);}break;default:if(s->state>(int)s->threshold){if((int)s->counter<20){ManagedSprite_OffsetPositionXY(s->sprite,0,4);ManagedSprite_GetPositionXY(s->sprite,&x,&y);if(y>130)ManagedSprite_SetDrawFlag(s->sprite,0);s->counter++;}else ManagedSprite_SetDrawFlag(s->sprite,0);}break;}}
