#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "../../../hardware/msm7k/libaudio-qdsp5v2/msm_audio.h"

int main(int argc, char **argv)
{
    int dev_id = (argc > 1) ? atoi(argv[1]) : 6;
    printf("Setting sound device to %d (6=SPEAKER, 0=HANDSET, 3=HEADSET)...\n", dev_id);
    int snd_fd = open("/dev/msm_snd", O_RDWR);
    if (snd_fd >= 0) {
        struct msm_snd_device_config dev;
        dev.device = dev_id;
        dev.ear_mute = 0;
        dev.mic_mute = 0;
        if (ioctl(snd_fd, SND_SET_DEVICE, &dev) < 0) {
            perror("SND_SET_DEVICE");
        } else {
            printf("SND_SET_DEVICE to %d succeeded!\n", dev_id);
        }

        struct msm_snd_volume_config vol;
        vol.device = dev_id;
        vol.method = 0;
        vol.volume = 100;
        ioctl(snd_fd, SND_SET_VOLUME, &vol);

        close(snd_fd);
    } else {
        perror("open /dev/msm_snd");
    }

    printf("Opening /dev/msm_pcm_out...\n");
    int pcm_fd = open("/dev/msm_pcm_out", O_RDWR);
    if (pcm_fd < 0) {
        perror("open /dev/msm_pcm_out");
        return 1;
    }

    struct msm_audio_config cfg;
    if (ioctl(pcm_fd, AUDIO_GET_CONFIG, &cfg) < 0) {
        perror("AUDIO_GET_CONFIG");
    } else {
        printf("Current config: ch=%d, rate=%d, buf_sz=%d, buf_cnt=%d\n",
               cfg.channel_count, cfg.sample_rate, cfg.buffer_size, cfg.buffer_count);
    }

    cfg.channel_count = 2;
    cfg.sample_rate = 44100;
    cfg.buffer_size = 4096;
    cfg.buffer_count = 4;

    if (ioctl(pcm_fd, AUDIO_SET_CONFIG, &cfg) < 0) {
        perror("AUDIO_SET_CONFIG");
    } else {
        printf("AUDIO_SET_CONFIG succeeded!\n");
    }

    if (ioctl(pcm_fd, AUDIO_START, 0) < 0) {
        perror("AUDIO_START");
    } else {
        printf("AUDIO_START succeeded!\n");
    }

    printf("Generating 440Hz tone for 2 seconds...\n");
    int sample_rate = 44100;
    int duration_sec = 2;
    int total_samples = sample_rate * duration_sec;
    int16_t *buf = malloc(total_samples * 2 * sizeof(int16_t));

    for (int i = 0; i < total_samples; i++) {
        int16_t s = (int16_t)(sin(2.0 * M_PI * 440.0 * i / sample_rate) * 25000.0);
        buf[i * 2] = s;     // Left
        buf[i * 2 + 1] = s; // Right
    }

    printf("Playing audio buffer (%d bytes)...\n", total_samples * 4);
    int written = 0;
    char *ptr = (char *)buf;
    int remaining = total_samples * 4;

    while (remaining > 0) {
        int chunk = remaining > 4096 ? 4096 : remaining;
        int ret = write(pcm_fd, ptr, chunk);
        if (ret > 0) {
            ptr += ret;
            remaining -= ret;
            written += ret;
        } else {
            perror("write pcm");
            break;
        }
    }

    printf("Done writing %d bytes, waiting for playback...\n", written);
    sleep(2);

    close(pcm_fd);
    free(buf);
    printf("Test finished successfully!\n");
    return 0;
}
