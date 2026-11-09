#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LOG_TAG "rmt_storage"
#include <cutils/log.h>

#define RMT_STORAGE_OPEN              0
#define RMT_STORAGE_WRITE             1
#define RMT_STORAGE_CLOSE             2
#define RMT_STORAGE_SEND_USER_DATA    3
#define RMT_STORAGE_READ              4
#define RMT_STORAGE_NOOP              255

#define RMT_STORAGE_MAX_IOVEC_XFR_CNT 5
#define MAX_NUM_CLIENTS 10
#define MAX_PATH_NAME 32
#define RAMFS_MODEMSTORAGE_ID 0x4D454653

enum {
    RMT_STORAGE_NO_ERROR = 0,
    RMT_STORAGE_ERROR_PARAM,
    RMT_STORAGE_ERROR_PIPE,
    RMT_STORAGE_ERROR_UNINIT,
    RMT_STORAGE_ERROR_BUSY,
    RMT_STORAGE_ERROR_DEVICE
};

enum {
    RMT_STORAGE_EVNT_OPEN = 0,
    RMT_STORAGE_EVNT_CLOSE = 1,
    RMT_STORAGE_EVNT_WRITE_BLOCK = 2,
    RMT_STORAGE_EVNT_GET_DEV_ERROR = 3,
    RMT_STORAGE_EVNT_WRITE_IOVEC = 4,
    RMT_STORAGE_EVNT_SEND_USER_DATA = 5,
    RMT_STORAGE_EVNT_READ_IOVEC = 6,
    RMT_STORAGE_EVNT_ALLOC_RMT_BUF = 7,
};

struct rmt_storage_iovec_desc {
    uint32_t sector_addr;
    uint32_t data_phy_addr;
    uint32_t num_sector;
};

struct rmt_storage_event {
    uint32_t id;
    uint32_t sid;
    uint32_t handle;
    char path[MAX_PATH_NAME];
    struct rmt_storage_iovec_desc xfer_desc[RMT_STORAGE_MAX_IOVEC_XFR_CNT];
    uint32_t xfer_cnt;
    uint32_t usr_data;
};

struct rmt_storage_send_sts {
    uint32_t err_code;
    uint32_t data;
    uint32_t handle;
    uint32_t xfer_dir;
};

struct rmt_shrd_mem_param {
    uint32_t sid;
    uint32_t start;
    uint32_t size;
    void *base;
};

#define RMT_STORAGE_IOCTL_MAGIC (0xC2)
#define RMT_STORAGE_SHRD_MEM_PARAM _IOWR(RMT_STORAGE_IOCTL_MAGIC, 0, struct rmt_shrd_mem_param)
#define RMT_STORAGE_WAIT_FOR_REQ   _IOR(RMT_STORAGE_IOCTL_MAGIC, 1, struct rmt_storage_event)
#define RMT_STORAGE_SEND_STATUS    _IOW(RMT_STORAGE_IOCTL_MAGIC, 2, struct rmt_storage_send_sts)

struct client_info {
    int fd;
    char path[MAX_PATH_NAME];
};

static struct client_info clients[MAX_NUM_CLIENTS];
static void *shrd_mem_base = NULL;
static uint32_t shrd_mem_start = 0;
static uint32_t shrd_mem_size = 0;

static const char *part_paths[MAX_NUM_CLIENTS] = { NULL };
static int num_part_paths = 0;

static int get_block_device_for_path(const char *path)
{
    if (strstr(path, "modem_fs1") || strstr(path, "fs1")) {
        if (num_part_paths > 0 && part_paths[0]) return open(part_paths[0], O_RDWR | O_SYNC);
        return open("/dev/block/mmcblk0p12", O_RDWR | O_SYNC);
    }
    if (strstr(path, "modem_fs2") || strstr(path, "fs2")) {
        if (num_part_paths > 1 && part_paths[1]) return open(part_paths[1], O_RDWR | O_SYNC);
        return open("/dev/block/mmcblk0p13", O_RDWR | O_SYNC);
    }
    if (strstr(path, "modem_fsg") || strstr(path, "fsg")) {
        if (num_part_paths > 2 && part_paths[2]) return open(part_paths[2], O_RDWR | O_SYNC);
        return open("/dev/block/mmcblk0p14", O_RDWR | O_SYNC);
    }
    return open(path, O_RDWR | O_SYNC);
}

