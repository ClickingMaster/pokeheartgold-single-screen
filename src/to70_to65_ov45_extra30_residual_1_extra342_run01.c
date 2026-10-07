#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#include "../lib/NitroDWC/src/tree-iterator4.h"
#pragma force_active on
struct S{int a;void *anchor;};typedef Iterator It;void ov45_02243128(It*,struct S*,void*);void ov45_02242C74(It*,struct S*,void*);void ov45_02243B50(It*,struct S*,Iterator);void ov45_022438F0(It*,struct S*,Iterator);int ov45_0224330C(struct S*,void*);int ov45_022435A8(struct S*,void*);
int ov45_0224330C(struct S*s,void*k){It it,out;ov45_02243128(&it,s,k);if(it.current==&s->anchor)return 0;ov45_02243B50(&out,s,(Iterator){it.current});return 1;}
