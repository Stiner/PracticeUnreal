// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class PRACTICEUNREAL_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<class USphereComponent> SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
    TObjectPtr<class ABaseCharacter> OwnerCharacter;

public:
    virtual void SetOwner(class AActor* NewOwner) override;

    virtual void StartUse();

protected:
    virtual void EndUse();
};
