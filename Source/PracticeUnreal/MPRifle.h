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
    UPROPERTY(EditDefaultsOnly, Category = "Components")
        class USphereComponent* SphereComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Components")
        class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Projectile")
        TSubclassOf<class AFirstPersonMPProjectile> ProjectileClass;

    UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
        class AFirstPersonMPCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
        float FireRate;

    bool _bIsFiringWeapon;
    FTimerHandle _firingTimer;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void OnEquiped(class AFirstPersonMPCharacter* equipedCharacter);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StopFire();

    void OnHandleFire();
};