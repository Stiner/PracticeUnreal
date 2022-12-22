#include "ThirdPersonMPWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ThirdPersonMPCharacter.h"
#include "ThirdPersonMPProjectile.h"

AThirdPersonMPWeapon::AThirdPersonMPWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->InitSphereRadius(32.0f);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    RootComponent = SphereComponent;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);

    SetReplicates(true);
}

void AThirdPersonMPWeapon::SetOwner(AActor* NewOwner)
{
    Super::SetOwner(NewOwner);

    OwnerCharacter = Cast<AThirdPersonMPCharacter>(NewOwner);
}

void AThirdPersonMPWeapon::StartFire()
{
    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;

        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &AThirdPersonMPWeapon::EndFire, FireRate, false);

        OnHandleFire();
    }
}

void AThirdPersonMPWeapon::EndFire()
{
    IsFiringWeapon = false;

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->OnEndUseWeapon();
    }
}

void AThirdPersonMPWeapon::OnHandleFire_Implementation()
{
}

void AThirdPersonMPWeapon::OnHandleFire_Server_Implementation()
{
    if (!IsValid(GetOwner()))
        return;

    APawn* instigator = GetOwner()->GetInstigator();
    if (!IsValid(instigator))
        return;

    FVector spawnLocation;
    FRotator spawnRotation;

    WeaponMesh->GetSocketWorldLocationAndRotation(TEXT("Muzzle"), spawnLocation, spawnRotation);

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = instigator;
    spawnParameters.Owner = Owner;

    AThirdPersonMPProjectile* projectile = GetWorld()->SpawnActor<AThirdPersonMPProjectile>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}