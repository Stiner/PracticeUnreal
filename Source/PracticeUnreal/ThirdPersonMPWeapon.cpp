#include "ThirdPersonMPWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Animation/AnimMontage.h"
#include "Engine/Classes/Sound/SoundBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "ThirdPersonMPCharacter.h"
#include "ThirdPersonHitEmitter.h"

#define WEAPON_SOCKET_NAME (TEXT("Muzzle"))

AThirdPersonMPWeapon::AThirdPersonMPWeapon()
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

void AThirdPersonMPWeapon::BeginPlay()
{
    Super::BeginPlay();

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonMPWeapon::OnBeginOverlap);
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

void AThirdPersonMPWeapon::OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AThirdPersonMPCharacter* character = Cast<AThirdPersonMPCharacter>(OtherActor);
    if (IsValid(character))
    {
        character->EquipWeapon(this);
    }
}

void AThirdPersonMPWeapon::OnHandleFire_Implementation()
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

void AThirdPersonMPWeapon::OnHandleFire_Server_Implementation(const FVector& FireLocation, const FRotator& FireDirection)
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
        AThirdPersonBaseCharacter* gameCharacter = Cast<AThirdPersonBaseCharacter>(hitResult.GetActor());
        if (IsValid(gameCharacter))
        {
            AActor* damagedActor = gameCharacter;
            float baseDamage = BaseDamage;
            AController* eventInstigator = OwnerCharacter->GetInstigatorController();
            AActor* damageCauser = this;
            TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();

            UGameplayStatics::ApplyDamage(gameCharacter, baseDamage, eventInstigator, damageCauser, damageTypeClass);

            GetWorld()->SpawnActor<AThirdPersonHitEmitter>(hitResult.Location, FireDirection);
        }
    }
}