int main(int argc, char **argv)
{
    LOGI("rmt_storage daemon starting...");
    int dev_fd = open("/dev/rmt_storage", O_RDWR);
    if (dev_fd < 0) {
        LOGE("Failed to open /dev/rmt_storage: %s", strerror(errno));
        return 1;
    }

    for (int i = 1; i < argc && i <= MAX_NUM_CLIENTS; i++) {
        part_paths[i - 1] = argv[i];
        num_part_paths++;
        LOGI("Partition arg[%d]: %s", i - 1, argv[i]);
    }

    struct rmt_shrd_mem_param param;
    memset(&param, 0, sizeof(param));
    param.sid = RAMFS_MODEMSTORAGE_ID;

    if (ioctl(dev_fd, RMT_STORAGE_SHRD_MEM_PARAM, &param) < 0) {
        LOGE("RMT_STORAGE_SHRD_MEM_PARAM failed: %s", strerror(errno));
        close(dev_fd);
        return 1;
    }

    shrd_mem_start = param.start;
    shrd_mem_size = param.size;
    LOGI("Shared memory: start=0x%08x, size=0x%08x", shrd_mem_start, shrd_mem_size);

    shrd_mem_base = mmap(NULL, shrd_mem_size, PROT_READ | PROT_WRITE,
                         MAP_SHARED, dev_fd, (off_t)shrd_mem_start);
    if (shrd_mem_base == MAP_FAILED) {
        LOGE("mmap failed: %s", strerror(errno));
        close(dev_fd);
        return 1;
    }
    LOGI("mmap succeeded at %p", shrd_mem_base);

    for (int i = 0; i < MAX_NUM_CLIENTS; i++) {
        clients[i].fd = -1;
    }

    while (1) {
        struct rmt_storage_event event;
        memset(&event, 0, sizeof(event));

        int ret = ioctl(dev_fd, RMT_STORAGE_WAIT_FOR_REQ, &event);
        if (ret < 0) {
            if (errno == EINTR) continue;
            LOGE("RMT_STORAGE_WAIT_FOR_REQ error: %s", strerror(errno));
            sleep(1);
            continue;
        }

        LOGI("Received event: id=%d, handle=%d, path=%s, xfer_cnt=%d",
             event.id, event.handle, event.path, event.xfer_cnt);

        struct rmt_storage_send_sts sts;
        memset(&sts, 0, sizeof(sts));
        sts.handle = event.handle;
        sts.err_code = RMT_STORAGE_NO_ERROR;

        if (event.id == RMT_STORAGE_EVNT_OPEN) {
            int handle = event.handle;
            if (handle > 0 && handle < MAX_NUM_CLIENTS) {
                if (clients[handle].fd < 0) {
                    clients[handle].fd = get_block_device_for_path(event.path);
                    strncpy(clients[handle].path, event.path, MAX_PATH_NAME);
                    LOGI("Opened block device for %s (handle=%d) -> fd=%d",
                         event.path, handle, clients[handle].fd);
                    if (clients[handle].fd < 0) {
                        LOGE("Failed to open block device for %s: %s",
                             event.path, strerror(errno));
                        sts.err_code = RMT_STORAGE_ERROR_DEVICE;
                    }
                }
            } else {
                sts.err_code = RMT_STORAGE_ERROR_PARAM;
            }
            sts.xfer_dir = 0;
            if (ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts) < 0) {
                LOGE("SEND_STATUS (OPEN) failed: %s", strerror(errno));
            }
        } else if (event.id == RMT_STORAGE_EVNT_WRITE_IOVEC) {
            int handle = event.handle;
            sts.xfer_dir = RMT_STORAGE_WRITE;
            if (handle > 0 && handle < MAX_NUM_CLIENTS && clients[handle].fd >= 0) {
                int blk_fd = clients[handle].fd;
                for (uint32_t i = 0; i < event.xfer_cnt; i++) {
                    uint32_t sec = event.xfer_desc[i].sector_addr;
                    uint32_t num = event.xfer_desc[i].num_sector;
                    uint32_t paddr = event.xfer_desc[i].data_phy_addr;

                    if (paddr >= shrd_mem_start &&
                        (paddr + num * 512) <= (shrd_mem_start + shrd_mem_size)) {
                        void *buf = (char *)shrd_mem_base + (paddr - shrd_mem_start);
                        off_t offset = (off_t)sec * 512;
                        size_t count = (size_t)num * 512;
                        ssize_t nw = pwrite(blk_fd, buf, count, offset);
                        if (nw != (ssize_t)count) {
                            LOGE("pwrite failed: sec=%u, num=%u, ret=%ld (%s)",
                                 sec, num, (long)nw, strerror(errno));
                            sts.err_code = RMT_STORAGE_ERROR_DEVICE;
                            break;
                        }
                    } else {
                        LOGE("Invalid paddr 0x%08x for size %u", paddr, num * 512);
                        sts.err_code = RMT_STORAGE_ERROR_PARAM;
                        break;
                    }
                }
                fsync(blk_fd);
            } else {
                LOGE("Invalid client handle %d for WRITE", handle);
                sts.err_code = RMT_STORAGE_ERROR_PARAM;
            }
            if (ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts) < 0) {
                LOGE("SEND_STATUS (WRITE) failed: %s", strerror(errno));
            }
        } else if (event.id == RMT_STORAGE_EVNT_READ_IOVEC) {
            int handle = event.handle;
            sts.xfer_dir = RMT_STORAGE_READ;
            if (handle > 0 && handle < MAX_NUM_CLIENTS && clients[handle].fd >= 0) {
                int blk_fd = clients[handle].fd;
                for (uint32_t i = 0; i < event.xfer_cnt; i++) {
                    uint32_t sec = event.xfer_desc[i].sector_addr;
                    uint32_t num = event.xfer_desc[i].num_sector;
                    uint32_t paddr = event.xfer_desc[i].data_phy_addr;

                    if (paddr >= shrd_mem_start &&
                        (paddr + num * 512) <= (shrd_mem_start + shrd_mem_size)) {
                        void *buf = (char *)shrd_mem_base + (paddr - shrd_mem_start);
                        off_t offset = (off_t)sec * 512;
                        size_t count = (size_t)num * 512;
                        ssize_t nr = pread(blk_fd, buf, count, offset);
                        if (nr != (ssize_t)count) {
                            LOGE("pread failed: sec=%u, num=%u, ret=%ld (%s)",
                                 sec, num, (long)nr, strerror(errno));
                            sts.err_code = RMT_STORAGE_ERROR_DEVICE;
                            break;
                        }
                    } else {
                        LOGE("Invalid paddr 0x%08x for size %u", paddr, num * 512);
                        sts.err_code = RMT_STORAGE_ERROR_PARAM;
                        break;
                    }
                }
            } else {
                LOGE("Invalid client handle %d for READ", handle);
                sts.err_code = RMT_STORAGE_ERROR_PARAM;
            }
            if (ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts) < 0) {
                LOGE("SEND_STATUS (READ) failed: %s", strerror(errno));
            }
        } else if (event.id == RMT_STORAGE_EVNT_CLOSE) {
            int handle = event.handle;
            if (handle > 0 && handle < MAX_NUM_CLIENTS && clients[handle].fd >= 0) {
                close(clients[handle].fd);
                clients[handle].fd = -1;
                LOGI("Closed client %d", handle);
            }
            sts.xfer_dir = 0;
            if (ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts) < 0) {
                LOGE("SEND_STATUS (CLOSE) failed: %s", strerror(errno));
            }
        } else if (event.id == RMT_STORAGE_EVNT_SEND_USER_DATA) {
            sts.xfer_dir = 0;
            if (ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts) < 0) {
                LOGE("SEND_STATUS (USER_DATA) failed: %s", strerror(errno));
            }
        } else {
            LOGI("Unhandled event id: %d", event.id);
            sts.xfer_dir = 0;
            ioctl(dev_fd, RMT_STORAGE_SEND_STATUS, &sts);
        }
    }

    close(dev_fd);
    return 0;
}
