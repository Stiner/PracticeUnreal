// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PRACTICEUNREAL_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Actor")
    TObjectPtr<class ABaseWeapon> Weapon;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay|Parameter")
    float WalkSpeedRate;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay|Parameter")
    float CrouchHeight;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay|Parameter")
    float TurnRateGamepad;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gameplay|Input")
    bool IsJumpButtonDown;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gameplay|Input")
    bool IsIronsightButtonDown;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gameplay|Input")
    bool IsCrouchingButtonDown;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, Category = "Gameplay|Replicate")
    bool UsingWeapon;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, Category = "Gameplay|Replicate")
    FRotator AimRotation;

    UPROPERTY(ReplicatedUsing = OnRep_IsIronsight)
    bool IsIronsight;

    UPROPERTY(ReplicatedUsing = OnRep_IsCrouching)
    bool IsCrouching;

public:
    APlayerCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay|Action")
    void EquipWeapon(class ABaseWeapon* equipWeapon);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay|Action")
    void UnequipWeapon();

    UFUNCTION(BlueprintCallable, Category = "Action")
    virtual void OnEndUseWeapon() override;

protected:
    virtual void EquipWeapon_Implementation(class ABaseWeapon* equipWeapon);
    virtual void UnequipWeapon_Implementation();

    virtual void StartUseWeapon() override;

    UFUNCTION(Server, Reliable, Category = "Gameplay|Server")
    void OnStartUseWeapon_Server();

    UFUNCTION(Server, Reliable, Category = "Gameplay|Server")
    void OnEndUseWeapon_Server();

    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Gameplay|Server")
    void UpdateAimDirection_Server(FRotator rotation);

    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Gameplay|Server")
    void UpdateIronsight_Server(bool value);

    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Gameplay|Server")
    void UpdateCrouch_Server(bool value);

    UFUNCTION()
    void OnRep_IsIronsight();

    UFUNCTION()
    void OnRep_IsCrouching();
};