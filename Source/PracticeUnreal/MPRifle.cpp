#include "MPRifle.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	ProjectileClass = AFirstPersonMPProjectile::StaticClass();

	FireRate = 0.25f;
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
	AActor* actor = GetAttachParentActor();
	if (actor)
	{
		APawn* instigator = actor->GetInstigator();
		if (instigator)
		{
			FVector spawnLocation = WeaponMesh->GetSocketLocation(TEXT("Ammo"));
			FRotator spawnRotation = GetActorRotation();

			FActorSpawnParameters spawnParameters;
			spawnParameters.Instigator = instigator;
			spawnParameters.Owner = this;

			AFirstPersonMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AFirstPersonMPProjectile>(spawnLocation, spawnRotation, spawnParameters);
		}
	}
}