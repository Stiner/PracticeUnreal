// Fill out your copyright notice in the Description page of Project Settings.

#include "PUBaseCharacter.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "PUGameModeBase.h"

// Sets default values
APUBaseCharacter::APUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void APUBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APUBaseCharacter, CurrentHealth);
}

void APUBaseCharacter::SetCurrentHealth(float healthValue)
{
    if (HasAuthority())
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void APUBaseCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void APUBaseCharacter::OnHealthUpdate()
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

float APUBaseCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);

    return damageApplied;
}

void APUBaseCharacter::OnDead_Server_Implementation()
{
    if (HasAuthority())
    {
        FString healthMsg = FString::Printf(TEXT("%s Die on Server"), *GetName());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);

        Dead();
    }
}

void APUBaseCharacter::Dead_Implementation()
{
    FString dieMsg = FString::Printf(TEXT("%s DIE"), *GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dieMsg);

    Destroy();

    APUGameModeBase* gameMode = Cast<APUGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (IsValid(gameMode))
    {
        gameMode->OnDeadPlayerCharacter(GetController());
    }
}
