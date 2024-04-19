// Practice Unreal by Stiner

#include "PUWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "PUCharacter.h"

#define WEAPON_SOCKET_NAME (TEXT("Muzzle"))

APUWeapon::APUWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->InitSphereRadius(32.0f);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

    RootComponent = SphereComponent;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<USoundBase> defaultSoundFinder(TEXT("/Game/FPWeapon/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02"));
    if (defaultSoundFinder.Succeeded())
    {
        WeaponSound = defaultSoundFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> defaultExplosionEffect(TEXT("/Game/Particles/P_Explosion.P_Explosion"));
    if (defaultExplosionEffect.Succeeded())
    {
        ExplosionEffect = defaultExplosionEffect.Object;
    }

    WeaponSoundLocation = FVector::ZeroVector;
    TraceDistance = 10000.0f;
    BaseDamage = 10.0f;

    SetReplicates(true);
}

void APUWeapon::BeginPlay()
{
    Super::BeginPlay();

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APUWeapon::OnBeginOverlap);
}

void APUWeapon::SetOwner(AActor* NewOwner)
{
    Super::SetOwner(NewOwner);

    OwnerCharacter = Cast<APUCharacter>(NewOwner);
}

void APUWeapon::StartFire()
{
    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;

        UWorld* world = GetWorld();
        world->GetTimerManager().SetTimer(_firingTimer, this, &APUWeapon::EndFire, FireRate, false);

        OnHandleFire();
    }
}

void APUWeapon::EndFire()
{
    IsFiringWeapon = false;

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->OnEndUseWeapon();
    }
}

void APUWeapon::OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APUCharacter* character = Cast<APUCharacter>(OtherActor);
    if (IsValid(character))
    {
        character->EquipWeapon(this);
    }
}

void APUWeapon::OnHandleFire_Implementation()
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

void APUWeapon::OnHandleFire_Server_Implementation(const FVector& FireLocation, const FRotator& FireDirection)
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
        APUBaseCharacter* gameCharacter = Cast<APUBaseCharacter>(hitResult.GetActor());
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

void APUWeapon::OnHitFire_Multicast_Implementation(const FVector& HitLocation, const FRotator& HitDirection)
{
    if (!HasAuthority())
    {
        FString msg = FString::Printf(TEXT("[Client] Baam! [%s] pos:%s"), *GetName(), *HitLocation.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, msg);
        UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, HitLocation, (-1 * HitDirection));
    }
}