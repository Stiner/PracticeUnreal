#include "MPRifle.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ThirdPersonMPCharacter.h"
#include "ThirdPersonMPProjectile.h"

AMPRifle::AMPRifle()
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

void AMPRifle::SetOwner(AActor* NewOwner)
{
    Super::SetOwner(NewOwner);

    OwnerCharacter = Cast<AThirdPersonMPCharacter>(NewOwner);
}

void AMPRifle::StartFire()
{
    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;

        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &AMPRifle::EndFire, FireRate, false);

        OnHandleFire();
        //OnHandleFire_Server();
    }
}

void AMPRifle::EndFire()
{
    IsFiringWeapon = false;

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->OnEndUseWeapon();
    }
}

void AMPRifle::OnHandleFire_Implementation()
{
}

void AMPRifle::OnHandleFire_Server_Implementation()
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