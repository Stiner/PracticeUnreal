// Practice Unreal by Stiner
#pragma once

#include "CoreMinimal.h"
#include "PMXTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMMDImporter_MeshData, Log, All)

/**
 * PMX Mesh Data
 */
class PMXMeshData
{
public:
    bool LoadBinary(const PMX::Byte*& Buffer, const PMX::Byte* BufferEnd);

protected:
    PMX::Header Header = { 0, };
    PMX::ModelInfo ModelInfo;

    int VertexCount;

    int SurfaceCount;

    int TextureCount;

    int MaterialCount;

    int BoneCount;

    int MorphCount;

    int DisplayframeCount;

    int RigidbodyCount;

    int JointCount;

    int SoftBodyCount;
};
