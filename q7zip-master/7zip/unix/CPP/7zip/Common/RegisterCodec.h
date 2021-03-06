// RegisterCodec.h

#ifndef __REGISTER_CODEC_H
#define __REGISTER_CODEC_H

#include "../Common/MethodId.h"
#include "../ICoder.h"

#include <mutex>

typedef void * (*CreateCodecP)();
struct CCodecInfo
{
  CreateCodecP CreateDecoder;
  CreateCodecP CreateEncoder;
  CMethodId Id;
  const wchar_t *Name;
  UInt32 NumInStreams;
  bool IsFilter;
  std::once_flag* once;
};

void RegisterCodec(const CCodecInfo *codecInfo) throw();

#define REGISTER_CODEC_NAME(x) CRegisterCodec ## x

#define REGISTER_CODEC(x) struct REGISTER_CODEC_NAME(x) \
    { \
        REGISTER_CODEC_NAME(x)() \
        { \
            if(g_CodecInfo.once==0)\
                g_CodecInfo.once = new std::once_flag();\
            std::call_once(*g_CodecInfo.once, RegisterCodec,(const CCodecInfo *)&g_CodecInfo); \
        } \
    }; \
    static REGISTER_CODEC_NAME(x) g_RegisterCodec; \
    void registerCodec##x() { static REGISTER_CODEC_NAME(x) g_RegisterCodecs; }

#define REGISTER_CODECS_NAME(x) CRegisterCodecs ## x
#define REGISTER_CODECS(x) struct REGISTER_CODECS_NAME(x) \
    { \
        REGISTER_CODECS_NAME(x)() \
        { \
            for (unsigned i = 0; i < ARRAY_SIZE(g_CodecsInfo); i++) \
            {\
                if(g_CodecsInfo[i].once==0)\
                    g_CodecsInfo[i].once = new std::once_flag();\
                std::call_once(*g_CodecsInfo[i].once, RegisterCodec,(const CCodecInfo *)&g_CodecsInfo[i]); \
            }\
        } \
    }; \
    static REGISTER_CODECS_NAME(x) g_RegisterCodecs; \
    void registerCodec##x() { static REGISTER_CODECS_NAME(x) g_RegisterCodecs; }


struct CHasherInfo
{
  IHasher * (*CreateHasher)();
  CMethodId Id;
  const wchar_t *Name;
  UInt32 DigestSize;
};

void RegisterHasher(const CHasherInfo *hasher) throw();

#define REGISTER_HASHER_NAME(x) CRegisterHasher ## x

#define REGISTER_HASHER(x) struct REGISTER_HASHER_NAME(x) { \
    REGISTER_HASHER_NAME(x)() { RegisterHasher(&g_HasherInfo); }}; \
    static REGISTER_HASHER_NAME(x) g_RegisterHasher;

#endif
