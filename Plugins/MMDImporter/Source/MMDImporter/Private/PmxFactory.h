// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PmxFactory.generated.h"

/**
 * 
 */
UCLASS()
class UPmxFactory : public UFactory
{
    GENERATED_BODY()

public:
    UPmxFactory(const FObjectInitializer& ObjectInitializer);

    virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

    virtual UObject* FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

    virtual UClass* ResolveSupportedClass() override; 
};
