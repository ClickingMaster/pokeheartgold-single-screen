#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 word;u16 attr;} Sprite;
typedef struct {u32 pad[2];Sprite **sprites;} Work;
extern Work ov13_0224F54C;extern u8 ov13_02245526[][3];
extern int ov13_022349B8(int);extern void ov13_022400F0(int,int,Sprite*);
void ov13_02234210(unsigned,int,int);
void ov13_02234210(unsigned type,int option,int index){u16 values[5];Sprite **base=ov13_0224F54C.sprites+6;Sprite **s;values[0]=204;values[1]=192;values[2]=192;values[3]=143;values[4]=143;s=base+index;
if((unsigned)(type-1)<=1){if(ov13_022349B8(index)==2)s=ov13_0224F54C.sprites+11;else s=ov13_0224F54C.sprites+12;}
if(ov13_02245526[type][option]){ov13_022400F0(0,ov13_02245526[type][option],*s);{u32 value=*(volatile u16*)&values[type];Sprite *obj=*(Sprite *volatile*)s;obj->word=((value<<23)>>7)|(obj->word&0xfe00ff00);}(*s)->attr=((*s)->attr&~0xc00)|0xc00;}else (*s)->word=((*s)->word&0xfe00ff00)|0x1000000;}
