#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "overlay07_obj_window_work_private.h"
#pragma opt_propagation off
void *BattleAnimUtil_AllocWork(void *, int);
void BattleAnimUtil_InitCommonWork(void *, Ov07T10Work *);
void *ov07_0221C4E8(u8 *, int);
int ov07_0221FB04(u32, u32);
u16 ov07_0221C468(void *), ov07_0221C470(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
void *ov07_0221BFC0(void *);
void ov07_0221C410(void *, void *, void *);
void ov07_0223174C(void *ctx, const Ov07Init *params) {
    Ov07ObjWindowWork *p=BattleAnimUtil_AllocWork(ctx,sizeof(*p));
    u32 mode;
    BattleAnimUtil_InitCommonWork(ctx,&p->prefix);
    p->init=*params;
    p->sprites[0]=ov07_0221C4E8((u8 *)p->prefix.owner,0);
    p->sprites[1]=ov07_0221C4E8((u8 *)p->prefix.owner,1);
    p->sprites[2]=ov07_0221C4E8((u8 *)p->prefix.owner,2);
    ov07_0221FB04((u32)p->prefix.owner,2);
    ManagedSprite_SetPriority(p->sprites[0],2);
    ManagedSprite_SetPriority(p->sprites[1],2);
    mode=BattleAnimUtil_GetBattlerType(ctx,p->init.value8==0?ov07_0221C468(ctx):ov07_0221C470(ctx));
    if((u32)ov07_0221BFC0(ctx)==1) {
        ManagedSprite_SetDrawFlag(p->sprites[2],0);
        mode=ov07_0221FB04((u32)p->prefix.owner,2);
        ManagedSprite_SetPriority(p->sprites[0],mode);
        ManagedSprite_SetPriority(p->sprites[1],mode);
    } else if(mode<=1) ManagedSprite_SetDrawFlag(p->sprites[2],0);
    else if(mode-3<=1) ManagedSprite_SetPriority(p->sprites[2],3);
    else ManagedSprite_SetPriority(p->sprites[2],1);
    {
        volatile u16 *reg=(volatile u16 *)0x0400004a;
        u32 value=*reg;
        value&=~0x3f00;
        value|=0x1600;
        value|=0x2000;
        *reg=value;
        G2_SetWndOutsidePlane(27,TRUE);
        GX_SetVisibleWnd(GX_WNDMASK_OW);
    }
    ManagedSprite_SetOamMode(p->sprites[1],GX_OAM_MODE_OBJWND);
    p->init.x=0;
    p->init.y=0;
    ov07_0221C410(p->prefix.owner,(void *)ov07_022315C0,p);
}
