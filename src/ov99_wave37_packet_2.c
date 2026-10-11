#include "ov99_wave37_packet_private.h"

void ov99_021E76D8(R26Work *w, u32 species) {
    R26Packet packet = { 0 };
    packet.word0 = (packet.word0 & 0xFFFFFE00) | (species & 0x1FF);
    packet.word0 = (packet.word0 & 0xFFFFC1FF) | ((ov99_021E71E4(w, species) << 27) >> 18);
    packet.word0 = (packet.word0 & 0xFFFF3FFF) | ((ov99_021E7208(w, species) << 30) >> 16);
    if (species == 0x147) {
        packet.word1 = ov99_021E723C((void **)w);
    }
    PokeathlonUtil_LoadMonPicSprite(w->all[3], (const PokeathlonMonInfo *)&packet, 0, 0, (enum HeapID)w->heap);
}
