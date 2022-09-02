#include "FirstPersonMPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AFirstPersonMPCharacter::AFirstPersonMPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void AFirstPersonMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFirstPersonMPCharacter, CurrentHealth);
}

void AFirstPersonMPCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

float AFirstPersonMPCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;
}

void AFirstPersonMPCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AFirstPersonMPCharacter::UseItem()
{
    HandleFire();
}

void AFirstPersonMPCharacter::HandleFire_Implementation()
{
    FString msg = FString::Printf(TEXT("%s fire"), *GetName());
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, msg);
}

void AFirstPersonMPCharacter::OnHealthUpdate()
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

    if (GetLocalRole() == ROLE_Authority)
    {
        FString healthMsg = FString::Printf(TEXT("%s now has %f health remaining."), *GetName(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);
    }

    // TOOD
}