#include "global.h"
#include "pokemon.h"
#include "bg_window.h"
#include "message_format.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
typedef struct Ov111TextWorkR10 {
    void *unk00;
    void *unk04;
    void *unk08;
    void *unk0C;
    void *string;
    void *unk14;
    void *unk18;
    u8 unk1C[0x10];
    int printerId;
    union {
        u32 raw;
        struct {
            u32 count : 16;
            u32 style : 3;
            u32 special : 1;
            u32 flagsHi : 12;
        } bits;
    } status;
} Ov111TextWorkR10;

extern void*ov111_021E6A2C(void*);
extern void ov111_021E6934(Ov111TextWorkR10 *,void *,int,int,u8),ov111_021E696C(Ov111TextWorkR10 *,void *,int,int,u8,int),ov111_021E69A0(Ov111TextWorkR10 *,void *,int,int,BOOL,int),ov111_021E5D2C(Window *,u32,u32);
void ov111_021E64C8(void *,void *);
void ov111_021E64C8(void*w,void*context){Ov111TextWorkR10 *ctx=context;void*formatter=ov111_021E6A2C(ctx);Pokemon*mon=*(void **)w;int hp,maxhp,glyph,color;GF_ASSERT(w);GF_ASSERT(ctx);hp=GetMonData(mon,163,0);maxhp=GetMonData(mon,164,0);BufferBoxMonSpeciesName(formatter,0,Mon_GetBoxMon(mon));ov111_021E6934(ctx,(Window *)((u8 *)w+12),4,4,1);color=0x70800;switch(GetMonGender(mon)){case 0:glyph=5;break;case 1:glyph=6;color=0x30400;break;case 2:glyph=7;break;default:GF_ASSERT(FALSE);break;}ov111_021E696C(ctx,(Window *)((u8 *)w+28),glyph,0,1,color);ov111_021E69A0(ctx,(Window *)((u8 *)w+44),GetMonData(mon,161,0),3,1,1);ov111_021E69A0(ctx,(Window *)((u8 *)w+60),hp,3,0,0);ov111_021E69A0(ctx,(Window *)((u8 *)w+76),maxhp,3,1,0);ov111_021E5D2C((Window *)((u8 *)w+92),(u16)hp,(u16)maxhp);}
