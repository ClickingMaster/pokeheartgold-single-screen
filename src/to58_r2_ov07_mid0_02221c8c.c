#include "to58_r2_ov07_mid0_private.h"

typedef struct R2MaskTable { int masks[15]; } R2MaskTable;
extern const R2MaskTable ov07_022363D8;

int ov07_02221C8C(int i);
int ov07_02221CB4(void);

int ov07_02221C8C(int i) {
    R2MaskTable table = ov07_022363D8;
    return table.masks[i];
}
