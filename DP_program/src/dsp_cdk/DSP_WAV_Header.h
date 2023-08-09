#ifndef DSP_WAV_HEADER
#define DSP_WAV_HEADER
// #include <DSP_freq.h>
#include <stdio.h>
typedef struct{
    int chunk1ID;
    int chunk1Size;
    int format;
    int chunk2ID;
    int chunk2Size;
    short Audioformat;
    short numChanels;
    int Sample_rate;
    int Byte_rate;
    short blok_align;
    short bits_per_sample;
    int chunk3ID;
    int chunk3Size;
}WAVHeader;



#endif