// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonGameCharacter.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AThirdPersonGameCharacter::AThirdPersonGameCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void AThirdPersonGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonGameCharacter, CurrentHealth);
}

void AThirdPersonGameCharacter::SetCurrentHealth(float healthValue)
{
    if (HasAuthority())
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void AThirdPersonGameCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AThirdPersonGameCharacter::OnHealthUpdate()
{
    if (IsLocallyControlled())
    {
        FString healthMsg = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);

        if (CurrentHealth <= 0)
        {
            FString deadMsg = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, deadMsg);
        }
    }

    if (HasAuthority())
    {
        FString healthMsg = FString::Printf(TEXT("%s now has %f health remaining."), *GetName(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);
    }
}

// 서버에서 실행 됨
float AThirdPersonGameCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);

    return damageApplied;
}
