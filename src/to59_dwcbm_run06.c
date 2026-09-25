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

int DWCi_AUTH_MakeWiFiID(void *backup) {
    DWCWifiInfo info;
    if (DWCi_AUTH_GetNewWiFiInfo(&info) == 0) {
        return 0;
    }
    return DWCi_BM_SetWiFiInfo(&info, backup) != 0;
}

int DWCi_AUTH_UpDateWiFiID(DWCWifiInfo *info, void *backup) {
    DWCWifiInfo current;
    DWCi_BM_GetWiFiInfo(&current);
    ((u64 *)info)[0] = ((u64 *)info)[1];
    ((u64 *)info)[1] = ((u64 *)&current)[1];
    return DWCi_BM_SetWiFiInfo(info, backup) != 0;
}
