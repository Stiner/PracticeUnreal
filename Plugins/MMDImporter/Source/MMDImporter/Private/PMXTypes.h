#pragma once

#include "CoreMinimal.h"

namespace PMX
{
    typedef unsigned char   Byte;

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

    enum class TextEncodingType : PMX::Byte
    {
        UTF16LE,
        UTF8,
    };

    struct Header
    {
        PMX::Byte Signature[4];     // "PMX " (0x50, 0x4d, 0x58, 0x20) : 공백으로 끝을 알림
        float Version;              // 2.0 / 2.1 : floating point 로 비교

        PMX::TextEncodingType TextEncoding;
        PMX::Byte AdditionalVec4Count;
        PMX::Byte VertexIndexSize;
        PMX::Byte TextureIndexSize;
        PMX::Byte MaterialIndexSize;
        PMX::Byte BoneIndexSize;
        PMX::Byte MorphIndexSize;
        PMX::Byte RigidbodyIndexSize;
    };

    struct ModelInfo
    {
        FString ModelNameLocal;     // 보통은 일본어
        FString ModelNameUniversal; // 보통은 영어
        FString CommentsLocal;      // 보통은 일본어
        FString CommentsUniversal;  // 보통은 영어
    };

    struct BDEF1
    {
        int Index;
    };

    struct BDEF2
    {
        int Index1;
        int Index2;

        float Weight1;
        float Weight2; // = 1.0 - Weight1
    };

    struct BDEF4
    {
        int Index1;
        int Index2;
        int Index3;
        int Index4;

        float Weight1;
        float Weight2;
        float Weight3;
        float Weight4;
    };

    // Spherical deform blending
    struct SDEF
    {
        int Index1;
        int Index2;
        float Weight1;
        float Weight2; // = 1.0 - Weight1

        PMX::Vector3 C;  // ???
        PMX::Vector3 R0; // ???
        PMX::Vector3 R1; // ???
    };

    // Dual quaternion deform blending
    struct QDEF
    {
        int Index1;
        int Index2;
        int Index3;
        int Index4;

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
