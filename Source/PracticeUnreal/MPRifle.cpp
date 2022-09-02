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

void AMPRifle::OnEquiped(AFirstPersonMPCharacter* equipedCharacter)
{
    
}

void AMPRifle::StartFire()
{
    if (!_bIsFiringWeapon)
    {
        _bIsFiringWeapon = true;
        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &AMPRifle::StopFire, FireRate, false);

        FString msg = FString::Printf(TEXT("Role:%d"), GetLocalRole());
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, msg);

        OnHandleFire();
    }
}

void AMPRifle::StopFire()
{
    _bIsFiringWeapon = false;
}

void AMPRifle::OnHandleFire()
{
    if (!OwnerCharacter)
        return;

    APawn* instigator = OwnerCharacter->GetInstigator();
    if (!instigator)
        return;

    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (!playerController)
        return;

    FVector spawnLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
    FRotator spawnRotation = playerController->PlayerCameraManager->GetCameraRotation();

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = instigator;
    spawnParameters.Owner = this;

    AFirstPersonMPProjectile* projectile = GetWorld()->SpawnActor<AFirstPersonMPProjectile>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}