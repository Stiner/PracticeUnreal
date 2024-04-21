// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleProjectile.generated.h"

UCLASS()
class PRACTICEUNREAL_API ASimpleProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASimpleProjectile();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USphereComponent> SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, category = "Components")
    TObjectPtr<class UStaticMeshComponent> StaticMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
    TObjectPtr<class UParticleSystem> ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Damage")
    TSubclassOf<class UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Damage")
    float Damage;

protected:
    virtual void Destroyed() override;

    UFUNCTION(Category = "Projectile")
    void OnProjectileImpact(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};