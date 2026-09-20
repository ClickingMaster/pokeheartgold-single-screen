#include "to55_ov07_31fe4_complete_private.h"

void ov07_02231FE4(void *owner, Ov07T10Work *work) {
    work->state[0] = 0;
    work->state[1] = 0;
    work->state[2] = 0;
    work->state[3] = 0;
    work->owner = owner;
    work->field08 = ov07_0221C530(owner);
    work->field0C = ov07_0221C514(owner);
    work->field10 = ov07_0221C528(owner);
    work->field14 = ov07_0221C4A0(owner);
    work->field18 = ov07_0221FA78(owner);
}

void ov07_02232020(void *owner, u32 flags, Ov07T10Slot *slots, int *count) {
    u32 first;
    u32 second;
    u32 third;
    u32 fourth;
    void *pic;

    *count = 0;
    if ((flags & 0x40) == 0x40) {
        flags = ov07_0221C468(owner);
        slots[*count].pokepic = ov07_0221FA48(owner, flags);
        if (slots[*count].pokepic != NULL) {
            slots[*count].index = flags;
            ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
            (*count)++;
        }

        first = ov07_0223197C(owner, flags);
        slots[*count].pokepic = ov07_0221FA48(owner, first);
        if (slots[*count].pokepic != NULL) {
            slots[*count].index = first;
            ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
            (*count)++;
        }

        flags = ov07_02231958(owner, ov07_022319E0(ov07_0221FA04(owner, flags)));
        slots[*count].pokepic = ov07_0221FA48(owner, flags);
        if (slots[*count].pokepic != NULL) {
            slots[*count].index = flags;
            ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
            (*count)++;
        }

        flags = ov07_0223197C(owner, flags);
        slots[*count].pokepic = ov07_0221FA48(owner, flags);
        if (slots[*count].pokepic != NULL) {
            slots[*count].index = flags;
            ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
            (*count)++;
        }
        return;
    } else {
        if ((flags & 0x20) == 0x20) {
            flags = ov07_0221C468(owner);
            first = ov07_0223197C(owner, flags);
            if (first != flags) {
                slots[*count].pokepic = ov07_0221FA48(owner, first);
                if (slots[*count].pokepic != NULL) {
                    slots[*count].index = first;
                    ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                    (*count)++;
                }
            }

            second = ov07_02231958(owner, ov07_022319E0(ov07_0221FA04(owner, flags)));
            if (second != flags) {
                slots[*count].pokepic = ov07_0221FA48(owner, second);
                if (slots[*count].pokepic != NULL) {
                    slots[*count].index = second;
                    ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                    (*count)++;
                }
            }

            third = ov07_0223197C(owner, second);
            if (third != flags) {
                slots[*count].pokepic = ov07_0221FA48(owner, third);
                if (slots[*count].pokepic != NULL) {
                    slots[*count].index = third;
                    ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                    (*count)++;
                }
            }
            return;
        } else if ((flags & 2) == 2) {
            slots[*count].pokepic = ov07_0221FA48(owner, ov07_0221C468(owner));
            if (slots[*count].pokepic != NULL) {
                slots[*count].index = ov07_0221C468(owner);
                ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                (*count)++;
            }
        }

        if (ov07_0221FAB0(owner) == TRUE && (flags & 4) == 4) {
            slots[*count].pokepic = ov07_0221FA48(owner, ov07_0223197C(owner, ov07_0221C468(owner)));
            if (slots[*count].pokepic != NULL) {
                slots[*count].index = ov07_0223197C(owner, ov07_0221C468(owner));
                ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                (*count)++;
            }
        }

        if ((flags & 8) == 8) {
            slots[*count].pokepic = ov07_0221FA48(owner, ov07_0221C470(owner));
            if (slots[*count].pokepic != NULL) {
                slots[*count].index = ov07_0221C470(owner);
                ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                (*count)++;
            }
        }

        if (ov07_0221FAB0(owner) == TRUE && (flags & 0x10) == 0x10) {
            slots[*count].pokepic = ov07_0221FA48(owner, ov07_0223197C(owner, ov07_0221C470(owner)));
            if (slots[*count].pokepic != NULL) {
                slots[*count].index = ov07_0223197C(owner, ov07_0221C470(owner));
                ov07_02231FA0(slots[*count].pokepic, slots[*count].xy);
                (*count)++;
            }
        }
    }

done:
    (void)pic;
    (void)fourth;
}

void ov07_022323FC(void *owner, u32 flags, Ov07T10Slot *slots, int *count) {
    *count = 0;
    if ((flags & 2) == 2) {
        slots[*count].sprite = ov07_0221C4E8(owner, *count);
        ov07_02231FD8(slots[*count].sprite, slots[*count].xy);
        (*count)++;
    }
    if ((flags & 4) == 4) {
        slots[*count].sprite = ov07_0221C4E8(owner, *count);
        ov07_02231FD8(slots[*count].sprite, slots[*count].xy);
        (*count)++;
    }
    if ((flags & 8) == 8) {
        slots[*count].sprite = ov07_0221C4E8(owner, *count);
        ov07_02231FD8(slots[*count].sprite, slots[*count].xy);
        (*count)++;
    }
    if ((flags & 0x10) == 0x10) {
        slots[*count].sprite = ov07_0221C4E8(owner, *count);
        ov07_02231FD8(slots[*count].sprite, slots[*count].xy);
        (*count)++;
    }
}
