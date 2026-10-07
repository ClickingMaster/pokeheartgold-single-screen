#include "global.h"
#pragma force_active on
typedef struct Ov96EA854Work {
    u32 unk00;
    void *unk04;
    void *unk08;
    u32 unk0C;
    void *unk10;
    void *unk14;
    void *unk18;
    u8 unk1C[0x330];
} Ov96EA854Work;
extern void ov96_021E8BF4(u8 *gfx,int index,int screen,void *palette);
void ov96_021EAF60(void *owner,u8 index,u32 screen,void *palette);
void ov96_021EAF60(void *owner,u8 index,u32 screen,void *palette){Ov96EA854Work *w=owner;ov96_021E8BF4(w->unk14,index,(int)screen,palette);}
