#pragma force_active on
// clang-format off
#include "global.h"
#include "to91_http_lookup41558_private.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef struct Payload { char pad[0x2c]; void *a; void *b; int gap[2]; void (*callback)(int,void*,void*); } Payload;
typedef struct Node { struct Node *prev,*next; int id; Payload *payload; int field; } Node;
extern void *(*ov38_02226218)(u32,u32);
extern void (*ov38_0222621C)(void*);
extern Node *ov38_02226230;
extern volatile int ov38_0222622C;
extern void ov38_02222B98(Payload*);
int ov38_02221E2C(Payload*);

int ov38_02221F38(int);
int ov38_02221E2C(Payload *p) {
 int result=-1; Node *n=ov38_02226218(20,4);
 if(n) {
  if(ov38_02226230) { n->prev=ov38_02226230->prev; n->next=ov38_02226230; ov38_02226230->prev->next=n; *(Node *volatile *)&ov38_02226230->prev=n; }else { n->prev=n; n->next=n; ov38_02226230=n; }
  { int id=ov38_0222622C; ov38_0222622C=(int)((u32)ov38_0222622C+1); n->id=id; }
  n->payload=p; n->field=-1;
  result=n->id; if(ov38_0222622C<0) ov38_0222622C=0;
 }
 return result;
}
