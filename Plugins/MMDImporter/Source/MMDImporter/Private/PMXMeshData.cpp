// Practice Unreal by Stiner
#include "PMXMeshData.h"

#define LOCTEXT_NAMESPACE "MMDImporter"

DEFINE_LOG_CATEGORY(LogMMDImporter_MeshData)

static SIZE_T ReadBuffer(void* OutDst, const PMX::Byte* InBuffer, SIZE_T ReadSize)
{
    FMemory::Memcpy(OutDst, InBuffer, ReadSize);
    return ReadSize;
}

bool PMXMeshData::LoadBinary(const PMX::Byte*& Buffer, const PMX::Byte* BufferEnd)
{
    auto BufferSize = BufferEnd - Buffer;
    const PMX::Byte* BufferCur = Buffer;

    BufferCur += ReadBuffer(Header.Signature, BufferCur, sizeof(Header.Signature));
    if (Header.Signature[0] == 'P' && Header.Signature[1] == 'M' && Header.Signature[2] == 'X')
    {
        UE_LOG(LogMMDImporter_MeshData, Error, TEXT("Correct file"));
    }
    else
    {
        UE_LOG(LogMMDImporter_MeshData, Error, TEXT("Incorrect file"));
        return false;
    }

    BufferCur += ReadBuffer(&Header.Version, BufferCur, sizeof(Header.Version));

    BufferCur += ReadBuffer(&Header.GlobalsCount, BufferCur, sizeof(Header.GlobalsCount));

    for (int i = 0; i < Header.GlobalsCount; ++i)
    {
        BufferCur += ReadBuffer(&(Header.Globals[i]), BufferCur, sizeof(Header.Globals[i]));
    }

    int TextBufferSize = 0;
    BufferCur += ReadBuffer(&TextBufferSize, BufferCur, sizeof(TextBufferSize));

    //for (int i = 0; i < TextBufferSize; ++i)
    //{
    //    BufferCur += 
    //}


    return true;
}

#undef LOCTEXT_NAMESPACE