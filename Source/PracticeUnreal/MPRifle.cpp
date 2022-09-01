#include "MPRifle.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

	FireRate = 0.1f;
	_bIsFiringWeapon = false;
}

void AMPRifle::StartFire()
{
	if (!_bIsFiringWeapon)
	{
		_bIsFiringWeapon = true;
		UWorld* world = GetWorld();
		world->GetTimerManager().SetTimer(_firingTimer, this, &AMPRifle::StopFire, FireRate, false);
		HandleFire();
	}
}

void AMPRifle::StopFire()
{
	_bIsFiringWeapon = false;
}

void AMPRifle::HandleFire_Implementation()
{
	AActor* parentActor = GetAttachParentActor();
    if (!parentActor)
        return;

	APawn* instigator = parentActor->GetInstigator();
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