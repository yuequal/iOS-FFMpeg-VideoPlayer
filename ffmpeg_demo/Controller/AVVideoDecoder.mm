//
//  AVVideoDecoder.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVVideoDecoder.h"

namespace AVVideoPlayer {
    
AVVideoDecoder::AVVideoDecoder()
{
    
}

int32_t AVVideoDecoder::Release()
{
    VideoDecode::Release();
    return -1;
}

int32_t AVVideoDecoder::RegisterCallBack(AVDecodeImageCallBack* callBack)
{
    m_decodeCallBack = callBack;
    return -1;
}


AVVideoDecoder::~AVVideoDecoder()
{
    Release();
}

int32_t AVVideoDecoder::InitVideoDecode(const AVVideoCodec *codec, int32_t ns = 0)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    int32_t ret = Release();
    
    AVCodecParameters *params = avcodec_parameters_alloc();
    AVCodec *mcodec = avcodec_find_decoder(params->codec_id);
    AVCodecContext *codecContext = avcodec_alloc_context3(mcodec);
    
    ret = avcodec_parameters_to_context(codecContext, params);
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    codecContext->extradata = nullptr;
    codecContext->extradata_size = 0;
    codecContext->thread_count = 8;
    codecContext->thread_type = FF_THREAD_SLICE;
    codecContext->get_buffer2 = AVGetBuffer;
    codecContext->opaque = this;
    m_codecContext.reset(codecContext);
    ret = avcodec_open2(m_codecContext.get(), mcodec, nullptr);
    return ret;
}
    
int AVVideoDecoder::AVGetBuffer(AVCodecContext *codecContext, AVFrame *frame, int flags)
{
    return 0;
}

int32_t AVVideoDecoder::Decode(const AVVideoFrameImage& image, const std::string& info, int64_t nv)
{
    return -1;
}

const char* AVVideoDecoder::kDecodeImplName = "FFMpeg";
const char* AVVideoDecoder::DecodeName() const
{
    return kDecodeImplName;
}
}

@implementation AVVideoDecoder

@end
