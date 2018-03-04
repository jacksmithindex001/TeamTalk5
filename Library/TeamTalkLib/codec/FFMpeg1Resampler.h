/*
 * Copyright (c) 2005-2018, BearWare.dk
 * 
 * Contact Information:
 *
 * Bjoern D. Rasmussen
 * Kirketoften 5
 * DK-8260 Viby J
 * Denmark
 * Email: contact@bearware.dk
 * Phone: +45 20 20 54 59
 * Web: http://www.bearware.dk
 *
 * This source code is part of the TeamTalk SDK owned by
 * BearWare.dk. Use of this file, or its compiled unit, requires a
 * TeamTalk SDK License Key issued by BearWare.dk.
 *
 * The TeamTalk SDK License Agreement along with its Terms and
 * Conditions are outlined in the file License.txt included with the
 * TeamTalk SDK distribution.
 *
 */

#if !defined(FFMPEGRESAMPLER_H)
#define FFMPEGRESAMPLER_H

// compatible with ffmpeg tag n1.1.13

extern "C" {
#include <libswresample/swresample.h>
#include <libavutil/audioconvert.h>
}

#include <vector>
#include <codec/AudioResampler.h>

class FFMPEGResampler : public AudioResampler
{
public:
    FFMPEGResampler();
    ~FFMPEGResampler();
    
    bool Init(int input_samplerate, int input_channels, 
              int output_samplerate, int output_channels);
    void Close();

    //return number of samples written to 'output_samples'
    int Resample(const short* input_samples, int input_samples_size,
                 short* output_samples, int output_samples_size);

private:
    SwrContext* m_ctx;
};

#endif