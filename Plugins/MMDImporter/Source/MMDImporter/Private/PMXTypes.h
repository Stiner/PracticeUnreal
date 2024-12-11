#pragma once

#include "CoreMinimal.h"

namespace PMX
{
    typedef unsigned char   byte;

    struct Vector2
    {
        float X;
        float Y;
    };
    struct Vector3
    {
        float X;
        float Y;
        float Z;
    };
    struct Vector4
    {
        float X;
        float Y;
        float Z;
        float W;
    };

    enum class TextEncoding
    {
        UTF16LE, UTF8,
    };

    // TODO : Globals를 그냥 다 개별 변수로 따로 빼는걸 고민 해봐야 할 듯.
    struct Header
    {
        PMX::Byte Signature[4];     // "PMX " (0x50, 0x4d, 0x58, 0x20) : 공백으로 끝을 알림
        float Version;              // 2.0 / 2.1 : floating point 로 비교
        PMX::Byte GlobalsCount;     // PMX 2.0은 8로 고정 됨. 가변적일 수 있음
        PMX::Byte* Globals;       // GloabalsCount 크기 만큼의 버퍼

        FString ModelNameLocal;     // 보통은 일본어
        FString ModelNameUniversal; // 보통은 영어
        FString CommentsLocal;      // 보통은 일본어
        FString CommentsUniversal;  // 보통은 영어
    };

    // Header의 Globals 배열의 각 인덱스별 용도에 대한 정의
    enum class GlobalsUsageByIndex
    {
        TextEncoding,           // 텍스트 인코딩: 인코딩 타입은 Text::Encoding에 정의
        AdditionalVec4Count,    // 각 Vertex에 Vector4가 추가 여부 (0~4)
        VertexIndexSize,        // The index type for vertices(See Index Types above)
        TextureIndexSize,       // The index type for textures(See Index Types above)
        MaterialIndexSize,      // The index type for materials(See Index Types above)
        BoneIndexSize,          // The index type for bones(See Index Types above)
        MorphIndexSize,         // The index type for morphs(See Index Types above)
        RigidbodyIndexSize,     // The index type for rigid bodies(See Index Types above)
    };

    struct BDEF1
    {
        int32 Index;
    };

    struct BDEF2
    {
        int32 Index1;
        int32 Index2;

        float Weight1;
        float Weight2; // = 1.0 - Weight1
    };

    struct BDEF4
    {
        int32 Index1;
        int32 Index2;
        int32 Index3;
        int32 Index4;

        float Weight1;
        float Weight2;
        float Weight3;
        float Weight4;
    };

    // Spherical deform blending
    struct SDEF
    {
        int32 Index1;
        int32 Index2;
        float Weight1;
        float Weight2; // = 1.0 - Weight1

        PMX::Vector3 C;  // ???
        PMX::Vector3 R0; // ???
        PMX::Vector3 R1; // ???
    };

    // Dual quaternion deform blending
    struct QDEF
    {
        int32 Index1;
        int32 Index2;
        int32 Index3;
        int32 Index4;

        float Weight1; // 총 가중치는 1.0을 보장하지 않음
        float Weight2; // 총 가중치는 1.0을 보장하지 않음
        float Weight3; // 총 가중치는 1.0을 보장하지 않음
        float Weight4; // 총 가중치는 1.0을 보장하지 않음
    };

    enum WeightDeformType
    {
        BDEF1,
        BDEF2,
        BDEF4,
        SDEF,
        QDEF
    };

    struct Vertex
    {
        FVector3f Position;
        FVector3f Normal;
        FVector3f UV;
        FVector4f Additional[4];  // Globals에 의해 갯수가 정해지며, 0개일 수 있음
        PMX::WeightDeformType WeightDeformType;  //
        // WeightDeformType에 따라 BDEF1..4/SDEF/QDEF 선택
        float EdgeScale;
    };


}
