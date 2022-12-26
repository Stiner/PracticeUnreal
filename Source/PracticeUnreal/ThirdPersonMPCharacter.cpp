// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonMPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ThirdPersonMPWeapon.h"

namespace
{
#define ATTACH_TO_BONE_NAME (TEXT("Weapon_R"))
}

AThirdPersonMPCharacter::AThirdPersonMPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonMPCharacter, UsingWeapon);
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
