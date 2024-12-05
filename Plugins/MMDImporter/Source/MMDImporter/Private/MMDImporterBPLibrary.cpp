#include "MMDImporterBPLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

UMMDImporterBPLibrary::UMMDImporterBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

float UMMDImporterBPLibrary::MMDImporterSampleFunction(float Param)
{
	return -1;
}

void UMMDImporterBPLibrary::Import(const FString& AssetName)
{
    if (AssetName.IsEmpty())
        return;

    // NOTE: 파일 경로(**/*.uasset) 조합 후 패키지 이름(/Game/**) 생성.
    const FString PackageFileName = FPaths::SetExtension(AssetName, FPackageName::GetAssetPackageExtension());
    const FString PackagePath = FPaths::Combine(FPaths::ProjectContentDir(), PackageFileName);
    const FString PackageName = FPackageName::FilenameToLongPackageName(PackagePath);

    UPackage* NewPackage = CreatePackage(*PackageName);
    if (IsValid(NewPackage) == false)
        return;

    // NOTE: 새 에셋 생성시 반드시 Outer와 RF_Public, RF_Standalone 플래그를 지정 해 줘야 함.
    UObject* NewAsset = CreateAsset(NewPackage, AssetName, RF_Public | RF_Standalone);
    if (IsValid(NewAsset) == false)
        return;

    FAssetRegistryModule::AssetCreated(NewAsset);

    NewPackage->MarkPackageDirty();

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;

    UPackage::SavePackage(NewPackage, NewAsset, *PackagePath, SaveArgs);

    TArray<UObject*> ObjectsToSync({ NewAsset });
    GEditor->SyncBrowserToObjects(ObjectsToSync);
}

UObject* UMMDImporterBPLibrary::CreateAsset(UObject* NewOuter, const FString& NewAssetName, const EObjectFlags NewFlags)
{
    UStaticMesh* NewStaticMesh = nullptr;

    NewStaticMesh = NewObject<UStaticMesh>(NewOuter, *NewAssetName, NewFlags);

    // TODO: 메쉬 구성

    return NewStaticMesh;
}
