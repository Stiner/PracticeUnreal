// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUWeapon.generated.h"

UCLASS()
class PRACTICEUNREAL_API APUWeapon : public AActor
{
    GENERATED_BODY()

public:
    APUWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<class USphereComponent> SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
    TObjectPtr<class APUCharacter> OwnerCharacter;

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
    virtual void BeginPlay() override;
    virtual void SetOwner(class AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StartFire();

protected:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void EndFire();

    UFUNCTION()
    void OnBeginOverlap(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
    void OnHandleFire();

    UFUNCTION(Server, Reliable)
    void OnHandleFire_Server(const FVector& FireLocation, const FRotator& FireRotation);

    UFUNCTION(NetMulticast, Reliable)
    void OnHitFire_Multicast(const FVector& HitLocation, const FRotator& HitDirection);
};
