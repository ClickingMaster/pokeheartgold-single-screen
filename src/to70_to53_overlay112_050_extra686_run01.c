#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x1e534];void *a,*b;} Work;
extern void ManagedSprite_SetAnim(void*,int);
extern void ManagedSprite_SetDrawFlag(void*,int);
extern void ManagedSprite_SetDrawPriority(void*,int);
extern void ManagedSprite_SetAnimateFlag(void*,int);
extern int ManagedSprite_GetActiveAnim(void*);
extern int ov112_021E7668(void);
void ov112_021EA5A4(Work*w,int n);
void ov112_021EA60C(Work*w);
void ov112_021EA5A4(Work*w,int n){ManagedSprite_SetAnim(w->a,0);ManagedSprite_SetDrawFlag(w->a,1);if(n==0)ManagedSprite_SetDrawFlag(w->b,0);else if(n>0){ManagedSprite_SetDrawPriority(w->a,2);ManagedSprite_SetDrawPriority(w->b,1);ManagedSprite_SetDrawFlag(w->b,1);ManagedSprite_SetAnim(w->b,n);ManagedSprite_SetAnimateFlag(w->b,1);}}
void ov112_021EA60C(Work*w){void*s=w->b;if(ov112_021E7668()){if(ManagedSprite_GetActiveAnim(s)==2)ManagedSprite_SetAnim(w->b,1);}else{if(ManagedSprite_GetActiveAnim(s)==1)ManagedSprite_SetAnim(w->b,2);}}
