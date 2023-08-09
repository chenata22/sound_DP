#include "DSP_freq.h"
#include <stdio.h>

int calculate_sound_freq(void *sampleArr, int bitsPerSample, int numChannels, int len , int samplesPerSec)
{
    int             bytesPerSample      = bitsPerSample / 8;
    int             repeat              = 0;                 //calculate frequency for each channel
    int             sampleData          = 0;
    unsigned int    maxCurAmptitude     = 0xffffffff;
    signed   int    minCurAmptitude     = 0xffffffff;
    int             waveDirection       = decending;
    int             numSamplesPerPeriod = 0;
    double          timePerSample       = 60.0 / (double)numSamplesPerPeriod;
    double          wavePeriod          = 0.0f;
    char            sampleBytesArr[4]   = {0,0,0,0};

    printf("min %d   max %d \n",minCurAmptitude, maxCurAmptitude);

    printf("time between 2 samples = %lf\n", timePerSample);

    if(bytesPerSample < 1 )
    {
        return -1;
    }

    printf("bytes per sample = %d ", bytesPerSample);

    for(repeat = 0; repeat < numChannels; repeat++)
    {
        for(int i = 0; i < len/numChannels; i ++,numSamplesPerPeriod ++)
        {   
            switch (bytesPerSample)
            {
            case 1:
                sampleData = *((char*)(sampleArr+i+repeat));
                break;
            case 2:
                sampleData = *((short*)(sampleArr+i+repeat));
                break;
            case 3:
                for(int b = 0; b < bytesPerSample; b ++)
                {
                    sampleBytesArr[i] = *((char*)(sampleArr+i+repeat+b));
                }
                sampleData = *((int*)sampleBytesArr);
                break;
            default:
                sampleData = *((char*)(sampleArr+i+repeat));
                break;
            }

            printf("sample = %d\n", sampleData);

            if(waveDirection == decending)
            {
                if(sampleData > minCurAmptitude)
                {

                    waveDirection = ascending;
                    wavePeriod    = (double)numSamplesPerPeriod * timePerSample;
                    printf("estimated current F = %lf\n", (60.0 / wavePeriod ));
                    numSamplesPerPeriod = -1;
                }
                else
                {
                    minCurAmptitude = sampleData;
                }
            }
            else
            {
                if(sampleData < maxCurAmptitude)
                {
                    waveDirection = decending;
                }
                else
                {
                    maxCurAmptitude = sampleData;
                }
            }
        }
    }

    return 0;
}
