#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef void (*ReqCallback)(int type, int result, int value);
typedef struct Request {
    int f00;
    int type;
    ReqCallback callback;
    int f0c;
    void *f10;
    int f14;
    void *f18;
    void *f1c;
    void *f20;
} Request;
extern char ov38_02225E24[], ov38_02225E34[];
extern void (*ov38_02226070)(void *);
extern int ov38_02226074, ov38_0222606C;
int ov38_02222F88(void *conn, const char *name, char **out);
int ov38_022218B4(const char *s, int n);
int ov38_0222308C(void *conn, char **out);
int ov38_0222187C(const char *a, const char *b, int n);
int ov38_022202C0(char *dst, const char *src, int len);
int ov38_02221414(void *conn, Request *req), ov38_02221494(void *conn, Request *req), ov38_02221468(void *conn, Request *req);
void ov38_02221734(void), ov38_02221748(void), ov38_02222E04(void *conn), ov38_022217D0(void), ov38_022217FC(void);
void ov38_02220830(int event, void *conn, Request *req);
// @fn ov38_02220830
void ov38_02220830(int event, void *conn, Request *req) {
    ReqCallback callback = req->callback;
    int f0c = req->f0c;
    int type = req->type;
    int value = -1;
    int result;
    char *status;
    char *body;
    char dec[4];

    switch (event) {
    case 0:
        if (ov38_02222F88(conn, ov38_02225E24, &status) != 3) {
            result = 3;
            break;
        } else {
            int code = ov38_022218B4(status, 3);
            if (code == 302) {
                BOOL found = FALSE;
                int len = ov38_0222308C(conn, &body);
                int i;
                if (len < 0) {
                    result = 3;
                    break;
                }
                for (i = 0; i < len;) {
                    int start = i;
                    char *p = body + i;
                    int n;
                    int vstart;
                    char *q;
                    int vlen;
                    while (*p != '=' && i < len) {
                        p++;
                        i++;
                    }
                    n = i - start;
                    i++;
                    if (n == 8) {
                        found = ov38_0222187C(body + start, ov38_02225E34, 8) == 0;
                    }
                    vstart = i;
                    q = body + i;
                    while (*q != '&' && *q != '\r' && *q != '\0' && i < len) {
                        q++;
                        i++;
                    }
                    vlen = i - vstart;
                    i++;
                    if (found) {
                        if (vlen == 4) {
                            int m = ov38_022202C0(dec, body + vstart, vlen);
                            dec[m] = '\0';
                            value = ov38_022218B4(dec, m);
                        }
                        break;
                    }
                }
                result = value < 0 ? 3 : 5;
            } else if (code <= 0 || code != 200) {
                result = 3;
            } else {
                switch (type) {
                case 1:
                    result = ov38_02221414(conn, req) ? 0 : 3;
                    break;
                case 2:
                    result = ov38_02221494(conn, req) ? 0 : 3;
                    break;
                case 3:
                    result = ov38_02221468(conn, req) ? 0 : 3;
                    break;
                }
            }
        }
        break;
    case 8:
        result = 6;
        break;
    case 6:
        result = 4;
        break;
    default:
        result = 3;
        break;
    }

    switch (type) {
    case 1:
        break;
    case 2:
        if (req->f20 != NULL) {
            ov38_02226070(req->f20);
        }
        if (req->f1c != NULL) {
            ov38_02226070(req->f1c);
        }
        ov38_02226070(req->f18);
        break;
    case 3:
        ov38_02226070(req->f10);
        break;
    }
    ov38_02221734();
    ov38_02226074 = -1;
    ov38_02221748();
    ov38_02222E04(conn);
    ov38_02226070(req);
    ov38_022217D0();
    if (result != 0) {
        ov38_0222606C = result;
    }
    if (callback != NULL) {
        callback(type, result, value);
    }
    if (f0c != 0) {
        ov38_022217FC();
    }
}
