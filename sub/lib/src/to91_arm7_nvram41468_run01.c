#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void NVRAM_ReadStatusRegister(u8 *);
int NvramCheckReadyToRead(void);
int NvramCheckReadyToWrite(void);
int NvramCheckReadyToRead(void) { u16 status; NVRAM_ReadStatusRegister((u8 *)&status); return (status & 1) == 0; }
int NvramCheckReadyToWrite(void) { u16 status; NVRAM_ReadStatusRegister((u8 *)&status); if (status & 1) return 0; return (status & 2) != 0; }
