#ifndef DSP_WAV_HEADER
#define DSP_WAV_HEADER

enum SineWaveDirection{
    decending = 0,
    ascending = 1
};


int calculate_sound_freq(void *sampleArr, int bitsPerSample, int numChannels, int len , int samplesPerSec);

#endif