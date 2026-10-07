#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "dgt.h"
// clang-format on
typedef struct DGTHash2Context Hash2;
typedef struct {int digest_size,block_size;Hash2 *context;u8 *digest;void(*reset)(Hash2*);void(*source)(Hash2*,u8*,u32);void(*finish)(Hash2*,u8*);} HmacParams;
extern const HmacParams _0210E188;
extern void (*_02110E70)(Hash2*,const void*,u32);
void DGT_Hash2CalcHmac(void*,const void*,u32,const void*,u32);
void HmacCalc(void*,const void*,int,const void*,int,HmacParams*);
#include <stddef.h>
typedef char Layout_Hash2_104[(sizeof(Hash2)==104)?1:-1];
typedef char Layout_Hash2_Count28[(offsetof(Hash2,Message_Block_Index)==28)?1:-1];
typedef char Layout_Hash2_Block32[(offsetof(Hash2,Message_Block)==32)?1:-1];
typedef char Layout_Hash2_Tail96[(offsetof(Hash2,Computed)==96 && offsetof(Hash2,Corrupted)==100)?1:-1];
typedef char Layout_Hmac_28[(sizeof(HmacParams)==28 && offsetof(HmacParams,context)==8 && offsetof(HmacParams,digest)==12 && offsetof(HmacParams,reset)==16 && offsetof(HmacParams,source)==20 && offsetof(HmacParams,finish)==24)?1:-1];
// @fn DGT_Hash2Reset
void DGT_Hash2Reset(Hash2*ctx){ctx->Intermediate_Hash[0]=0x67452301;ctx->Intermediate_Hash[1]=0xefcdab89;ctx->Intermediate_Hash[2]=0x98badcfe;ctx->Intermediate_Hash[3]=0x10325476;ctx->Intermediate_Hash[4]=0xc3d2e1f0;ctx->Length_Low=0;ctx->Length_High=0;ctx->Message_Block_Index=0;}


// @fn DGT_Hash2SetSource
void DGT_Hash2SetSource(Hash2 *p,u8 *src,u32 size){const u8 *s=src;u8 *buf=p->Message_Block;u32 n;int count;if(!size)return;n=p->Length_Low+(size<<3);if(n<p->Length_Low)p->Length_High++;p->Length_High+=size>>29;p->Length_Low=n;if(p->Message_Block_Index){if(p->Message_Block_Index+size>=64){n=64-p->Message_Block_Index;MI_CpuCopy8(s,buf+p->Message_Block_Index,n);size-=n;s+=n;_02110E70(p,buf,64);p->Message_Block_Index=0;}else{MI_CpuCopy8(s,buf+p->Message_Block_Index,size);p->Message_Block_Index+=size;return;}}if(size>=64){count=size&~63;size-=count;if(!((u32)s&3)){_02110E70(p,s,count);s+=count;}else{do{MI_CpuCopy8(s,buf,64);s+=64;_02110E70(p,buf,64);count-=64;}while(count>0);}}p->Message_Block_Index=size;if(size)MI_CpuCopy8(s,buf,size);}


// @fn DGT_Hash2GetDigest
void DGT_Hash2GetDigest(Hash2*ctx,u8*out){int n=ctx->Message_Block_Index,index=n>>2;u32*words=ctx->Message_Block_Words;u8*bytes;u32 value;if(!(n&3))words[index]=0;bytes=ctx->Message_Block;bytes[n++]=128;while(n&3)bytes[n++]=0;index++;if(ctx->Message_Block_Index>=56){while(index<16){words[index]=0;index++;}_02110E70(ctx,words,64);index=0;}while(index<14){words[index]=0;index++;}
 value=ctx->Length_Low;bytes[63]=value;bytes[62]=value>>8;bytes[61]=value>>16;bytes[60]=value>>24;value=ctx->Length_High;bytes[59]=value;bytes[58]=value>>8;bytes[57]=value>>16;bytes[56]=value>>24;_02110E70(ctx,words,64);
 value=ctx->Intermediate_Hash[0];out[0]=value>>24;out[1]=value>>16;out[2]=value>>8;out[3]=value;
 value=ctx->Intermediate_Hash[1];out[4]=value>>24;out[5]=value>>16;out[6]=value>>8;out[7]=value;
 value=ctx->Intermediate_Hash[2];out[8]=value>>24;out[9]=value>>16;out[10]=value>>8;out[11]=value;
 value=ctx->Intermediate_Hash[3];out[12]=value>>24;out[13]=value>>16;out[14]=value>>8;out[15]=value;
 value=ctx->Intermediate_Hash[4];out[16]=value>>24;out[17]=value>>16;out[18]=value>>8;out[19]=value;
 ctx->Message_Block_Index=0;MI_CpuClear32(&ctx,4);}


// @fn DGT_Hash2CalcHmac
void DGT_Hash2CalcHmac(void *out,const void *src,u32 size,const void *key,u32 keySize){Hash2 ctx;u8 digest[20];HmacParams param=_0210E188;param.context=&ctx;param.digest=digest;param.reset=DGT_Hash2Reset;param.source=DGT_Hash2SetSource;param.finish=DGT_Hash2GetDigest;HmacCalc(out,src,size,key,keySize,&param);}



// @fn HmacCalc
void HmacCalc(void *out,const void *src,int size,const void *key,int keySize,HmacParams *p){u8 digest[64],inner[64],outer[64];const u8 *k=key;int i;if(!out||!src||!size||!key||!keySize||!p)return;if(keySize>p->block_size){p->reset(p->context);p->source(p->context,(u8*)key,keySize);p->finish(p->context,digest);keySize=p->digest_size;k=digest;}for(i=0;i<keySize;i++)inner[i]=k[i]^0x36;for(;i<p->block_size;i++)inner[i]=0x36;p->reset(p->context);p->source(p->context,inner,p->block_size);p->source(p->context,(u8*)src,size);p->finish(p->context,p->digest);for(i=0;i<keySize;i++)outer[i]=k[i]^0x5c;for(;i<p->block_size;i++)outer[i]=0x5c;p->reset(p->context);p->source(p->context,outer,p->block_size);p->source(p->context,p->digest,p->digest_size);p->finish(p->context,(u8*)out);}
