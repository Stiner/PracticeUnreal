// Practice Unreal by Stiner

#include "PUCharacter.h"
#include "Net/UnrealNetwork.h"
#include "PUWeapon.h"

#define ATTACH_TO_BONE_NAME (TEXT("Weapon_R"))

APUCharacter::APUCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    WalkSpeedRate = 0.25f;
    CrouchHeight = 70.0f;
    TurnRateGamepad = 45.0f;
}

void APUCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APUCharacter, UsingWeapon);
    DOREPLIFETIME(APUCharacter, AimRotation);
    DOREPLIFETIME(APUCharacter, IsIronsight);
    DOREPLIFETIME(APUCharacter, IsCrouching);
}

void APUCharacter::EquipWeapon_Implementation(APUWeapon* equipWeapon)
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

void APUCharacter::UnequipWeapon_Implementation()
{
    Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    Weapon->SetOwner(nullptr);
    Weapon = nullptr;
}

void APUCharacter::StartUseWeapon()
{
    if (IsValid(Weapon))
    {
        Weapon->StartFire();

        OnStartUseWeapon_Server();
    }
}

void APUCharacter::OnEndUseWeapon()
{
    OnEndUseWeapon_Server();
}

void APUCharacter::OnStartUseWeapon_Server_Implementation()
{
    UsingWeapon = true;
}

void APUCharacter::OnEndUseWeapon_Server_Implementation()
{
    UsingWeapon = false;
}

void APUCharacter::UpdateAimDirection_Server_Implementation(FRotator rotation)
{
    if (HasAuthority())
    {
        AimRotation = rotation;
    }
}

void APUCharacter::UpdateIronsight_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsIronsight = value;
    }
}

void APUCharacter::UpdateCrouch_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsCrouching = value;
    }
}

void APUCharacter::OnRep_IsIronsight()
{
    if (!HasAuthority())
    {
        IsIronsightButtonDown = IsIronsight;
    }
}

void APUCharacter::OnRep_IsCrouching()
{
    if (!HasAuthority())
    {
        IsCrouchingButtonDown = IsCrouching;
    }
}
