#include "ThirdPersonMPWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Animation/AnimMontage.h"
#include "Engine/Classes/Sound/SoundBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ThirdPersonMPCharacter.h"

namespace
{
    static const TCHAR* FireSocketName = TEXT("Muzzle");
}

AThirdPersonMPWeapon::AThirdPersonMPWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->InitSphereRadius(32.0f);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    RootComponent = SphereComponent;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonMPWeapon::OnComponentBeginOverlap);

    static ConstructorHelpers::FObjectFinder<USoundBase> defaultSoundFinder(TEXT("/Game/FPWeapon/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02"));
    if (defaultSoundFinder.Succeeded())
    {
        WeaponSound = defaultSoundFinder.Object;
    }

    WeaponSoundLocation = FVector::ZeroVector;
    TraceDistance = 10000.0f;
    BaseDamage = 10.0f;

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

void AThirdPersonMPWeapon::OnComponentBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AThirdPersonMPCharacter* character = Cast<AThirdPersonMPCharacter>(OtherActor);
    if (IsValid(character))
    {
        character->EquipWeapon(this);
    }
}

void AThirdPersonMPWeapon::OnHandleFire_Implementation()
{
    UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, WeaponSoundLocation);

    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->PlayAnimMontage(FireFeedbackMotion);

        FTransform socketTransform = WeaponMesh->GetSocketTransform(FireSocketName, ERelativeTransformSpace::RTS_World);
        FVector fireLocation = socketTransform.GetLocation();
        FRotator fireRotation = socketTransform.Rotator();

        OnHandleFire_Server(fireLocation, fireRotation);
    }
}

void AThirdPersonMPWeapon::OnHandleFire_Server_Implementation(const FVector& FireLocation, const FRotator& FireRotation)
{
    FVector fireStart = FireLocation;
    FVector fireEnd = FireLocation + (UKismetMathLibrary::GetForwardVector(FireRotation) * TraceDistance);
    ETraceTypeQuery traceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
    bool bTraceComplex = false;
    TArray<AActor*> actorsToIgnore = { this, OwnerCharacter };
    EDrawDebugTrace::Type drawDebugTraceType = EDrawDebugTrace::Type::ForDuration;
    bool bIgnoreSelf = true;

    FHitResult hitResult;
    bool isHitted = UKismetSystemLibrary::LineTraceSingle(this, fireStart, fireEnd, traceChannel, bTraceComplex, actorsToIgnore, drawDebugTraceType, hitResult, bIgnoreSelf);

    if (isHitted && hitResult.bBlockingHit)
    {
        AThirdPersonGameCharacter* gameCharacter = Cast<AThirdPersonGameCharacter>(hitResult.GetActor());
        if (IsValid(gameCharacter))
        {
            AActor* damagedActor = gameCharacter;
            float baseDamage = BaseDamage;
            AController* eventInstigator = OwnerCharacter->GetInstigatorController();
            AActor* damageCauser = this;
            TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();

            UGameplayStatics::ApplyDamage(gameCharacter, baseDamage, eventInstigator, damageCauser, damageTypeClass);
        }
    }
}