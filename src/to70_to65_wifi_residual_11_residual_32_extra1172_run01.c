#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[0x455];
    u8 status, retained;
    u8 p2[0x3cd];
    u8 *record;
    u32 size, filled;
} Record;

typedef struct {
    u8 p[12];
    Record *record;
    u8 p2[0x34];
    u32 available;
} Socket;

extern void *(*gCpsAlloc)(u32);
extern u8 *CPSi_TcpReadRaw(u32 *, Socket *);
extern void CPSi_SocConsumeRaw(u32, Socket *);
extern void parse_record_in_buf(Record *, u8 *);
void try_fill_record(Socket *);

void try_fill_record(Socket *s) {
    Record *r = s->record;
    u32 size;
    u8 *data;
    int complete;
    if (r->record == 0) {
        if (s->available < 5) {
            return;
        }
        data = CPSi_TcpReadRaw(&size, s);
        size = (data[3] << 8) + data[4] + 5;
        if (size > 0x4805) {
            r->status = 9;
            return;
        }
        r->record = gCpsAlloc(size);
        if (r->record == 0) {
            r->status = 9;
            return;
        }
        r->size = size;
        r->filled = 0;
        r->retained = 0;
    } else if (s->available == 0) {
        return;
    }
    data = CPSi_TcpReadRaw(&size, s);
    if (size >= r->size - r->filled) {
        size = r->size - r->filled;
        complete = 1;
    } else {
        complete = 0;
    }
    MI_CpuCopy8(data, r->record + r->filled, size);
    CPSi_SocConsumeRaw(size, s);
    if (complete) {
        parse_record_in_buf(r, r->record);
        if (r->retained == 0) {
            r->record = 0;
        }
    } else {
        r->filled += size;
    }
}
