#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonMPWeapon.generated.h"

class AThirdPersonMPCharacter;
class USoundBase;
class UAnimMontage;

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

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
        AThirdPersonMPCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
        USoundBase* WeaponSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
        FVector WeaponSoundLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
        UAnimMontage* FireFeedbackMotion;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
        float FireRate;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gameplay")
        bool IsFiringWeapon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
        float TraceDistance;

    UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
        float BaseDamage;

    FTimerHandle _firingTimer;

public:
    virtual void SetOwner(AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void StartFire();

protected:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void EndFire();

    UFUNCTION(Category = "Gameplay")
        void OnComponentBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
        void OnHandleFire();
    virtual void OnHandleFire_Implementation();

    UFUNCTION(Server, Reliable)
        void OnHandleFire_Server(const FVector& FireLocation, const FRotator& FireRotation);
};
