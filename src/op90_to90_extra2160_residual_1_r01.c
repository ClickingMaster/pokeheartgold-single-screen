#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern const s8 bits$7808[];
int BN_num_bits_word(u32 l);
// @fn BN_num_bits_word
int BN_num_bits_word(u32 l) {
    int n;
    if (l & 0xffff0000) {
        n = (l & 0xff000000) ? 24 : 16;
    } else {
        n = (l & 0xff00) ? 8 : 0;
    }
    l >>= n;
    if (l & 0xf0) { return bits$7808[l >> 4] + n + 4; }
    return bits$7808[l] + n;
}
