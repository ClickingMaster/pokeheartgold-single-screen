#pragma force_active on
#include "global.h"
void WifiSetup_DecodeLittleEndian(u32*,const u8*,u32);
void WifiSetup_FillBytes(void*,int,u32);
typedef struct HashCtx { u32 state[4]; u32 bitsLo,bitsHi; u8 buf[64]; } HashCtx;
void WifiSetup_Md5Transform(HashCtx*,const u8*);
#define ROL(x,s) (((x)<<(s))|((x)>>(32-(s))))
#define F(b,c,d) (((b)&(c))|((~(b))&(d)))
#define F14(b,c,d) (((b)&(c))|((inverse)&(d)))
#define G0(b,c,d) (((b)&(d))|((inverse)&(c)))
#define G(b,c,d) (((b)&(d))|((c)&(~(d))))
#define H(b,c,d) ((b)^(c)^(d))
#define I(b,c,d) ((c)^((b)|(~(d))))
#define STEP(f,a,b,c,d,x,k,s) do {(a)+=(f(b,c,d)+(x)+(k));(a)=ROL(a,s);(a)+=(b);}while(0)

static inline void AccumulateDigest(HashCtx *ctx,u32 a,u32 b,u32 c,u32 d){ctx->state[0]+=a;ctx->state[1]+=b;ctx->state[2]+=c;ctx->state[3]+=d;}
// @fn WifiSetup_Md5Transform
void WifiSetup_Md5Transform(HashCtx*context,const u8*input){u32*state=context->state;u32 a=state[0],b=state[1],c=state[2],d=state[3];u32 inverse;u32 words[16];u32 w0,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15;WifiSetup_DecodeLittleEndian(words,input,64);
w0=words[0];STEP(F,a,b,c,d,w0,0xd76aa478u,7);
w1=words[1];STEP(F,d,a,b,c,w1,0xe8c7b756u,12);
w2=words[2];STEP(F,c,d,a,b,w2,0x242070dbu,17);
w3=words[3];STEP(F,b,c,d,a,w3,0xc1bdceeeu,22);
w4=words[4];STEP(F,a,b,c,d,w4,0xf57c0fafu,7);
w5=words[5];STEP(F,d,a,b,c,w5,0x4787c62au,12);
w6=words[6];STEP(F,c,d,a,b,w6,0xa8304613u,17);
w7=words[7];STEP(F,b,c,d,a,w7,0xfd469501u,22);
w8=words[8];STEP(F,a,b,c,d,w8,0x698098d8u,7);
w9=words[9];STEP(F,d,a,b,c,w9,0x8b44f7afu,12);
w10=words[10];STEP(F,c,d,a,b,w10,0xffff5bb1u,17);
w11=words[11];STEP(F,b,c,d,a,w11,0x895cd7beu,22);
w12=words[12];STEP(F,a,b,c,d,w12,0x6b901122u,7);
w13=words[13];STEP(F,d,a,b,c,w13,0xfd987193u,12);
inverse=~d;w14=words[14];STEP(F14,c,d,a,b,w14,0xa679438eu,17);
w15=words[15];STEP(F,b,c,d,a,w15,0x49b40821u,22);
STEP(G0,a,b,c,d,w1,0xf61e2562u,5);
STEP(G,d,a,b,c,w6,0xc040b340u,9);
STEP(G,c,d,a,b,w11,0x265e5a51u,14);
STEP(G,b,c,d,a,w0,0xe9b6c7aau,20);
STEP(G,a,b,c,d,w5,0xd62f105du,5);
STEP(G,d,a,b,c,w10,0x02441453u,9);
STEP(G,c,d,a,b,w15,0xd8a1e681u,14);
STEP(G,b,c,d,a,w4,0xe7d3fbc8u,20);
STEP(G,a,b,c,d,w9,0x21e1cde6u,5);
STEP(G,d,a,b,c,w14,0xc33707d6u,9);
STEP(G,c,d,a,b,w3,0xf4d50d87u,14);
STEP(G,b,c,d,a,w8,0x455a14edu,20);
STEP(G,a,b,c,d,w13,0xa9e3e905u,5);
STEP(G,d,a,b,c,w2,0xfcefa3f8u,9);
STEP(G,c,d,a,b,w7,0x676f02d9u,14);
STEP(G,b,c,d,a,w12,0x8d2a4c8au,20);
STEP(H,a,b,c,d,w5,0xfffa3942u,4);
STEP(H,d,a,b,c,w8,0x8771f681u,11);
STEP(H,c,d,a,b,w11,0x6d9d6122u,16);
STEP(H,b,c,d,a,w14,0xfde5380cu,23);
STEP(H,a,b,c,d,w1,0xa4beea44u,4);
STEP(H,d,a,b,c,w4,0x4bdecfa9u,11);
STEP(H,c,d,a,b,w7,0xf6bb4b60u,16);
STEP(H,b,c,d,a,w10,0xbebfbc70u,23);
STEP(H,a,b,c,d,w13,0x289b7ec6u,4);
STEP(H,d,a,b,c,w0,0xeaa127fau,11);
STEP(H,c,d,a,b,w3,0xd4ef3085u,16);
STEP(H,b,c,d,a,w6,0x04881d05u,23);
STEP(H,a,b,c,d,w9,0xd9d4d039u,4);
STEP(H,d,a,b,c,w12,0xe6db99e5u,11);
STEP(H,c,d,a,b,w15,0x1fa27cf8u,16);
STEP(H,b,c,d,a,w2,0xc4ac5665u,23);
STEP(I,a,b,c,d,w0,0xf4292244u,6);
STEP(I,d,a,b,c,w7,0x432aff97u,10);
STEP(I,c,d,a,b,w14,0xab9423a7u,15);
STEP(I,b,c,d,a,w5,0xfc93a039u,21);
STEP(I,a,b,c,d,w12,0x655b59c3u,6);
STEP(I,d,a,b,c,w3,0x8f0ccc92u,10);
STEP(I,c,d,a,b,w10,0xffeff47du,15);
STEP(I,b,c,d,a,w1,0x85845dd1u,21);
STEP(I,a,b,c,d,w8,0x6fa87e4fu,6);
STEP(I,d,a,b,c,w15,0xfe2ce6e0u,10);
STEP(I,c,d,a,b,w6,0xa3014314u,15);
STEP(I,b,c,d,a,w13,0x4e0811a1u,21);
STEP(I,a,b,c,d,w4,0xf7537e82u,6);
STEP(I,d,a,b,c,w11,0xbd3af235u,10);
STEP(I,c,d,a,b,w2,0x2ad7d2bbu,15);
STEP(I,b,c,d,a,w9,0xeb86d391u,21);
AccumulateDigest(context,a,b,c,d);WifiSetup_FillBytes(words,0,64);}
