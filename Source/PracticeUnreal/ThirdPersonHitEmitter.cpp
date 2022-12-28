// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonHitEmitter.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AThirdPersonHitEmitter::AThirdPersonHitEmitter()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> defaultExplosionEffect(TEXT("/Game/Particles/P_Explosion.P_Explosion"));
    if (defaultExplosionEffect.Succeeded())
    {
        ParticleSystem->SetTemplate(defaultExplosionEffect.Object);
    }

    AutoDestroyTime = 2.0f;

    SetReplicates(true);
}

void AThirdPersonHitEmitter::BeginPlay()
{
    GetWorld()->GetTimerManager().SetTimer(HandleTimerAutoDestory, this, &AThirdPersonHitEmitter::OnTimerAutoDestroy, AutoDestroyTime, false);
}

void AThirdPersonHitEmitter::OnTimerAutoDestroy()
{
    Destroy();
}
