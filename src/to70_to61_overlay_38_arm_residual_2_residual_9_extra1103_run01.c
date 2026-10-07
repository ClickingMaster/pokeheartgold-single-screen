#pragma force_active on
// clang-format off
#include "global.h"
#include "to91_http_lookup41558_private.h"
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
int ov38_02221F38(int id) {
 int result=0; Node *n=ov38_02221EE0(id);
 if(n) {
 if(ov38_02226230!=ov38_02226230->prev) { n->prev->next=n->next; n->next->prev=n->prev; if(ov38_02226230==n) ov38_02226230=n->next; }
 else ov38_02226230=0;
 { Payload *p=n->payload; void (*callback)(int,void*,void*)=p->callback; void *b=p->b; void *a=p->a; ov38_02222B98(p); ov38_0222621C(n); callback(8,b,a); }
 result=1;
 }
 return result;
}
