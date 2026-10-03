#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x618];u8 flags[3][512];int highest[3];} Work;extern void ov96_021FBF90(Work *,int,int);void ov96_021FC0FC(Work *,int,int);
void ov96_021FC0FC(Work *w,int position,int group){int q=position/8;int rem;if(q!=0){rem=position-q*8;q--;if(q<512&&w->flags[group][q]!=0&&w->highest[group]<q){w->highest[group]=q;ov96_021FBF90(w,group,rem);}}}
