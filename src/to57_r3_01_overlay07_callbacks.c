#include "global.h"

int ov07_0221FA04(void *, int);
int ov07_02231924(void *, int);
void *ov07_0221BFC0(void *);

int ov07_0223192C(void *, int);
int ov07_02231958(void *, int);
int ov07_0223197C(void *, int);
int ov07_022319E0(int);
void ov07_02231A50(void *, int, void *);
void ov07_02231A20(int, int, void *);

int ov07_0223192C(void *owner, int index) {
    int result;
    switch (ov07_0221FA04(owner, index)) {
    case 0:
    case 2:
    case 4:
        result = 3;
        break;
    case 1:
    case 3:
    case 5:
        result = 4;
        break;
    }
    return result;
}

int ov07_02231958(void *owner, int value) {
    int i;
    for (i = 0; i < 4; i++) {
        int current = ov07_0221FA04(owner, i);
        if (current != value) {
            continue;
        }
        return i;
    }
    return 0;
}

int ov07_0223197C(void *owner, int index) {
    int i;
    u32 value = ov07_0221FA04(owner, index);
    if (value <= 1) {
        return value;
    }
    for (i = 0; i < 4; i++) {
        int other = ov07_0221FA04(owner, i);
        switch (value) {
        case 2:
            if (other == 4) {
                return i;
            }
            break;
        case 4:
            if (other == 2) {
                return i;
            }
            break;
        case 3:
            if (other == 5) {
                return i;
            }
            break;
        case 5:
            if (other == 3) {
                return i;
            }
            break;
        }
    }
    GF_AssertFail();
    return value;
}

int ov07_022319E0(int value) {
    switch (value) {
    case 0:
        return 1;
    case 1:
        return 0;
    case 2:
        return 3;
    case 3:
        return 2;
    case 4:
        return 5;
    case 5:
        return 4;
    default:
        GF_AssertFail();
        return 0;
    }
}
typedef struct Ov07Pair16 {
    u16 x;
    u16 y;
} Ov07Pair16;
typedef struct Ov07SmallTable {
    Ov07Pair16 row[6];
} Ov07SmallTable;
extern const Ov07SmallTable ov07_02236864;
void ov07_02231A20(int index, int unused, void *out_) {
    Ov07SmallTable local = ov07_02236864;
    Ov07Pair16 *out = out_;
    *out = local.row[index];
}
void ov07_02231A50(void *owner, int index, void *out) {
    int first = ov07_02231924(owner, index);
    int second = (int)ov07_0221BFC0(owner);
    ov07_02231A20(first, second, out);
}

typedef struct Ov07Pair32 {
    u32 a;
    u32 b;
} Ov07Pair32;

typedef struct Ov07BigTable {
    Ov07Pair32 row[0x120];
} Ov07BigTable;

typedef struct Ov07Triple32 {
    u32 a;
    u32 b;
    u32 c;
} Ov07Triple32;

extern const Ov07BigTable ov07_0223687C;

void ov07_02231A70(int group, Ov07Triple32 *out, int unused, int variant, int offset);

void ov07_02231A70(int group, Ov07Triple32 *out, int unused, int variant, int offset) {
    Ov07BigTable local = ov07_0223687C;
    u8 *base = (u8 *)&local + group * 0x120;
    int byteOffset = (variant + offset * 2) * sizeof(Ov07Triple32);
    u32 c = *(u32 *)(base + byteOffset + 8);
    u32 b = *(u32 *)(base + byteOffset + 4);
    u32 a = *(u32 *)(base + byteOffset);
    out->a = a;
    out->b = b;
    out->c = c;
}
