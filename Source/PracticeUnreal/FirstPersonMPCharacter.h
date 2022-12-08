// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonMPCharacter.generated.h"

class AMPRfile;

UCLASS()
class PRACTICEUNREAL_API AFirstPersonMPCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFirstPersonMPCharacter();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
        void SetCurrentHealth(float healthValue);

    UFUNCTION(BlueprintCallable, Category = "Health")
        float TakeDamage(float damageTaken, struct FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

protected:
    UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
        AMPRifle* Weapon;

    UPROPERTY(EditDefaultsOnly, Category = "Health")
        float MaxHealth;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
        float CurrentHealth;

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
        void EquipWeapon(const AMPRifle* equipWeapon);
    virtual void EquipWeapon_Implementation(const AMPRifle* equipWeapon);

protected:
    UFUNCTION(BlueprintCallable, Category = "Action")
        void UseWeapon();

    UFUNCTION(Server, Reliable)
        void HandleFireOnServer();

    UFUNCTION()
        void OnRep_CurrentHealth();

    void OnHealthUpdate();
};