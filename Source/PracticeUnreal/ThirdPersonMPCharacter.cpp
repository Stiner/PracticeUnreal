// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonMPCharacter.h"

#include "Net/UnrealNetwork.h"
#include "ThirdPersonMPWeapon.h"

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
