#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct HttpState {
    u8 pad00[0xc];
    int size;
    u8 pad10[8];
    void *buffer;
    u8 pad1c[0x420 - 0x1c];
} HttpState;
typedef struct HttpSecure {
    u8 pad000[0x800];
    char *host;
    u8 pad804[0x830 - 0x804];
} HttpSecure;
typedef struct HttpHandle {
    int f00;
    int f04;
    int secure;
    int hostEnd;
    int pathStart;
    int f14;
    int f18;
    int f1c;
    HttpSecure *ssl;
    char *url;
    int port;
    void *param;
    HttpState *state;
    int f34;
    int f38;
    void *callback;
    int mode;
    u8 config[0x14];
} HttpHandle;
extern int ov38_02226214;
extern void *(*ov38_02226218)(int size, int align);
extern void (*ov38_0222621C)(void *ptr);
extern char ov38_02225ED0[], ov38_02225ED8[];
extern u8 ov38_02224F74[];
void ov38_02223358(void *ptr, int size);
int ov38_0222334C(const char *s);
int ov38_022232E4(const char *a, const char *b, int n);
int ov38_02224A98(const char *s, int n);
void ov38_0222336C(void *dst, const void *src, int n);
int ov38_02224D58(const char *s, int n);
HttpHandle *ov38_022225B8(const char *url, int mode, void *buffer, int size, void *callback, void *param);
// @fn ov38_022225B8
HttpHandle *ov38_022225B8(const char *url, int mode, void *buffer, int size, void *callback, void *param) {
    int len;
    int prefix;
    int i;
    int j;
    const char *host;
    int pct;
    int esc;
    BOOL slash;
    char *p;
    HttpHandle *h = NULL;

    if (mode != 0 && mode != 1 && mode != 2) {
        ov38_02226214 = 11;
        goto fail;
    }
    if (size == 0) {
        ov38_02226214 = 11;
        goto fail;
    }
    h = ov38_02226218(sizeof(HttpHandle), 4);
    if (h == NULL) {
        ov38_02226214 = 1;
        goto fail;
    }
    ov38_02223358(h, sizeof(HttpHandle));
    h->state = ov38_02226218(sizeof(HttpState), 4);
    if (h->state == NULL) {
        ov38_02226214 = 1;
        goto fail;
    }
    ov38_02223358(h->state, sizeof(HttpState));
    h->state->buffer = buffer;
    h->state->size = size;
    len = ov38_0222334C(url);
    if (len <= 7) {
        ov38_02226214 = 4;
        goto fail;
    }
    prefix = 7;
    h->port = 80;
    if (ov38_022232E4(url, ov38_02225ED0, prefix) != 0) {
        if (ov38_022232E4(url, ov38_02225ED8, 8) != 0) {
            ov38_02226214 = 4;
            goto fail;
        }
        h->secure = 1;
        h->port = 443;
        prefix = 8;
    }
    len -= prefix;
    host = url + prefix;
    if (len <= 0) {
        ov38_02226214 = 4;
        goto fail;
    }

    i = 0;
    pct = 0;
    esc = 0;
    while (i < len && host[i] != '/') {
        if (esc == 2) {
            esc--;
        } else if (esc == 1) {
            char c = ov38_02224A98(host + (i - 1), 2);
            esc--;
            if (c < 0) {
                ov38_02226214 = 4;
                goto fail;
            }
            if (c == '/') {
                pct--;
                break;
            }
        } else if (host[i] == '%') {
            esc = 2;
            pct++;
        }
        i++;
    }
    if (esc != 0) {
        ov38_02226214 = 4;
        goto fail;
    }

    h->url = ov38_02226218(prefix + len - pct * 2 + 1, 4);
    if (h->url == NULL) {
        ov38_02226214 = 1;
        goto fail;
    }
    ov38_0222336C(h->url, url, prefix);

    i = 0;
    j = 0;
    esc = 0;
    slash = FALSE;
    for (; i < len; i++) {
        if (esc == 2) {
            esc--;
        } else if (esc == 1) {
            char c = ov38_02224A98(host + (i - 1), 2);
            esc--;
            h->url[prefix + j - 1] = c;
            if (c == '/') {
                slash = TRUE;
            }
        } else {
            char c = host[i];
            if (c == '/') {
                slash = TRUE;
            }
            if (!slash && c == '%') {
                esc = 2;
            } else {
                h->url[prefix + j] = c;
            }
            j++;
        }
    }
    h->url[prefix + j] = '\0';

    p = h->url + prefix;
    for (i = 0; i < j; i++) {
        if (p[i] == '/' || p[i] == ':') {
            h->hostEnd = i + prefix;
            break;
        }
    }
    if (i == j) {
        h->pathStart = h->hostEnd = i + prefix;
    } else if (p[i] == '/') {
        h->pathStart = h->hostEnd;
    } else if (p[i] == ':') {
        for (; i < j; i++) {
            if (p[i] == '/') {
                h->pathStart = i + prefix;
                break;
            }
        }
        if (i == j) {
            h->pathStart = i + prefix;
        } else {
            int start = h->hostEnd + 1;
            int port = ov38_02224D58(h->url + start, h->pathStart - start);
            if (port < 0) {
                port = h->port;
            } else if (port > 0xFFFF) {
                ov38_02226214 = 4;
                goto fail;
            }
            h->port = (u16)port;
        }
    }

    if (h->secure) {
        int n = h->hostEnd - prefix;
        h->ssl = ov38_02226218(sizeof(HttpSecure), 4);
        if (h->ssl == NULL) {
            ov38_02226214 = 1;
            goto fail;
        }
        ov38_02223358(h->ssl, sizeof(HttpSecure));
        h->ssl->host = ov38_02226218(n + 1, 4);
        if (h->ssl->host == NULL) {
            ov38_02226214 = 1;
            goto fail;
        }
        ov38_0222336C(h->ssl->host, h->url + prefix, n);
        h->ssl->host[n] = '\0';
    }
    ov38_0222336C(h->config, ov38_02224F74, sizeof(h->config));
    h->param = param;
    h->mode = mode;
    h->callback = callback;
    return h;

fail:
    if (h != NULL) {
        if (h->ssl != NULL) {
            if (h->ssl->host != NULL) {
                ov38_0222621C(h->ssl->host);
            }
            ov38_0222621C(h->ssl);
        }
        if (h->url != NULL) {
            ov38_0222621C(h->url);
        }
        if (h->state != NULL) {
            ov38_0222621C(h->state);
        }
        ov38_0222621C(h);
    }
    return NULL;
}
