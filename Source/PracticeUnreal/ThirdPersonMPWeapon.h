#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonMPWeapon.generated.h"

class AThirdPersonMPCharacter;

UCLASS()
class PRACTICEUNREAL_API AThirdPersonMPWeapon : public AActor
{
    GENERATED_BODY()

public:
    AThirdPersonMPWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Projectile")
        TSubclassOf<class AThirdPersonMPCharacter> ProjectileClass;

    UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
        AThirdPersonMPCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
        float FireRate;

    UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
        bool IsFiringWeapon;

    FTimerHandle _firingTimer;

public:
    virtual void SetOwner(AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StartFire();

protected:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
        void OnHandleFire();
    virtual void OnHandleFire_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void EndFire();

    UFUNCTION(Server, Reliable)
        void OnHandleFire_Server();
};
