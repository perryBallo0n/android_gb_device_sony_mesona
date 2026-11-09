#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/wireless.h>
#ifndef ARPHRD_ETHER
#define ARPHRD_ETHER 1
#endif

#define IFNAME "wlan0"

static int sock = -1;

static int open_sock(void) {
    if (sock < 0) {
        sock = socket(PF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("socket(PF_INET, SOCK_DGRAM)");
            return -1;
        }
    }
    return sock;
}

static int do_ifup(int up) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    if (ioctl(open_sock(), SIOCGIFFLAGS, &ifr) < 0) {
        perror("SIOCGIFFLAGS");
        return -1;
    }
    if (up) {
        ifr.ifr_flags |= IFF_UP;
    } else {
        ifr.ifr_flags &= ~IFF_UP;
    }
    if (ioctl(open_sock(), SIOCSIFFLAGS, &ifr) < 0) {
        perror("SIOCSIFFLAGS");
        return -1;
    }
    printf("Interface %s set %s\n", IFNAME, up ? "UP" : "DOWN");
    return 0;
}

static int do_info(void) {
    struct ifreq ifr;
    struct iwreq iwr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    if (ioctl(open_sock(), SIOCGIFHWADDR, &ifr) == 0) {
        unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
        printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        perror("SIOCGIFHWADDR");
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    if (ioctl(open_sock(), SIOCGIWNAME, &iwr) == 0) {
        printf("Wireless Name: %s\n", iwr.u.name);
    } else {
        perror("SIOCGIWNAME");
    }

    struct iw_range range;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t)&range;
    iwr.u.data.length = sizeof(range);
    iwr.u.data.flags = 0;
    if (ioctl(open_sock(), SIOCGIWRANGE, &iwr) == 0) {
        printf("SIOCGIWRANGE success: we_version_compiled=%d, we_version_source=%d\n",
               range.we_version_compiled, range.we_version_source);
    } else {
        perror("SIOCGIWRANGE");
    }

    return 0;
}

static int do_mode(void) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.mode = IW_MODE_INFRA;
    printf("Calling SIOCSIWMODE (IW_MODE_INFRA)...\n"); fflush(stdout);
    if (ioctl(open_sock(), SIOCSIWMODE, &iwr) == 0) {
        printf("SIOCSIWMODE success\n");
    } else {
        perror("SIOCSIWMODE");
    }
    return 0;
}

static int do_disc(void) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.ap_addr.sa_family = ARPHRD_ETHER;
    memset(iwr.u.ap_addr.sa_data, 0, 6);
    printf("Calling SIOCSIWAP (00:00:00:00:00:00)...\n"); fflush(stdout);
    if (ioctl(open_sock(), SIOCSIWAP, &iwr) == 0) {
        printf("SIOCSIWAP success\n");
    } else {
        perror("SIOCSIWAP");
    }
    return 0;
}

static int do_auth(int idx, int value) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.param.flags = idx & IW_AUTH_INDEX;
    iwr.u.param.value = value;
    printf("Calling SIOCSIWAUTH (idx=%d, val=%d)...\n", idx, value); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWAUTH, &iwr);
    if (ret == 0) {
        printf("SIOCSIWAUTH success\n");
    } else {
        perror("SIOCSIWAUTH");
    }
    return ret;
}

static int do_freq(int freq) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.freq.m = freq * 100000;
    iwr.u.freq.e = 1;
    printf("Calling SIOCSIWFREQ (%d MHz)...\n", freq); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWFREQ, &iwr);
    if (ret == 0) {
        printf("SIOCSIWFREQ success\n");
    } else {
        perror("SIOCSIWFREQ");
    }
    return ret;
}

static int do_essid(const char *ssid) {
    struct iwreq iwr;
    char buf[33];
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    memset(buf, 0, sizeof(buf));
    strncpy(buf, ssid, 32);
    iwr.u.essid.pointer = (caddr_t)buf;
    iwr.u.essid.length = strlen(buf);
    iwr.u.essid.flags = 1;
    printf("Calling SIOCSIWESSID ('%s', len=%d)...\n", buf, iwr.u.essid.length); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWESSID, &iwr);
    if (ret == 0) {
        printf("SIOCSIWESSID success\n");
    } else {
        perror("SIOCSIWESSID");
    }
    return ret;
}

