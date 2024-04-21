// Practice Unreal by Stiner

#include "BaseCharacter.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "NetPlayGameMode.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseCharacter, CurrentHealth);
}

void ABaseCharacter::OnEndUseWeapon()
{
}

void ABaseCharacter::StartUseWeapon()
{
}

void ABaseCharacter::SetCurrentHealth(float healthValue)
{
    if (HasAuthority())
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
        OnHealthUpdate();
    }
}

void ABaseCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void ABaseCharacter::OnHealthUpdate()
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

float ABaseCharacter::TakeDamage(float damageTaken, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    float damageApplied = CurrentHealth - damageTaken;
    SetCurrentHealth(damageApplied);

    return damageApplied;
}

void ABaseCharacter::OnDead_Server_Implementation()
{
    if (HasAuthority())
    {
        FString healthMsg = FString::Printf(TEXT("%s Die on Server"), *GetName());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, healthMsg);

        Dead();
    }
}

void ABaseCharacter::Dead_Implementation()
{
    FString dieMsg = FString::Printf(TEXT("%s DIE"), *GetName());
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dieMsg);

    Destroy();

    ANetPlayGameMode* gameMode = Cast<ANetPlayGameMode>(UGameplayStatics::GetGameMode(this));
    if (IsValid(gameMode))
    {
        gameMode->OnDeadPlayerCharacter(GetController());
    }
}
