#include "audio_resample.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERY_SMALL (1e-30f)

static int resampling_mono(short *inpcm, short *outpcm, int srcrate, int tarrate,
        int innum, RESAMPLE *resample) {
    int outnum = 0;
    int n = 0;
    int n_1 = 0;
    int value = 0;
    int pcmval[2];
    float nk = 0;
    float w[2];
    float ratio = (float)srcrate / (float)tarrate;

    while (1) {
        nk = ratio * (resample->outnum + outnum);
        n = (int)nk;
        w[0] = nk - n;
        w[1] = 1 - w[0];

        n = n - resample->innum;
        n_1 = n + 1;

        if (n_1 >= innum) {
            memcpy(resample->inpcm, &inpcm[innum - INPCM_DELAY_SIZE],
                    sizeof(short) * INPCM_DELAY_SIZE);
            break;
        }

        if (n < 0) {
            pcmval[0] = resample->inpcm[INPCM_DELAY_SIZE + n];
        } else {
            pcmval[0] = inpcm[n];
        }

        if (n_1 < 0) {
            pcmval[1] = resample->inpcm[INPCM_DELAY_SIZE + n_1];
        } else {
            pcmval[1] = inpcm[n_1];
        }

        value = w[0] * pcmval[1] + w[1] * pcmval[0];
        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[outnum] = value;
        outnum++;
    }

    return outnum;
}

int resampling_dual(short *inpcm, short *outpcm, int srcrate, int tarrate,
        int innum, RESAMPLE *resample) {
    int outnum = 0;
    int n_1 = 0;
    int value = 0;
    int pcmval[2];

    float nk = 0.0;
    int n = 0;
    float w[2];
    float ratio = (float)srcrate / (float)tarrate;

    while (1) {
        nk = ratio * (resample->outnum + outnum);
        n = (int)nk;
        w[0] = nk - n;
        w[1] = 1 - w[0];

        n = n - resample->innum;
        n_1 = n + 1;

        if (n_1 >= innum) {
            memcpy(resample->inpcm, &inpcm[innum * 2 - INPCM_DELAY_SIZE * 2],
                    sizeof(short) * INPCM_DELAY_SIZE * 2);
            break;
        }

        //first channel
        if (n < 0) {
            pcmval[0] = resample->inpcm[INPCM_DELAY_SIZE * 2 + 2 * n];
        } else {
            pcmval[0] = inpcm[2 * n];
        }

        if (n_1 < 0) {
            pcmval[1] = resample->inpcm[INPCM_DELAY_SIZE * 2 + 2 * n_1];
        } else {
            pcmval[1] = inpcm[2 * n_1];
        }

        value = w[0] * pcmval[1] + w[1] * pcmval[0];
        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[outnum * 2] = value;

        //second channel
        if (n < 0) {
            pcmval[0] = resample->inpcm[INPCM_DELAY_SIZE * 2 + 2 * n + 1];
        } else {
            pcmval[0] = inpcm[2 * n + 1];
        }
        if (n_1 < 0) {
            pcmval[1] = resample->inpcm[INPCM_DELAY_SIZE * 2 + 2 * n_1 + 1];
        } else {
            pcmval[1] = inpcm[2 * n_1 + 1];
        }

        value = w[0] * pcmval[1] + w[1] * pcmval[0];
        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[outnum * 2 + 1] = value;
        outnum++;
    }

    return outnum;
}

int resampling_dual_down_ch(short *inpcm, int channelidx, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample) {
    int outnum = 0;
    int i = 0;
    int n = 0;
    int n_1 = 0;
    int value = 0;
    int pcmval[2];
    float nk = 0.0;
    float w[2];
    float ratio = (float)srcrate / (float)tarrate;

    while (1) {
        nk = ratio * (resample->outnum + outnum);
        n = (int)nk;
        w[0] = nk - n;
        w[1] = 1 - w[0];

        n = n - resample->innum;
        n_1 = n + 1;

        if (n_1 >= innum) {
            for (i = 0; i < INPCM_DELAY_SIZE; i++)
            {
                resample->inpcm[i] = inpcm[(innum - INPCM_DELAY_SIZE) * 2 + channelidx + 2 * i];
            }
            break;
        }

        if (n < 0) {
            pcmval[0] = resample->inpcm[INPCM_DELAY_SIZE + n];
        } else {
            pcmval[0] = inpcm[n * 2 + channelidx];
        }

        if (n_1 < 0) {
            pcmval[1] = resample->inpcm[INPCM_DELAY_SIZE + n_1];
        } else {
            pcmval[1] = inpcm[n_1 * 2 + channelidx];
        }
        value = w[0] * pcmval[1] + w[1] * pcmval[0];

        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[outnum] = value;
        outnum++;
    }

    return outnum;
}

static void dc_reject_mono(short *inpcm, int cutoff_Hz, short *outpcm, float *hp_mem,
        int len, int Fs) {
    int i = 0, value = 0;
    float coef = 0.0;

    coef = 4.0f * cutoff_Hz / Fs;
    for (i = 0; i < len; i++) {
        float x = 0.0, tmp = 0, y = 0;
        x = inpcm[i];
        //stage 1
        tmp = x - hp_mem[0];
        hp_mem[0] = hp_mem[0] + coef * (x - hp_mem[0]) + VERY_SMALL;
        //stage 2
        y = tmp - hp_mem[1];
        hp_mem[1] = hp_mem[1] + coef * (tmp - hp_mem[1]) + VERY_SMALL;
        value = y;

        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[i] = value;
    }
}

