#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "dgt.h"
// clang-format on
#pragma force_active on
typedef struct DGTHash1Context Hash1;
typedef struct DGTHash2Context Hash2;
typedef struct {int digest_size,block_size;Hash2 *context;u8 *digest;void(*reset)(Hash2*);void(*source)(Hash2*,u8*,u32);void(*finish)(Hash2*,u8*);} HmacParams;
extern const HmacParams _0210E188;
extern void ProcessBlock(Hash1*);
extern void (*_02110E70)(Hash2*,const void*,u32);
void DGT_Hash2CalcHmac(void*,const void*,u32,const void*,u32);
void HmacCalc(void*,const void*,int,const void*,int,HmacParams*);
#include <stddef.h>
typedef char Layout_Hash1_88[(sizeof(Hash1)==88)?1:-1];
typedef char Layout_Hash1_Counter16[(offsetof(Hash1,length)==16)?1:-1];
typedef char Layout_Hash1_Block24[(offsetof(Hash1,buffer8)==24)?1:-1];
typedef char Layout_Hash2_104[(sizeof(Hash2)==104)?1:-1];
typedef char Layout_Hash2_Count28[(offsetof(Hash2,Message_Block_Index)==28)?1:-1];
typedef char Layout_Hash2_Block32[(offsetof(Hash2,Message_Block)==32)?1:-1];
typedef char Layout_Hash2_Tail96[(offsetof(Hash2,Computed)==96 && offsetof(Hash2,Corrupted)==100)?1:-1];
typedef char Layout_Hmac_28[(sizeof(HmacParams)==28 && offsetof(HmacParams,context)==8 && offsetof(HmacParams,digest)==12 && offsetof(HmacParams,reset)==16 && offsetof(HmacParams,source)==20 && offsetof(HmacParams,finish)==24)?1:-1];
void DGT_Hash1Reset(Hash1*ctx){ctx->state[0]=0x67452301;ctx->state[1]=0xefcdab89;ctx->state[2]=0x98badcfe;ctx->state[3]=0x10325476;ctx->length=0;}


void DGT_Hash1SetSource(Hash1 *p,u8 *src,u32 size){const u8 *s=src;u32 used=(u32)p->length&63;u32 remaining=64-used;int blocks;const u8 *q;p->length+=size;if(remaining>size){if(size)MI_CpuCopy8(s,p->buffer8+used,size);return;}MI_CpuCopy8(s,p->buffer8+used,remaining);ProcessBlock(p);size-=remaining;blocks=size>>6;q=s+remaining;while(blocks>0){MI_CpuCopy8(q,p->buffer8,64);q+=64;ProcessBlock(p);blocks--;}if(size&63)MI_CpuCopy8(q,p->buffer8,size&63);}


extern u8 _02110CA8[];
void DGT_Hash1GetDigest_R(u8 *digest, Hash1 *ctx) {
    u64 bits=ctx->length << 3;
    u32 high=(u32)(bits >> 32);
    u32 low=(u32)bits;
    u32 used,remaining;
    DGT_Hash1SetSource(ctx,_02110CA8,1);
    used=ctx->length & 63;
    remaining=64-used;
    if(remaining<8) {
        MI_CpuClear8(ctx->buffer8+used,remaining);
        ProcessBlock(ctx);
        used=0; remaining=64;
    }
    if(remaining>8) MI_CpuClear8(ctx->buffer8+used,remaining-8);
    ctx->buffer32[14]=low;
    ctx->buffer32[15]=high;
    ProcessBlock(ctx);
    MI_CpuCopy8(ctx,digest,16);
    MI_CpuClear8(ctx,88);
}
