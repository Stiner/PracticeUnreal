// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonMPCharacter.generated.h"

class AMPRfile;

UCLASS()
class PRACTICEUNREAL_API AThirdPersonMPCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
        AMPRifle* Weapon;

    UPROPERTY(EditDefaultsOnly, Category = "Health")
        float MaxHealth;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
        float CurrentHealth;

    UPROPERTY(Replicated)
        bool UsingWeapon;

public:
    AThirdPersonMPCharacter();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
        void SetCurrentHealth(float healthValue);

    UFUNCTION(BlueprintCallable, Category = "Health")
        float TakeDamage(float damageTaken, struct FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
        void EquipWeapon(AMPRifle* equipWeapon);
    virtual void EquipWeapon_Implementation(AMPRifle* equipWeapon);

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

    UFUNCTION()
        void OnRep_CurrentHealth();

    void OnHealthUpdate();
};