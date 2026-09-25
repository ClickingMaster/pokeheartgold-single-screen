typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef int s32;
typedef struct DWCWifiInfo {
    u32 words[5];
} DWCWifiInfo;
typedef struct DWCPair {
    u32 a;
    u32 b;
} DWCPair;

extern void MIi_CpuClear16(u32, void *, u32);
extern int DWCi_AUTH_GetNewWiFiInfo(DWCWifiInfo *);
extern int DWCi_BM_SetWiFiInfo(const DWCWifiInfo *, void *);
extern int DWCi_BM_GetWiFiInfo(DWCWifiInfo *);

void initPage(void *backup, int page);
int DWCi_AUTH_MakeWiFiID(void *backup);
int DWCi_AUTH_UpDateWiFiID(DWCWifiInfo *info, void *backup);
void DWC_Auth_GetId(DWCWifiInfo *out);

void initPage(void *backup, int page) {
    MIi_CpuClear16(0, (u8 *)backup + (page << 8), 0x100);
    ((u8 *)backup + (page << 8))[0xe7] = 0xff;
}
