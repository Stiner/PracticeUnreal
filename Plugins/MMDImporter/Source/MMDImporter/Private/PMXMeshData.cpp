// Practice Unreal by Stiner
#include "PMXMeshData.h"

#define LOCTEXT_NAMESPACE "MMDImporter"

DEFINE_LOG_CATEGORY(LogMMDImporter_MeshData)

static SIZE_T ReadBuffer(void* OutDst, const PMX::Byte* const InBuffer, SIZE_T ReadSize)
{
    FMemory::Memcpy(OutDst, InBuffer, ReadSize);
    return ReadSize;
}

static SIZE_T ReadText(FString& OutString, const PMX::Byte* InBuffer, PMX::TextEncodingType EncodingType)
{
    int32 TextByteSize = 0;
    InBuffer += ReadBuffer(&TextByteSize, InBuffer, sizeof(TextByteSize));

    TArray<PMX::Byte> TextBytes;
    TextBytes.AddUninitialized(TextByteSize);
    InBuffer += ReadBuffer(TextBytes.GetData(), InBuffer, sizeof(PMX::Byte) * TextByteSize);

    switch (EncodingType)
    {
        case PMX::TextEncodingType::UTF16LE:
            {
                TextBytes.Add(0); TextBytes.Add(0);

                OutString = FUTF16ToTCHAR(reinterpret_cast<UTF16CHAR*>(TextBytes.GetData())).Get();
            }
            break;

        case PMX::TextEncodingType::UTF8:
            {
                TextBytes.Add(0);

                OutString = FUTF8ToTCHAR(reinterpret_cast<UTF8CHAR*>(TextBytes.GetData())).Get();
            }
            break;

        default:
            check(TEXT("Invalid encoding type."));
    }

    return sizeof(int32) + TextByteSize;
}

bool PMXMeshData::LoadBinary(const PMX::Byte*& Buffer, const PMX::Byte* BufferEnd)
{
    auto BufferSize = BufferEnd - Buffer;
    const PMX::Byte* BufferCur = Buffer;

    BufferCur += ReadBuffer(Header.Signature, BufferCur, sizeof(Header.Signature));
    if ((Header.Signature[0] == 'P' && Header.Signature[1] == 'M' && Header.Signature[2] == 'X') == false)
        return false;

    BufferCur += ReadBuffer(&Header.Version, BufferCur, sizeof(Header.Version));

    PMX::Byte GlobalsCount = 0;
    BufferCur += ReadBuffer(&GlobalsCount, BufferCur, sizeof(PMX::Byte));

    BufferCur += ReadBuffer(&Header.TextEncoding, BufferCur, sizeof(PMX::Byte) * GlobalsCount);

    BufferCur += ReadText(ModelInfo.ModelNameLocal, BufferCur, Header.TextEncoding);
    BufferCur += ReadText(ModelInfo.ModelNameUniversal, BufferCur, Header.TextEncoding);
    BufferCur += ReadText(ModelInfo.CommentsLocal, BufferCur, Header.TextEncoding);
    BufferCur += ReadText(ModelInfo.CommentsUniversal, BufferCur, Header.TextEncoding);

    return true;
}

#undef LOCTEXT_NAMESPACE