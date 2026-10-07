#ifndef POKEHEARTGOLD_TO91_FRONTIER_DOWNLOAD_NAMES_H
#define POKEHEARTGOLD_TO91_FRONTIER_DOWNLOAD_NAMES_H
#include "global.h"
typedef struct Ov67DownloadedName {
    u16 name[9];
    u8 a, b, pad[4], bytes[8];
    u8 b0 : 1, b1 : 1, rest : 6;
    u8 end;
} Ov67DownloadedName;
typedef Ov67DownloadedName Ov67DownloadRecords[30];
typedef char Ov67DownloadedNameSize[(sizeof(Ov67DownloadedName) == 34) ? 1 : -1];
typedef char Ov67DownloadRecordsSize[(sizeof(Ov67DownloadRecords) == 0x3FC) ? 1 : -1];
#endif
