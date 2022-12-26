// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonMPGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PRACTICEUNREAL_API AThirdPersonMPGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AThirdPersonMPGameModeBase();

    UFUNCTION(BlueprintNativeEvent, Category = "Gameplay")
        void OnDeadPlayerCharacter();

protected:
    virtual void OnDeadPlayerCharacter_Implementation();
};
