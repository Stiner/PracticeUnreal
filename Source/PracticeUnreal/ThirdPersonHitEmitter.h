// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonHitEmitter.generated.h"

class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class PRACTICEUNREAL_API AThirdPersonHitEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThirdPersonHitEmitter();

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
        float AutoDestroyTime;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
        UParticleSystemComponent* ParticleSystem;

    FTimerHandle HandleTimerAutoDestory;

    void OnTimerAutoDestroy();
};
