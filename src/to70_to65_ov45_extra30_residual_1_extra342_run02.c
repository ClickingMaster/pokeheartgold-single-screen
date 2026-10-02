#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
struct S{int a;int anchor;};struct It{void*p;};void ov45_02243128(struct It*,struct S*,void*);void ov45_02242C74(struct It*,struct S*,void*);void ov45_02243B50(struct It*,struct S*,void*);void ov45_022438F0(struct It*,struct S*,void*);int ov45_0224330C(struct S*,void*);int ov45_022435A8(struct S*,void*);
int ov45_022435A8(struct S*s,void*k){struct It it,out;volatile struct It copy;ov45_02242C74(&it,s,k);if(it.p==&s->anchor)return 0;copy.p=it.p;ov45_022438F0(&out,s,it.p);return 1;}
