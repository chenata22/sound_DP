#include <iostream>
#include <SDL2/SDL.h>
#include "DSP_WAV_Header.h"

int calculate_sound_freq(void *sampleArr, int bitsPerSample, int numChannels, int len , int samplesPerSec);

enum SineWaveDirection{
    decending = 0,
    ascending = 1
};

int main(int argc, char* argv[])
{
    int                 width           = 800;
    int                 height          = 600;
    SDL_Window*         myWindow        = NULL;
    SDL_Event           myEvent         = {};
    SDL_AudioSpec       audio_params    = {};
    Uint32              audio_len       = 0;
    Uint8 *             audio_data      = NULL;
    void *              sampleArray     = NULL;

    WAVHeader m_header = {0};
    FILE * fwav;

    if(argc < 2)
    {
        std::cout<<"input file name!"<<std::endl;
        return -1;
    }

    fwav = fopen(argv[1],"rb");

    if(fwav == NULL){
        std::cout<<"could not open file!"<<std::endl;
    }

    fread(&m_header,sizeof(WAVHeader),1,fwav);

    std::cout<<m_header.chunk1ID<<std::endl;
    std::cout<<m_header.chunk1Size<<std::endl;
    std::cout<<m_header.format<<std::endl;
    std::cout<<std::endl;
    std::cout<<m_header.chunk2ID<<std::endl;
    std::cout<<m_header.chunk2Size<<std::endl;
    std::cout<<m_header.Audioformat<<std::endl;
    std::cout<<m_header.numChanels<<std::endl;
    std::cout<<m_header.Sample_rate<<std::endl;
    std::cout<<m_header.Byte_rate<<std::endl;
    std::cout<<m_header.blok_align<<std::endl;
    std::cout<<m_header.bits_per_sample<<std::endl;
    std::cout<<std::endl;
    std::cout<<m_header.chunk3ID<<std::endl;
    std::cout<<m_header.chunk3Size<<std::endl;

    sampleArray = malloc(m_header.chunk3Size);

    fread(sampleArray,sizeof(char),m_header.chunk3Size,fwav);

    std::cout<<*(short*)sampleArray<<std::endl;
    std::cout<<*(short*)(sampleArray+2)<<std::endl;

    if (0 < calculate_sound_freq(sampleArray,m_header.bits_per_sample,m_header.numChanels,m_header.chunk3Size,m_header.Sample_rate))
    {
        std::cout<<"Error calculating"<<std::endl;
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    // myWindow = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //                                          width, height, SDL_WINDOW_ALLOW_HIGHDPI );


    if(myWindow == NULL)
    {
        std::cout<<"ERROR"<<std::endl;
        //return 1;
    }
    else
    {
        std::cout<<"SFSG"<<std::endl;
    }

    SDL_LoadWAV(argv[1],&audio_params,&audio_data,&audio_len);
    std::cout<<SDL_GetError()<<std::endl;
    //SDL_LoadWAV_RW(SDL_RWFromFile("Paranoid.wav", "rb"), 1, &audio_params,audio_data,&audio_len);
    std::cout<<"chanels = "<<(Uint32)audio_params.channels<<"     samples = "<< audio_params.samples<< audio_params.freq<<std::endl;

    for(int i = 0; i < audio_params.samples; i++)
    {
       // printf("%x ",audio_data[i]);
    }

    while (true)
    {
        if(SDL_PollEvent( &myEvent))
        {
            if(SDL_QUIT == myEvent.type)
            {
                std::cout<<"bye bye!"<<std::endl;
                break;
            }
        }
    }

    if(myWindow != NULL)
        SDL_DestroyWindow(myWindow);
    SDL_Quit();
    
    return 0;
}

int calculate_sound_freq(void *sampleArr, int bitsPerSample, int numChannels, int len , int samplesPerSec)
{
    int             bytesPerSample      = bitsPerSample / 8;
    int             repeat              = 0;                 //calculate frequency for each channel
    int             sampleData          = 0;
    signed int    maxCurAmptitude     = 0;
    signed int    minCurAmptitude     = 0;
    int             waveDirection       = ascending;
    double          timePerSample       = (double)60.0 /samplesPerSec; //s
    double          wavePeriod          = 0.0f;
    char            sampleBytesArr[4]   = {0,0,0,0};
    int             numSamplesPerPeriod = 0;

    printf("min %d   max %d \n",minCurAmptitude, maxCurAmptitude);

    printf("time between 2 samples = %lf s\n", timePerSample);

    if(bytesPerSample < 1 )
    {
        return -1;
    }

    printf("bytes per sample = %d \n", bytesPerSample);

    for(repeat = 0; repeat < numChannels; repeat++)
    {
        for(int i = 0; i < len/numChannels; i += bytesPerSample,numSamplesPerPeriod ++)
        {   
            switch (bytesPerSample)
            {
            case 1:
                sampleData = *((char*)(sampleArr+i+(repeat*bytesPerSample)));
                break;
            case 2:
                sampleData = (int)*((short*)(sampleArr+i+(repeat*bytesPerSample)));
                break;
            case 3:
                for(int b = 0; b < bytesPerSample; b ++)
                {
                    sampleBytesArr[i] = *((char*)(sampleArr+i+(repeat*bytesPerSample)+b));
                }
                sampleData = *((int*)sampleBytesArr);
                break;
            default:
                sampleData = *((char*)(sampleArr+i+(repeat*bytesPerSample)));
                break;
            }

            //printf("sample = %d\n", sampleData);

            if(waveDirection == decending)
            {
                if(sampleData > minCurAmptitude)
                {
                    if(sampleData - minCurAmptitude > 50)
                    {
                    //printf("stoyan 1\n");
                    waveDirection = ascending;
                    wavePeriod    = (double)numSamplesPerPeriod * (double)timePerSample;
                    printf("estimated current F = %02lf , samples needed = %d\n", ((double)60.0 / wavePeriod ),numSamplesPerPeriod);
                    numSamplesPerPeriod = -1;
                    maxCurAmptitude = sampleData;
                    }
                }
                else
                {
                    //printf("stoyan 2\n");
                    minCurAmptitude = sampleData;
                }
            }
            else
            {
                if(sampleData < maxCurAmptitude)
                {
                    if(maxCurAmptitude - sampleData > 50)
                    {
                        //printf("stoyan 3\n");
                        waveDirection = decending;
                        minCurAmptitude = sampleData;
                    }
                }
                else
                {
                    //printf("stoyan 4\n");
                    maxCurAmptitude = sampleData;
                }
            }
        }
    }

    return 0;
}
