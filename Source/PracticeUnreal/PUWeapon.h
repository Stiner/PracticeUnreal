// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUWeapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class APUCharacter;
class USoundBase;
class UAnimMontage;
class UPrimitiveComponent;
class UParticleSystem;

UCLASS()
class PRACTICEUNREAL_API APUWeapon : public AActor
{
    GENERATED_BODY()

public:
    APUWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Gameplay")
    APUCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
    USoundBase* WeaponSound;

    UPROPERTY(EditAnywhere, Category = "Gameplay")
    UParticleSystem* ExplosionEffect;

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
    virtual void BeginPlay() override;
    virtual void SetOwner(AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StartFire();

protected:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void EndFire();

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay")
    void OnHandleFire();
    virtual void OnHandleFire_Implementation();

    UFUNCTION(Server, Reliable)
    void OnHandleFire_Server(const FVector& FireLocation, const FRotator& FireRotation);

    UFUNCTION(NetMulticast, Reliable)
    void OnHitFire_Multicast(const FVector& HitLocation, const FRotator& HitDirection);
};
