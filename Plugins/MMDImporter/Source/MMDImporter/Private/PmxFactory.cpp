// Practice Unreal by Stiner


#include "PmxFactory.h"

UPmxFactory::UPmxFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Formats.Empty();
    Formats.Add(TEXT("md;Markdown Document"));
    
    bCreateNew = false;
    bText = false;
    bEditorImport = true;
}

UObject* UPmxFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
    return Super::FactoryCreateFile(InClass, InParent, InName, Flags, Filename, Parms, Warn, bOutOperationCanceled);
}

UObject* UPmxFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
    return Super::FactoryCreateBinary(InClass, InParent, InName, Flags, Context, Type, Buffer, BufferEnd, Warn, bOutOperationCanceled);
}

UClass* UPmxFactory::ResolveSupportedClass()
{
    return UPmxFactory::StaticClass();
}
