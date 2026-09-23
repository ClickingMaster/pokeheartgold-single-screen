#include "global.h"

typedef struct { u32 value[11]; } Table11;
typedef struct { u32 value[6]; } Table6;
typedef struct { u32 value[3]; } Table3;
extern const Table11 ov07_02237228;
extern const Table6 ov07_02237188;
extern const Table3 ov07_0223717C;
int ov07_02232540(u32);
int ov07_02232580(u32);
int ov07_022325BC(u32);

int ov07_02232540(u32 value) {
    Table11 list = ov07_02237228;
    int result = 0;
    u32 i = result;
    u32 *ptr = list.value;
    while (i < 11) {
        if (value == *ptr) {
            result = 1;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}

int ov07_02232580(u32 value) {
    Table6 list = ov07_02237188;
    int result = 0;
    u32 i = result;
    u32 *ptr = list.value;
    while (i < 6) {
        if (value == *ptr) {
            result = 1;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}

int ov07_022325BC(u32 value) {
    Table3 list = ov07_0223717C;
    int result = 1;
    u32 i = 0;
    u32 *ptr = list.value;
    while (i < 3) {
        if (value == *ptr) {
            result = 0;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}
