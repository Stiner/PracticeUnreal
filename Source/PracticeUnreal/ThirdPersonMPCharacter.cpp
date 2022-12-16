#include "ThirdPersonMPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MPRifle.h"

AThirdPersonMPCharacter::AThirdPersonMPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void AThirdPersonMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonMPCharacter, CurrentHealth);
}

float AThirdPersonMPCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;
}

void AThirdPersonMPCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void AThirdPersonMPCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void AThirdPersonMPCharacter::OnHealthUpdate()
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

void AThirdPersonMPCharacter::EquipWeapon_Implementation(AMPRifle* equipWeapon)
{
    if (IsValid(equipWeapon))
    {
        if (IsValid(equipWeapon->Owner))
            return;
        if (IsValid(Weapon))
            return;

        Weapon = equipWeapon;

        Weapon->SetOwner(this);
    }
}

void AThirdPersonMPCharacter::UnequipWeapon_Implementation()
{
    if (IsValid(Weapon))
    {
        Weapon->SetOwner(nullptr);
        Weapon = nullptr;
    }
}

void AThirdPersonMPCharacter::StartUseWeapon()
{
    if (IsValid(Weapon))
    {
        Weapon->StartFire();

        StartUseWeapon_Server();
    }
}

void AThirdPersonMPCharacter::OnEndUseWeapon()
{
    OnEndUseWeapon_Server();
}

void AThirdPersonMPCharacter::StartUseWeapon_Server_Implementation()
{
    UsingWeapon = true;
}

void AThirdPersonMPCharacter::OnEndUseWeapon_Server_Implementation()
{
    UsingWeapon = false;
}
