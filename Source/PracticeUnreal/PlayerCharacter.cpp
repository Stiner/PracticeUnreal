// Practice Unreal by Stiner

#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "BaseWeapon.h"

#define ATTACH_TO_BONE_NAME (TEXT("Weapon_R"))

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    WalkSpeedRate = 0.25f;
    CrouchHeight = 70.0f;
    TurnRateGamepad = 45.0f;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerCharacter, UsingWeapon);
    DOREPLIFETIME(APlayerCharacter, AimRotation);
    DOREPLIFETIME(APlayerCharacter, IsIronsight);
    DOREPLIFETIME(APlayerCharacter, IsCrouching);
}

void APlayerCharacter::EquipWeapon_Implementation(ABaseWeapon* equipWeapon)
{
    if (IsValid(equipWeapon)
        && !IsValid(equipWeapon->GetOwner())
        && !IsValid(Weapon))
    {
        Weapon = equipWeapon;
        Weapon->SetOwner(this);
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ATTACH_TO_BONE_NAME);
    }
}

void APlayerCharacter::UnequipWeapon_Implementation()
{
    Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    Weapon->SetOwner(nullptr);
    Weapon = nullptr;
}

void APlayerCharacter::StartUseWeapon()
{
    Super::StartUseWeapon();

    if (IsValid(Weapon))
    {
        Weapon->StartUse();

        OnStartUseWeapon_Server();
    }
}

void APlayerCharacter::OnEndUseWeapon()
{
    OnEndUseWeapon_Server();
}

void APlayerCharacter::OnStartUseWeapon_Server_Implementation()
{
    UsingWeapon = true;
}

void APlayerCharacter::OnEndUseWeapon_Server_Implementation()
{
    UsingWeapon = false;
}

void APlayerCharacter::UpdateAimDirection_Server_Implementation(FRotator rotation)
{
    if (HasAuthority())
    {
        AimRotation = rotation;
    }
}

void APlayerCharacter::UpdateIronsight_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsIronsight = value;
    }
}

void APlayerCharacter::UpdateCrouch_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsCrouching = value;
    }
}

void APlayerCharacter::OnRep_IsIronsight()
{
    if (!HasAuthority())
    {
        IsIronsightButtonDown = IsIronsight;
    }
}

void APlayerCharacter::OnRep_IsCrouching()
{
    if (!HasAuthority())
    {
        IsCrouchingButtonDown = IsCrouching;
    }
}
