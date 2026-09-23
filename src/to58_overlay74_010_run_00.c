#include "to58_overlay74_010_private.h"

extern void ClearFrameAndWindow2(void *window, int);
extern void ClearWindowTilemapAndCopyToVram(void *window);
extern void RemoveWindow(void *window);

void ov74_02231930(u8 *base) {
    MigratedPokemon *migrated = Save_MigratedPokemon_Get(*(SaveData **)(base + 0x10));
    u32 id = ov74_02233F68();
    MigratedPokemon_RecordMigration(migrated, id);
    sub_0201A728(4);
    SaveGameNormal(*(SaveData **)(base + 0x10));
    sub_0201A738(4);
}
