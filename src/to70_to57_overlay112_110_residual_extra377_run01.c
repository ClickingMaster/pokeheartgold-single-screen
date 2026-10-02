#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Bits {u16 value;u8 first:5;u8 second:2;u8 third:1;u8 pad;} Bits;
typedef struct SourceBits {u8 first:5;u8 second:2;u8 pad:1;u8 pad2:1;u8 third:1;} SourceBits;
typedef struct Params {void *data;u32 delta;Bits bits;} Params;
void ov112_021F30BC(void *,Params *),ov112_021F3140(void *,Bits *),ov112_021ED7DC(u8 *);int ov112_021F31BC(void *);
void ov112_021ED7DC(u8 *s){Params p={0};Bits b;p.data=s+0x9d54;p.bits.value=*(u16 *)(s+0x9d44);p.bits.first=((SourceBits *)(s+0x9d51))->first;p.bits.second=((SourceBits *)(s+0x9d51))->second;p.bits.third=((SourceBits *)(s+0x9d51))->third;u32 current=*(u32 *)(s+0x10f0);u32 old=*(u32 *)(s+0x1d758);if(current<old)p.delta=0;else p.delta=current-old;ov112_021F30BC(s+0xaa34,&p);if(ov112_021F31BC(s+0x9dfc)){b.value=*(u16 *)(s+0xad00);b.first=((SourceBits *)(s+0xad0d))->first;b.second=((SourceBits *)(s+0xad0d))->second;b.third=((SourceBits *)(s+0xad0d))->third;ov112_021F3140(s+0x9dfc,&b);}}
