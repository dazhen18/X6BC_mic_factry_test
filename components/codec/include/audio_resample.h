#ifndef __AUDIO_RESAMPLE_H__
#define __AUDIO_RESAMPLE_H__

#define INPCM_DELAY_SIZE (6)

typedef struct {
    short inpcm[INPCM_DELAY_SIZE * 2];  ///the pcm value of last time calling.  maximum should be 6: 48000/8000;
    int innum;  /// the total input pcm number
    int outnum; /// the total outnum pcm number
    float hp_mem[4]; ///for filter, the first two is for first channel, the last two is for second channel
} RESAMPLE;

/**
 * @brief re-sampling
 *
 * @param inpcm     : input buffer that musn't be NULL
 * @param outpcm    : output buffer that musn't be NULL
 * @param srcrate   : source sample rate
 * @param tarrate   : target sample rate
 * @param inpcm     : input buffer size in bytes
 * @param channel   : source channel quantity
 * @param resample  : control structure
 *
 * @return
 *     - (0) error
 *     - (Other, > 0) output buffer bytes
 */
int resampling_process(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, int channel, RESAMPLE *resample);

/**
 * @brief re-sampling from 1 channel to 2
 *
 * @param inpcm     : input buffer that musn't be NULL
 * @param outpcm    : output buffer that musn't be NULL
 * @param srcrate   : source sample rate
 * @param tarrate   : target sample rate
 * @param inpcm     : input buffer size in bytes
 * @param channel   : source channel quantity
 * @param resample  : control structure
 *
 * @return
 *     - (0) error
 *     - (Other, > 0) output buffer bytes
 */
int resampling_process_up_channel(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, RESAMPLE *resample);

/**
 * @brief re-sampling from 2 channel to 1
 *
 * @param inpcm     : input buffer that musn't be NULL
 * @param outpcm    : output buffer that musn't be NULL
 * @param srcrate   : source sample rate
 * @param tarrate   : target sample rate
 * @param inpcm     : input buffer size in bytes
 * @param resample  : control structure
 *
 * @return
 *     - (0) error
 *     - (Other, > 0) output buffer bytes
 */
int resampling_process_down_channel(short *inpcm, short *outpcm,
        int srcrate, int tarrate, int innum, int channelidx, RESAMPLE *resample);

#endif //__AUDIO_RESAMPLE_H__