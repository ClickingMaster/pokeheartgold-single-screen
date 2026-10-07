#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[20];void*renderer;u8 q[24];u16 keys,pad,repeat;u8 r[22];u16 selected,other;u8 s[20];u32 value;u8 t[2],mode;}Work;
void ov102_021E940C(void*,int),ov102_021E79A0(void*,void*),ov102_021E7968(void*,void*);int ov102_021E7D30(Work*,int),ov102_021E7DBC(Work*);
int ov102_021E7DBC(Work*w){if(w->keys&2){w->selected=2;ov102_021E940C(w->renderer,15);return ov102_021E7D30(w,2);}if(w->keys&1)return ov102_021E7D30(w,w->selected);if(w->keys&64){if(w->selected!=0){w->selected--;return ov102_021E7D30(w,3);}if(w->mode!=0&&w->other!=0){w->other--;return ov102_021E7D30(w,3);}return 1;}if(w->keys&128){if(w->selected==0){if(w->mode==2&&w->other==0){w->other++;return ov102_021E7D30(w,3);}w->selected++;return ov102_021E7D30(w,3);}if(w->selected==1){w->selected++;return ov102_021E7D30(w,3);}return 1;}if(w->repeat&32){ov102_021E79A0(&w->value,(u8*)w+8);return ov102_021E7D30(w,4);}if(w->repeat&16){ov102_021E7968(&w->value,(u8*)w+8);return ov102_021E7D30(w,4);}return 1;}
