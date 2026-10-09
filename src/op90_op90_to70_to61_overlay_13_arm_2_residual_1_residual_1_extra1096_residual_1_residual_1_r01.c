#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *a; void *b; } Work;
Work *ov13_0224F464;
void ov13_022425F0(int, void *);
void ov13_02241D98(void *);
void ov13_022412B0(void *p);
void ov13_02227028(void);
// @fn ov13_02227028
void ov13_02227028(void) {
    ov13_022425F0(1, ov13_0224F464->a);
    ov13_02241D98(ov13_0224F464->b);
    ov13_022412B0(&ov13_0224F464);
}