static int do_wap(const char *mac_str) {
    struct iwreq iwr;
    unsigned int mac[6];
    int i;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.ap_addr.sa_family = ARPHRD_ETHER;
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6) {
        for (i = 0; i < 6; i++) iwr.u.ap_addr.sa_data[i] = (unsigned char)mac[i];
    } else {
        printf("Invalid MAC format: %s\n", mac_str);
        return -1;
    }
    printf("Calling SIOCSIWAP (%s)...\n", mac_str); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWAP, &iwr);
    if (ret == 0) {
        printf("SIOCSIWAP success\n");
    } else {
        perror("SIOCSIWAP");
    }
    return ret;
}

#define WLC_SCAN 50
#define WLC_SCAN_RESULTS 51

typedef struct wl_ioctl {
    uint32_t cmd;
    void *buf;
    uint32_t len;
    uint8_t set;
    uint32_t used;
    uint32_t needed;
} wl_ioctl_t;

typedef struct wlc_ssid {
    uint32_t SSID_len;
    unsigned char SSID[32];
} wlc_ssid_t;

typedef struct wl_bss_info {
    uint32_t version;
    uint32_t length;
    unsigned char BSSID[6];
    uint16_t beacon_period;
    uint16_t capability;
    uint8_t  SSID_len;
    uint8_t  SSID[32];
    struct {
        uint32_t count;
        uint8_t rates[16];
    } rateset;
    uint16_t chanspec;
    uint16_t atim_window;
    uint8_t  dtim_period;
    int16_t  RSSI;
    int8_t   phy_noise;
    uint8_t  n_cap;
    uint32_t nbss_cap;
    uint8_t  ctl_ch;
    uint32_t reserved32[1];
    uint8_t  flags;
    uint8_t  reserved[3];
    uint8_t  basic_mcs[16];
    uint16_t ie_offset;
    uint32_t ie_length;
    int16_t  SNR;
} wl_bss_info_t;

typedef struct wl_scan_results {
    uint32_t buflen;
    uint32_t version;
    uint32_t count;
    wl_bss_info_t bss_info[1];
} wl_scan_results_t;

static int do_wlc_scan(void) {
    struct ifreq ifr;
    wl_ioctl_t ioc;
    wlc_ssid_t ssid;

    memset(&ssid, 0, sizeof(ssid));
    memset(&ioc, 0, sizeof(ioc));
    ioc.cmd = WLC_SCAN;
    ioc.buf = &ssid;
    ioc.len = sizeof(ssid);
    ioc.set = 1;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    ifr.ifr_data = (caddr_t)&ioc;

    printf("Calling SIOCDEVPRIVATE (WLC_SCAN 50)...\n"); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCDEVPRIVATE, &ifr);
    if (ret >= 0) {
        printf("WLC_SCAN success, ret=%d\n", ret);
    } else {
        perror("WLC_SCAN (SIOCDEVPRIVATE)");
    }
    return ret;
}

static int do_wlc_results(void) {
    struct ifreq ifr;
    wl_ioctl_t ioc;
    char buf[8192];
    wl_scan_results_t *results = (wl_scan_results_t *)buf;

    memset(buf, 0, sizeof(buf));
    results->buflen = sizeof(buf);

    memset(&ioc, 0, sizeof(ioc));
    ioc.cmd = WLC_SCAN_RESULTS;
    ioc.buf = buf;
    ioc.len = sizeof(buf);
    ioc.set = 0;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    ifr.ifr_data = (caddr_t)&ioc;

    printf("Calling SIOCDEVPRIVATE (WLC_SCAN_RESULTS 51)...\n"); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCDEVPRIVATE, &ifr);
    if (ret >= 0) {
        printf("WLC_SCAN_RESULTS success! count=%u\n", results->count);
        wl_bss_info_t *bi = results->bss_info;
        for (uint32_t i = 0; i < results->count; i++) {
            char ssid_str[33];
            memset(ssid_str, 0, sizeof(ssid_str));
            memcpy(ssid_str, bi->SSID, bi->SSID_len < 32 ? bi->SSID_len : 32);
            unsigned char *mac = (unsigned char *)&bi->BSSID;
            printf("  [%u] BSSID: %02x:%02x:%02x:%02x:%02x:%02x, Ch: %2u, RSSI: %3d dBm, IEs: %4u B, SSID: \"%s\"\n",
                   i, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
                   bi->ctl_ch, bi->RSSI, bi->ie_length, ssid_str);
            bi = (wl_bss_info_t *)((char *)bi + bi->length);
        }
    } else {
        perror("WLC_SCAN_RESULTS (SIOCDEVPRIVATE)");
    }
    return ret;
}

