#pragma force_active on
#include "global.h"
#pragma force_active on
void *Heap_Alloc(int, u32);
void Heap_Free(void *);
u16 MATH_CalcCRC16(const void *, const void *, u32);
void CRYPTO_RC4Init(void *, const void *, u32);
void CRYPTO_RC4Encrypt(void *, const void *, u32, void *);
u8 *sub_02035754(int);
void ov74_02235ED0(u8 *,void *,int);
void ov74_02235F58(u8 *,void *,int);
void ov74_02235ED0(u8 *src, void *dst, int heap) {
 void *table=Heap_Alloc(heap,512);
 u16 crc;
 u16 key[4];
 u16 val;
 int i;
 u16 *p;
 void *state;
 MATHi_CRC16InitTableRev(table,0xa001);
 crc=MATH_CalcCRC16(table,src,80);
 Heap_Free(table);
 OS_GetMacAddress((u8 *)key);
 key[3]=key[1];key[1]=crc;
 val=0xd679;
 for(i=0,p=key;i<4;i++,p++){*p ^=val;val=*p;}
 state=Heap_Alloc(heap,260);
 CRYPTO_RC4Init(state,key,8);
 CRYPTO_RC4Encrypt(state,src+80,856,dst);
 Heap_Free(state);
}
