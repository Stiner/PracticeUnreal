// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonMPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ThirdPersonMPWeapon.h"

#define ATTACH_TO_BONE_NAME (TEXT("Weapon_R"))

AThirdPersonMPCharacter::AThirdPersonMPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    WalkSpeedRate = 0.25f;
    CrouchHeight = 70.0f;
    TurnRateGamepad = 45.0f;
}

void AThirdPersonMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonMPCharacter, UsingWeapon);
    DOREPLIFETIME(AThirdPersonMPCharacter, AimRotation);
    DOREPLIFETIME(AThirdPersonMPCharacter, IsIronsight);
    DOREPLIFETIME(AThirdPersonMPCharacter, IsCrouching);
}

void AThirdPersonMPCharacter::EquipWeapon_Implementation(AThirdPersonMPWeapon* equipWeapon)
{
    if (   IsValid(equipWeapon)
        && !IsValid(equipWeapon->GetOwner())
        && !IsValid(Weapon))
    {
        Weapon = equipWeapon;
        Weapon->SetOwner(this);
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ATTACH_TO_BONE_NAME);
    }
}

void AThirdPersonMPCharacter::UnequipWeapon_Implementation()
{
    Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    Weapon->SetOwner(nullptr);
    Weapon = nullptr;
}

void AThirdPersonMPCharacter::StartUseWeapon()
{
    if (IsValid(Weapon))
    {
        Weapon->StartFire();

        OnStartUseWeapon_Server();
    }
}

void AThirdPersonMPCharacter::OnEndUseWeapon()
{
    OnEndUseWeapon_Server();
}

void AThirdPersonMPCharacter::OnStartUseWeapon_Server_Implementation()
{
    UsingWeapon = true;
}

void AThirdPersonMPCharacter::OnEndUseWeapon_Server_Implementation()
{
    UsingWeapon = false;
}

void AThirdPersonMPCharacter::UpdateAimDirection_Server_Implementation(FRotator rotation)
{
    if (HasAuthority())
    {
        AimRotation = rotation;
    }
}

void AThirdPersonMPCharacter::UpdateIronsight_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsIronsight = value;
    }
}

void AThirdPersonMPCharacter::UpdateCrouch_Server_Implementation(bool value)
{
    if (HasAuthority())
    {
        IsCrouching = value;
    }
}

void AThirdPersonMPCharacter::OnRep_IsIronsight()
{
    if (!HasAuthority())
    {
        IsIronsightButtonDown = IsIronsight;
    }
}

void AThirdPersonMPCharacter::OnRep_IsCrouching()
{
    if (!HasAuthority())
    {
        IsCrouchingButtonDown = IsCrouching;
    }
}
