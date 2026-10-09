#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

int rand(void);
BOOL ov00_0220BC50(const u8 *buf);
extern const char ov00_022199E4[];
int abs(int x);
char *ov00_0220BE08(char *out, const char *key);

// @fn ov00_0220BE08
char *ov00_0220BE08(char *out, const char *key) {
    int i;
    int keyLen[1];
    BOOL valid;
    keyLen[0] = strlen(ov00_022199E4);
    valid = ov00_0220BC50((const u8 *)key);
    for (i = 0; i < 32; i++) {
        if (!valid || i == 0 || i == 13) {
            out[i] = rand() % 93 + 33;
        } else {
            int prev = ((i == 1 || i == 14) ? key[i] : key[i - 1]);
            out[i] = abs(((u8 *)key)[(i * (u8)key[i] + ov00_022199E4[(i + (u8)key[i]) % keyLen[0]]) % 32] ^ ov00_022199E4[(prev * (i * 0x4647)) % keyLen[0]]) % 93 + 33;
        }
    }
    return out;
}
