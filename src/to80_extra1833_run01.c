#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Row {u16 value:11,unused:5;u16 other;u32 rest[13];} Row;
extern void *ov80_022372B4(void *);
extern void ov80_02236BE4(u8,void *,void *,int);
extern void ov80_02236E24(void *,u8,void *,void *,void *,void *,u16,u16 *,u16 *);
extern void ov80_022300D4(void *,int,int);
extern int ov80_02237254(u8);
extern void *ov80_02236DF8(u8,int);
extern void ov80_02236E90(void *,u16,u8,Row *,void *,void *,void *,void *,int);
extern void *AllocMonZeroed(int);
extern int ov80_02237120(void *);
extern void ov80_0222A140(Row *,void *,int);
extern void ov80_0222A3BC(void *,void *,void *);
extern void Heap_Free(void *);
extern void *Party_GetMonByIndex(void *,int);
void ov80_0222FF00(u8 *);
typedef struct Orig {u8 pad[0x282];u16 value;} Orig;
void ov80_0222FF00(u8 *s){Row rows[12];u16 ids[6],others[6];int count,i;
 ov80_02236BE4(s[4],ov80_022372B4(s),s+0x18,14);
 ov80_02236E24(ov80_022372B4(s),s[5],s+0x254,s+0x280,s+0x260,s+0x268,*(u16 *)(s+8),0,0);
 ov80_022300D4(s,4,0);ov80_022300D4(s,5,0);count=6;
 {u8 *p;i=0;p=s;for(;i<6;i++){rows[i]=*(Row *)(p+0x280);p+=56;}}
 if(ov80_02237254(s[4])==1){struct{Row*r;u16*id;Orig*p;u16*other;}v;i=0;v.r=(Row *)(s+0x280);v.id=ids;v.p=(Orig *)s;v.other=others;for(;i<6;i++){*v.id++=v.r->value;*v.other++=v.p->value;v.r++;v.p=(Orig *)((u8 *)v.p+56);}
 ov80_02236E24(ov80_022372B4(s),s[5],s+0x584,s+0x5b0,s+0x590,s+0x598,*(u16 *)(s+0x580),ids,others);
 ov80_022300D4(s,4,1);ov80_022300D4(s,5,1);count=12;{u8 *p;i=0;p=s;for(;i<6;i++){rows[i+6]=*(Row *)(p+0x5b0);p+=56;}}}
 ov80_02236E90(ov80_02236DF8(s[4],1),((u16 *)(s+0x18))[s[6]],s[5],rows,s+0x3d2,s+0x3f0,s+0x3da,s+0x3e0,count);
 for(i=0;i<6;i++){void *mon=AllocMonZeroed(11);ov80_0222A140(&((Row *)(s+0x280))[i],mon,ov80_02237120(s));ov80_0222A3BC(*(void **)(s+0x4f8),*(void **)(s+0x4d4),mon);Heap_Free(mon);}
 for(i=0;i<6;i++)Party_GetMonByIndex(*(void **)(s+0x4d4),i);
}
