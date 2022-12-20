// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonGameCharacter.h"
#include "ThirdPersonMPCharacter.generated.h"

class AThirdPersonMPWeapon;

UCLASS()
class PRACTICEUNREAL_API AThirdPersonMPCharacter : public AThirdPersonGameCharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
        AThirdPersonMPWeapon* Weapon;

    UPROPERTY(Replicated)
        bool UsingWeapon;

public:
    AThirdPersonMPCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
        void EquipWeapon(AThirdPersonMPWeapon* equipWeapon);
    virtual void EquipWeapon_Implementation(AThirdPersonMPWeapon* equipWeapon);

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
        void UnequipWeapon();
    virtual void UnequipWeapon_Implementation();

    UFUNCTION(Category = "Action")
        void OnEndUseWeapon();

protected:
    UFUNCTION(BlueprintCallable, Category = "Action")
        void StartUseWeapon();

    UFUNCTION(Server, Reliable)
        void StartUseWeapon_Server();

    UFUNCTION(Server, Reliable)
        void OnEndUseWeapon_Server();
};