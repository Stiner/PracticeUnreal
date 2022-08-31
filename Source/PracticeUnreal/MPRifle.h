// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPRifle.generated.h"

UCLASS()
class PRACTICEUNREAL_API AMPRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPRifle();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Projectile")
		TSubclassOf<class AFirstPersonMPProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	bool _bIsFiringWeapon;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	UFUNCTION(Server, Reliable)
		void HandleFire();

	FTimerHandle _firingTimer;
};