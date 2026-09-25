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

void DWC_Auth_GetId(DWCWifiInfo *out) {
    DWCWifiInfo info;
    u32 high;
    u32 low;
    int valid;
    DWCi_BM_GetWiFiInfo(&info);
    ((u64 *)out)[0] = ((u64 *)&info)[0];
    ((u64 *)out)[1] = ((u64 *)&info)[1];
    high = info.words[1];
    low = info.words[0];
    valid = 0;
    if (high != 0 || low != 0) {
        valid = 1;
    }
    out->words[4] = valid;
}
