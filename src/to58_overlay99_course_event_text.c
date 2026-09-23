#include "global.h"

typedef struct Ov99PageWork {
    void *records;
    u8 pad04[0x0C];
    void *text;
} Ov99PageWork;

extern const u32 ov99_021E958C[];
void *ov98_0221EEFC(void *);
int ov99_021E5C20(void *, int, u32);
void BufferPokeathlonEventName(void *, u32, u32);
void ov98_0221ECD0(void *, u32, u32, s32, u32, u32);
BOOL ov99_021E5BB4(void *, int);
u16 ov99_021E5BD8(void *, int, int);
void ov98_0221EBD8(void *, u32, u32, u32);
void ov99_021E607C(Ov99PageWork *, int);

void ov99_021E607C(Ov99PageWork *work, int page) {
    void *format = ov98_0221EEFC(work->text);
    int i;
    const u32 *table = ov99_021E958C;
    for (i = 0; i < 3; i++, table += 2) {
        BufferPokeathlonEventName(format, 1, (u8)ov99_021E5C20(work->records, page, (u8)i));
        ov98_0221ECD0(work->text, table[0], 10, i + 1, 1, 0);
        if (ov99_021E5BB4(work->records, page)) {
            ov98_0221ECD0(work->text, table[1], 8, ov99_021E5BD8(work->records, page, (u16)i), 3, 0);
        } else {
            ov98_0221EBD8(work->text, table[1], 9, 0);
        }
    }
    if (ov99_021E5BB4(work->records, page)) {
        ov98_0221ECD0(work->text, 12, 8, ov99_021E5BD8(work->records, page, 3), 3, 0);
    } else {
        ov98_0221EBD8(work->text, 12, 9, 0);
    }
}
