#include "overlay02_port_private.h"

WIP_LOCAL void ov02_02248DF0(void *a0, u8 *sm) {
    ov02_StateMachineFunc *table = ov02_02253320[sm[0]];
    while (table[sm[1]](sm) == 1) {
    }
}
