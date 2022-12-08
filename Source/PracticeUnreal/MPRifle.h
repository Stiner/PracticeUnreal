#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPRifle.generated.h"

UCLASS()
class PRACTICEUNREAL_API AMPRifle : public AActor
{
    GENERATED_BODY()

public:
    AMPRifle();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Projectile")
        TSubclassOf<class AFirstPersonMPProjectile> ProjectileClass;

    UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
        class AFirstPersonMPCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
        float FireRate;

    UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
        bool IsFiringWeapon;

    FTimerHandle _firingTimer;

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
        void OnHandleFire();
    virtual void OnHandleFire_Implementation();

protected:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StopFire();
};
