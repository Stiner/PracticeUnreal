// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.h"
#include "SimpleGun.generated.h"

UCLASS()
class PRACTICEUNREAL_API ASimpleGun : public ABaseWeapon
{
    GENERATED_BODY()

public:
    ASimpleGun();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
    TObjectPtr<class USoundBase> WeaponSound;

    UPROPERTY(EditAnywhere, Category = "Gameplay")
    TObjectPtr<class UParticleSystem> ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
    FVector WeaponSoundLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
    TObjectPtr<class UAnimMontage> FireFeedbackMotion;

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
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    virtual void StartUse() override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    virtual void EndUse() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
    void OnHandleFire();

    UFUNCTION(Server, Reliable)
    void OnHandleFire_Server(const FVector& FireLocation, const FRotator& FireRotation);

    UFUNCTION(NetMulticast, Reliable)
    void OnHitFire_Multicast(const FVector& HitLocation, const FRotator& HitDirection);
};
