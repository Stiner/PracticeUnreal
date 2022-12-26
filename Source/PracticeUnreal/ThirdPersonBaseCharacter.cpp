// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonBaseCharacter.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "ThirdPersonMPGameModeBase.h"

// Sets default values
AThirdPersonBaseCharacter::AThirdPersonBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void AThirdPersonBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonBaseCharacter, CurrentHealth);
}

void AThirdPersonBaseCharacter::SetCurrentHealth(float healthValue)
{
    if (HasAuthority())
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void AThirdPersonBaseCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AThirdPersonBaseCharacter::OnHealthUpdate()
{
    if (HasAuthority())
    {
        FString healthMsg = FString::Printf(TEXT("%s now has %f health remaining."), *GetName(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);

        if (CurrentHealth <= 0)
        {
            OnDead_Server();
        }
    }
}

float AThirdPersonBaseCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);

    return damageApplied;
}

void AThirdPersonBaseCharacter::OnDead_Server_Implementation()
{
    if (HasAuthority())
    {
        FString healthMsg = FString::Printf(TEXT("%s Die on Server"), *GetName());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);

        Dead();
    }
}

void AThirdPersonBaseCharacter::Dead_Implementation()
{
    FString dieMsg = FString::Printf(TEXT("%s DIE"), *GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dieMsg);

    Destroy();

    AThirdPersonMPGameModeBase* gameMode = Cast<AThirdPersonMPGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (IsValid(gameMode))
    {
        gameMode->OnDeadPlayerCharacter();
    }
}