typedef struct android_wifi_priv_cmd {
	char *buf;
	int used_len;
	int total_len;
} android_wifi_priv_cmd;

static int do_android_priv(const char *cmd) {
    struct ifreq ifr;
    android_wifi_priv_cmd priv_cmd;
    char buf[256];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, cmd, sizeof(buf) - 1);

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    priv_cmd.buf = buf;
    priv_cmd.used_len = strlen(buf);
    priv_cmd.total_len = sizeof(buf);
    ifr.ifr_data = (caddr_t)&priv_cmd;

    printf("Calling SIOCDEVPRIVATE+1 ('%s')...\n", cmd); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCDEVPRIVATE + 1, &ifr);
    if (ret >= 0) {
        printf("SIOCDEVPRIVATE+1 success, ret=%d reply: '%s'\n", ret, buf);
    } else {
        perror("SIOCDEVPRIVATE+1");
    }
    return ret;
}

static int do_priv(const char *cmd) {
    struct iwreq iwr;
    char buf[256];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, cmd, sizeof(buf) - 1);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.data.pointer = buf;
    iwr.u.data.length = strlen(buf);
    iwr.u.data.flags = 0;

    printf("Calling SIOCSIWPRIV ('%s')...\n", cmd); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWPRIV, &iwr);
    if (ret == 0) {
        printf("SIOCSIWPRIV success, reply: '%s'\n", buf);
    } else {
        perror("SIOCSIWPRIV");
    }
    return ret;
}

static int do_bcm_ioctl(int cmd, void *buf, int len, int set) {
    struct ifreq ifr;
    struct {
        wl_ioctl_t ioc;
        uint32_t driver;
    } ioc_buf;

    memset(&ioc_buf, 0, sizeof(ioc_buf));
    ioc_buf.ioc.cmd = cmd;
    ioc_buf.ioc.buf = buf;
    ioc_buf.ioc.len = len;
    ioc_buf.ioc.set = set;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, IFNAME, IFNAMSIZ);
    ifr.ifr_data = (caddr_t)&ioc_buf;

    printf("Calling SIOCDEVPRIVATE cmd=%d len=%d set=%d...\n", cmd, len, set); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCDEVPRIVATE, &ifr);
    if (ret == 0) {
        printf("SIOCDEVPRIVATE cmd=%d success!\n", cmd);
    } else {
        perror("SIOCDEVPRIVATE");
    }
    return ret;
}

static int do_test_wpa_auth(int wpa_auth, int wsec) {
    printf("Testing direct WLC_SET_WSEC (%d) and WLC_SET_WPA_AUTH (0x%x)...\n", wsec, wpa_auth);
    int ret = do_bcm_ioctl(134, &wsec, sizeof(wsec), 1);
    if (ret != 0) return ret;
    ret = do_bcm_ioctl(165, &wpa_auth, sizeof(wpa_auth), 1);
    return ret;
}

static int do_bcm_assoc(const char *ssid_name, int wpa_auth, int wsec) {
    int infra = 1;
    int auth = 0;
    wlc_ssid_t s;
    memset(&s, 0, sizeof(s));
    s.SSID_len = strlen(ssid_name);
    memcpy(s.SSID, ssid_name, s.SSID_len);

    printf("1. Set WSEC (%d)...\n", wsec);
    if (do_bcm_ioctl(134, &wsec, sizeof(wsec), 1) < 0) return -1;

    printf("2. Set WPA_AUTH (0x%x)...\n", wpa_auth);
    if (do_bcm_ioctl(165, &wpa_auth, sizeof(wpa_auth), 1) < 0) return -1;

    printf("3. Set INFRA (%d)...\n", infra);
    if (do_bcm_ioctl(20, &infra, sizeof(infra), 1) < 0) return -1;

    printf("4. Set AUTH (%d)...\n", auth);
    if (do_bcm_ioctl(22, &auth, sizeof(auth), 1) < 0) return -1;

    printf("5. Set SSID ('%s')...\n", ssid_name);
    if (do_bcm_ioctl(26, &s, sizeof(s), 1) < 0) return -1;

    printf("Association request sent successfully!\n");
    return 0;
}

