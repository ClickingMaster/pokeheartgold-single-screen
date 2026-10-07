#pragma force_active on
#include "global.h"

typedef struct HeaderInfo {
    u32 unk00;
    u32 unk04;
    u32 sizeLo : 8;
    u32 sizeHi : 24;
} HeaderInfo;

typedef struct Header {
    u32 unk00_0 : 12;
    u32 encrypted : 4;
    u32 seed : 16;
    HeaderInfo info;
    u32 data[1];
} Header;

void ov74_022311DC(Header *header);
u32 ov74_0223127C(u32 seed, u32 size, void *data);
void ov74_022313F0(Header *header);

// @fn ov74_022313F0
void ov74_022313F0(Header *header) {
    u32 seed;
    ov74_022311DC(header);
    if (header->encrypted == 1) {
        seed = ov74_0223127C(header->seed, sizeof(HeaderInfo), &header->info);
        ov74_0223127C(seed, header->info.sizeLo + header->info.sizeHi - 16, header->data);
    }
}
