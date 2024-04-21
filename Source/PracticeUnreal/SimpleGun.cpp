// Practice Unreal by Stiner

#include "SimpleGun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "PlayerCharacter.h"

#define WEAPON_SOCKET_NAME (TEXT("Muzzle"))

ASimpleGun::ASimpleGun()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->InitSphereRadius(32.0f);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

    RootComponent = SphereComponent;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);

    WeaponSoundLocation = FVector::ZeroVector;
    TraceDistance = 10000.0f;
    BaseDamage = 10.0f;

    SetReplicates(true);
}

void ASimpleGun::StartUse()
{
    Super::StartUse();

    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;

        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &ASimpleGun::EndUse, FireRate, false);

        OnHandleFire();
    }
}

void ASimpleGun::EndUse()
{
    Super::EndUse();

    IsFiringWeapon = false;

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->OnEndUseWeapon();
    }
}

void ASimpleGun::OnHandleFire_Implementation()
{
    FVector worldPositionSocket; FQuat worldRotationSocket;
    WeaponMesh->GetSocketWorldLocationAndRotation(WEAPON_SOCKET_NAME, worldPositionSocket, worldRotationSocket);

    UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, worldPositionSocket);

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->PlayAnimMontage(FireFeedbackMotion);

        FTransform socketTransform = WeaponMesh->GetSocketTransform(WEAPON_SOCKET_NAME, ERelativeTransformSpace::RTS_World);
        FVector fireLocation = socketTransform.GetLocation();
        FRotator fireDirection = socketTransform.Rotator();

        OnHandleFire_Server(fireLocation, fireDirection);
    }
}

void ASimpleGun::OnHandleFire_Server_Implementation(const FVector& FireLocation, const FRotator& FireDirection)
{
    FVector fireStart = FireLocation;
    FVector fireEnd = FireLocation + (UKismetMathLibrary::GetForwardVector(FireDirection) * TraceDistance);
    ETraceTypeQuery traceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
    bool bTraceComplex = false;
    TArray<AActor*> actorsToIgnore = { this, OwnerCharacter };
    EDrawDebugTrace::Type drawDebugTraceType = EDrawDebugTrace::Type::ForDuration;
    bool bIgnoreSelf = true;

    FHitResult hitResult;
    bool isHitted = UKismetSystemLibrary::LineTraceSingle(this, fireStart, fireEnd, traceChannel, bTraceComplex, actorsToIgnore, drawDebugTraceType, hitResult, bIgnoreSelf);
    if (isHitted && hitResult.bBlockingHit)
    {
        ABaseCharacter* gameCharacter = Cast<ABaseCharacter>(hitResult.GetActor());
        if (IsValid(gameCharacter))
        {
            if (HasAuthority())
            {
                AActor* damagedActor = gameCharacter;
                float baseDamage = BaseDamage;
                AController* eventInstigator = OwnerCharacter->GetInstigatorController();
                AActor* damageCauser = this;
                TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();

                UGameplayStatics::ApplyDamage(gameCharacter, baseDamage, eventInstigator, damageCauser, damageTypeClass);
            }
        }

        OnHitFire_Multicast(hitResult.Location, hitResult.ImpactNormal.Rotation());
    }
}

void ASimpleGun::OnHitFire_Multicast_Implementation(const FVector& HitLocation, const FRotator& HitDirection)
{
    if (!HasAuthority())
    {
        FString msg = FString::Printf(TEXT("[Client] Baam! [%s] pos:%s"), *GetName(), *HitLocation.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, msg);
        UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, HitLocation, (-1 * HitDirection));
    }
}