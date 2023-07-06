// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonBaseCharacter.generated.h"

UCLASS()
class PRACTICEUNREAL_API AThirdPersonBaseCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Health")
        float MaxHealth;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
        float CurrentHealth;

public:
    // Sets default values for this character's properties
    AThirdPersonBaseCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
        FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
        float TakeDamage(float damageTaken, struct FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
        void OnDead_Server();

protected:
    UFUNCTION(BlueprintCallable, Category = "Health")
        void SetCurrentHealth(float healthValue);

    UFUNCTION()
        void OnRep_CurrentHealth();

    void OnHealthUpdate();

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
        void Dead();
    virtual void Dead_Implementation();
};
