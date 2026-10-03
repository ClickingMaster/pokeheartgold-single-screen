#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{int*cur,*begin,*middle,*end;u8 flag;u8 pad[3];} Iterator;
static inline const int *Minimum(const int*a,const int*b){return *a<*b?a:b;}
static inline void Copy(int*first,int*last,int*dest){int count=last-first;memmove(dest-count,first,count*4);}
void ov45_0223C70C(Iterator*,Iterator*,Iterator*,Iterator*);
#pragma opt_propagation off
void ov45_0223C70C(Iterator*out,Iterator*first,Iterator*last,Iterator*dest){while(first->cur!=last->cur){int cap,amount;if(last->cur==last->begin)last->cur=last->end;--last->cur;if(dest->cur==dest->begin)dest->cur=dest->end;--dest->cur;cap=dest->cur-(dest->cur<dest->middle?dest->begin:dest->middle);if((first->cur>=first->middle)==(last->cur>=last->middle)){int n=last->cur-first->cur;amount=*Minimum(&cap,&n);}else {int n=last->cur-(last->cur<last->middle?last->begin:last->middle);amount=*Minimum(&cap,&n);}last->cur-=amount;Copy(last->cur,last->cur+(amount+1),dest->cur+1);dest->cur-=amount;}out->cur=dest->cur;out->begin=dest->begin;out->middle=dest->middle;out->end=dest->end;}