static void dc_reject_dual(short *inpcm, int cutoff_Hz, short *outpcm, float *hp_mem,
        int len, int Fs) {
    int i = 0, value = 0;
    float coef = 0.0;

    coef = 4.0f * cutoff_Hz / Fs;

    //first channel
    for (i = 0; i < len; i += 2) {
        float x = 0.0, tmp = 0, y = 0;
        x = inpcm[i];
        //stage 1
        tmp = x - hp_mem[0];
        hp_mem[0] = hp_mem[0] + coef * (x - hp_mem[0]) + VERY_SMALL;
        //stage 2
        y = tmp - hp_mem[1];
        hp_mem[1] = hp_mem[1] + coef * (tmp - hp_mem[1]) + VERY_SMALL;
        value = y;
        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[i] = value;
        //fprintf(debugfile, "%d : %f : %f => %d\n", outcnt++, hp_mem[0], hp_mem[1], value);
    }

    //second channel
    for (i = 1; i < len; i += 2) {
        float x = 0.0, tmp = 0, y = 0;
        x = inpcm[i];
        /* First stage */
        tmp = x - hp_mem[2];
        hp_mem[2] = hp_mem[2] + coef * (x - hp_mem[2]) + VERY_SMALL;
        /* Second stage */
        y = tmp - hp_mem[3];
        hp_mem[3] = hp_mem[3] + coef * (tmp - hp_mem[3]) + VERY_SMALL;
        value = y;
        if (value > 32767) {
            value = 32767;
        }
        if (value < -32768) {
            value = -32768;
        }
        outpcm[i] = value;
    }
}

static int resampling_process_mono(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample) {
    int outnum = 0;

    outnum = resampling_mono(inpcm, outpcm, srcrate, tarrate, innum, resample);
    resample->innum += innum;
    resample->outnum += outnum;

    if ((resample->innum >= srcrate) && (resample->outnum >= tarrate)) {
        resample->innum -= srcrate;
        resample->outnum -= tarrate;
    }

    dc_reject_mono(outpcm, 3, outpcm, resample->hp_mem, outnum, tarrate);

    return outnum;
}

static int resampling_process_dual(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample) {
    int outnum = 0;
    innum /= 2;

    outnum = resampling_dual(inpcm, outpcm, srcrate, tarrate, innum, resample);
    resample->innum += innum;
    resample->outnum += outnum;

    //fprintf(debugfile,"outnum = %d : %d : %d : %d\n", outnum, innum, resample->innum, resample->outnum);

    if ((resample->innum >= srcrate) && (resample->outnum >= tarrate)) {
        resample->innum -= srcrate;
        resample->outnum -= tarrate;
    }

    outnum *= 2;
    dc_reject_dual(outpcm, 3, outpcm, resample->hp_mem, outnum, tarrate);

    return outnum;
}

static int resampling_process_dual_down_ch(short *inpcm, int channelidx, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample) {
    int outnum = 0;
    innum /= 2;

    outnum = resampling_dual_down_ch(inpcm, channelidx, outpcm, srcrate, tarrate, innum, resample);
    resample->innum += innum;
    resample->outnum += outnum;

    if ((resample->innum >= srcrate) && (resample->outnum >= tarrate)) {
        resample->innum -= srcrate;
        resample->outnum -= tarrate;
    }

    dc_reject_mono(outpcm, 3, outpcm, resample->hp_mem, outnum, tarrate);

    return outnum;
}

int resampling_process(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, int channel, RESAMPLE *resample) {
    int outnum = 0;
    int i = 0;

    if ((innum % channel) != 0) {
        printf("input pcm number error. For two channel case, left & right channel should has same number\n");
        return 0;
    }

    if (srcrate == tarrate) {
        memcpy(outpcm, inpcm, sizeof(short)*innum);
        outnum = innum;
    } else { ////attention, perhaps the output number has several point more or less, so need to check and adjust
        if (channel == 1)
        {
            outnum = resampling_process_mono(inpcm, outpcm, srcrate, tarrate, innum, resample);
        }
        else
        {
            outnum = resampling_process_dual(inpcm, outpcm, srcrate, tarrate, innum, resample);
        }
    }

    //for (i = 0; i < outnum; i++)
    //{
    //  if (outpcm[i] >= 32767)
    //      outpcm[i] = 32767;
    //  if (outpcm[i] <= -32768)
    //      outpcm[i] = -32768;
    //}

    return outnum;
}

int resampling_process_up_channel(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample) {
    int outnum = 0;
    int i = 0;

    if (srcrate == tarrate) {
        memcpy(outpcm, inpcm, sizeof(short)*innum);
        outnum = innum;
    } else { //attention, perhaps the output number has several point more or less, so need to check and adjust
        outnum = resampling_process_mono(inpcm, outpcm, srcrate, tarrate, innum, resample);
    }

    //copy one channel data to another
    for (i = outnum - 1; i >= 0; i--) {
        outpcm[i * 2 + 1] = outpcm[i];
        outpcm[i * 2] = outpcm[i];
    }
    outnum *= 2;

    return outnum;
}

int resampling_process_down_channel(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, int channelidx, RESAMPLE *resample) {
    int outnum = 0;
    int i = 0;

    if ((innum & 1) != 0) {
        //printf("input pcm number error. For two channel case, left & right channel should has same number\n");
        return 0;
    }

    if (srcrate == tarrate) {
        outnum = 0;
        for (i = channelidx; i < innum; i += 2) {
            outpcm[outnum++] = inpcm[i];
        }
    } else { //attention, perhaps the output number has several point more or less, so need to check and adjust
        outnum = resampling_process_dual_down_ch(inpcm, channelidx, outpcm, srcrate, tarrate, innum, resample);
    }

    return outnum;
}
