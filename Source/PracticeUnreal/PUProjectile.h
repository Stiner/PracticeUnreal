#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class PRACTICEUNREAL_API APUProjectile : public AActor
{
    GENERATED_BODY()

public:
    APUProjectile();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, category = "Components")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Damage")
    TSubclassOf<class UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Damage")
    float Damage;

protected:
    virtual void Destroyed() override;

    UFUNCTION(Category = "Projectile")
    void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};