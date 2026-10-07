#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_rotateloops off
typedef struct Link {u32 first;struct Link *next;} Link;
typedef struct Wave {u8 prefix[24];Link*links;} Wave;
typedef struct Bank {u8 prefix[24];struct {Wave*wave;Link*next;} slot[4];} Bank;
void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
void SND_DestroyBank(Bank*);
void SND_DestroyBank(Bank*b){int i;Link*link;SNDi_LockMutex();link=(Link*)(b->slot);i=0;do{Wave*w=b->slot[i].wave;if(w){Link*p=w->links;if(link==p){w->links=b->slot[i].next;DC_StoreRange(w,0x3c);}else{if(p){do{Link*n=p->next;if(link==n)break;p=n;}while(p);}p->next=b->slot[i].next;DC_StoreRange(p,8);}}i++;link++;}while(i<4);SNDi_UnlockMutex();}
