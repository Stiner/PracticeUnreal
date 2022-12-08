#include "FirstPersonMPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MPRifle.h"

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

float AFirstPersonMPCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;
}

void AFirstPersonMPCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void AFirstPersonMPCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
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

void AFirstPersonMPCharacter::EquipWeapon_Implementation(const AMPRifle* equipWeapon)
{
}

void AFirstPersonMPCharacter::UseWeapon()
{
    if (IsValid(Weapon))
    {
        Weapon->OnHandleFire();
    }

    HandleFireOnServer();
}

void AFirstPersonMPCharacter::HandleFireOnServer_Implementation()
{
    
}