static int do_wext_scan(void) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    printf("Calling SIOCSIWSCAN...\n"); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCSIWSCAN, &iwr);
    if (ret == 0) {
        printf("SIOCSIWSCAN success!\n");
    } else {
        perror("SIOCSIWSCAN");
    }
    return ret;
}

static int do_results(void) {
    struct iwreq iwr;
    char buffer[4096];
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, IFNAME, IFNAMSIZ);
    iwr.u.data.pointer = buffer;
    iwr.u.data.length = sizeof(buffer);
    iwr.u.data.flags = 0;

    printf("Calling SIOCGIWSCAN...\n"); fflush(stdout);
    int ret = ioctl(open_sock(), SIOCGIWSCAN, &iwr);
    if (ret == 0) {
        printf("SIOCGIWSCAN success! Received %d bytes of scan results.\n", iwr.u.data.length);
    } else {
        perror("SIOCGIWSCAN");
    }
    return ret;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [up|down|info|mode|disc|scan|results|priv <cmd>]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "up") == 0) {
        return do_ifup(1);
    } else if (strcmp(argv[1], "down") == 0) {
        return do_ifup(0);
    } else if (strcmp(argv[1], "info") == 0) {
        return do_info();
    } else if (strcmp(argv[1], "mode") == 0) {
        return do_mode();
    } else if (strcmp(argv[1], "disc") == 0) {
        return do_disc();
    } else if (strcmp(argv[1], "scan") == 0) {
        return do_wext_scan();
    } else if (strcmp(argv[1], "wlc_scan") == 0) {
        return do_wlc_scan();
    } else if (strcmp(argv[1], "wlc_results") == 0) {
        return do_wlc_results();
    } else if (strcmp(argv[1], "results") == 0) {
        return do_results();
    } else if (strcmp(argv[1], "priv") == 0) {
        if (argc < 3) {
            printf("Usage: %s priv <cmd>\n", argv[0]);
            return 1;
        }
        return do_priv(argv[2]);
    } else if (strcmp(argv[1], "apriv") == 0) {
        if (argc < 3) {
            printf("Usage: %s apriv <cmd>\n", argv[0]);
            return 1;
        }
        return do_android_priv(argv[2]);
    } else if (strcmp(argv[1], "bcm_assoc") == 0) {
        if (argc < 3) { printf("Usage: %s bcm_assoc <ssid> [wpa_auth] [wsec]\n", argv[0]); return 1; }
        int wpa_auth = 0x0080; /* WPA2_AUTH_PSK */
        int wsec = 4; /* AES */
        if (argc >= 4) wpa_auth = strtol(argv[3], NULL, 0);
        if (argc >= 5) wsec = strtol(argv[4], NULL, 0);
        return do_bcm_assoc(argv[2], wpa_auth, wsec);
    } else if (strcmp(argv[1], "bcm_auth") == 0) {
        int wpa_auth = 0x0080; /* WPA2_AUTH_PSK */
        int wsec = 4; /* AES */
        if (argc >= 3) wpa_auth = strtol(argv[2], NULL, 0);
        if (argc >= 4) wsec = strtol(argv[3], NULL, 0);
        return do_test_wpa_auth(wpa_auth, wsec);
    } else if (strcmp(argv[1], "auth") == 0) {
        if (argc < 4) { printf("Usage: %s auth <idx> <val>\n", argv[0]); return 1; }
        return do_auth(atoi(argv[2]), atoi(argv[3]));
    } else if (strcmp(argv[1], "freq") == 0) {
        if (argc < 3) { printf("Usage: %s freq <mhz>\n", argv[0]); return 1; }
        return do_freq(atoi(argv[2]));
    } else if (strcmp(argv[1], "essid") == 0) {
        if (argc < 3) { printf("Usage: %s essid <ssid>\n", argv[0]); return 1; }
        return do_essid(argv[2]);
    } else if (strcmp(argv[1], "wap") == 0) {
        if (argc < 3) { printf("Usage: %s wap <bssid>\n", argv[0]); return 1; }
        return do_wap(argv[2]);
    } else {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}
