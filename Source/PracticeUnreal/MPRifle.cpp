#include "MPRifle.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FirstPersonMPCharacter.h"
#include "FirstPersonMPProjectile.h"

AMPRifle::AMPRifle()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->InitSphereRadius(32.0f);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    RootComponent = SphereComponent;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);
}

void AMPRifle::OnHandleFire_Implementation()
{
    if (!IsValid(OwnerCharacter))
        return;

    APawn* instigator = OwnerCharacter->GetInstigator();
    if (!IsValid(instigator))
        return;

    FVector spawnLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
    FRotator spawnRotation = WeaponMesh->GetSocketRotation(TEXT("Muzzel"));

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = instigator;
    spawnParameters.Owner = Owner;

    AFirstPersonMPProjectile* projectile = GetWorld()->SpawnActor<AFirstPersonMPProjectile>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}

void AMPRifle::StartFire()
{
    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;

        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &AMPRifle::StopFire, FireRate, false);

        FString msg = FString::Printf(TEXT("Role:%d"), GetLocalRole());
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, msg);
    }
}

void AMPRifle::StopFire()
{
    IsFiringWeapon = false;
}
