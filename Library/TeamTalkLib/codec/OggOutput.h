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

#ifndef OGGOUTPUT_H
#define OGGOUTPUT_H

#include <ogg/ogg.h>

#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>
#include <ace/Bound_Ptr.h>
#include <ace/Null_Mutex.h>
#include <ace/SString.h>

#include <vector>

class OggOutput
{
public:
    OggOutput(const OggOutput&) = delete;
    OggOutput();
    ~OggOutput();
    bool Open(int stream_id);
    void Close();

    int PutPacket(const ogg_packet& op);
    int GetPageOut(ogg_page& og);
    int FlushPageOut(ogg_page& og);

protected:
    ogg_stream_state m_os;
};

class OggFile
{
public:
    ~OggFile();
    bool Open(const ACE_TString& filename);
    void Close();

    int WriteOggPage(const ogg_page& og);

private:
    ACE_FILE_IO m_out_file;
};

#if defined(ENABLE_SPEEX)

#include <speex/speex_header.h>

class SpeexOgg
{
public:
    SpeexOgg();
    ~SpeexOgg();

    bool Open(const SpeexHeader& spx_header,
              spx_int32_t lookahead);
    void Close();

    int PutEncoded(const char* enc_data, int len, 
                   unsigned short packetno,
                   unsigned int timestamp,
                   bool last);

    int GetPageOut(ogg_page& og);
    int FlushPageOut(ogg_page& og);

private:
    void Reset();
    OggOutput m_ogg;
    spx_int32_t m_lookahead;
    ogg_int64_t m_counter;
    int m_frame_size;
    int m_msec_per_packet;
    unsigned short m_last_packetno;
    unsigned int m_last_timestamp;
};

double speex_granule_time(const SpeexHeader& spx_header,
                          spx_int32_t lookahead, ogg_int64_t granpos);

int speex_packet_jump(int msec_per_packet,
                      unsigned int last_timestamp,
                      unsigned int cur_timestamp);

class SpeexFile
{
public:
    SpeexFile();
    ~SpeexFile();
    bool Open(const ACE_TString& filename,
              int bandmode, bool vbr);

    void Close();

    int WriteEncoded(const char* enc_data, int len, 
                     bool last);

private:
    SpeexOgg m_speex;
    OggFile m_ogg;
    SpeexHeader m_spx_header;
    bool m_initial_packet;
    unsigned short m_packet_no;
    unsigned int m_timestamp;
    ogg_page m_aud_page;
};

typedef ACE_Strong_Bound_Ptr< SpeexFile, ACE_Null_Mutex > speexfile_t;

#include "SpeexEncoder.h"

class SpeexEncFile
{
public:
    SpeexEncFile();

    bool Open(const ACE_TString& filename,
              int bandmode, int complexity, float vbr_quality,
              int bitrate, int maxbitrate, bool dtx);
    void Close();
    int Encode(const short* samples, bool last=false);

private:
    SpeexEncoder m_encoder;
    SpeexFile m_file;
    std::vector<char> m_buffer;
};

typedef ACE_Strong_Bound_Ptr< SpeexEncFile, ACE_Null_Mutex > speexencfile_t;

#endif /* ENABLE_SPEEX */

#if defined(ENABLE_OPUSTOOLS)
class OpusFile
{
public:
    OpusFile();

    bool Open(const ACE_TString& filename,
              int channels, int samplerate, int framesize);
    void Close();

    int WriteEncoded(const char* enc_data, int enc_len, bool last=false);
private:
    OggOutput m_ogg;
    OggFile m_oggfile;
    int m_samplerate, m_frame_size;
    ogg_int64_t m_granule_pos, m_packet_no;
};

typedef ACE_Strong_Bound_Ptr< OpusFile, ACE_Null_Mutex > opusfile_t;

#endif /* ENABLE_OPUSTOOLS */

#if defined(ENABLE_OPUSTOOLS) && defined(ENABLE_OPUS)

#include "OpusEncoder.h"

class OpusEncFile
{
public:
    OpusEncFile();

    bool Open(const ACE_TString& filename, int channels, 
              int samplerate, int framesize, int app);
    void Close();
    int Encode(const short* input_buffer, int input_samples, bool last=false);

    OpusEncode& getEncoder() { return m_encoder; }

private:
    OpusEncode m_encoder;
    OpusFile m_file;
    std::vector<char> m_buffer;
};

typedef ACE_Strong_Bound_Ptr< OpusEncFile, ACE_Null_Mutex > opusencfile_t;

#endif


#endif
