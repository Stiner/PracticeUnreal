// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonMPGameModeBase.generated.h"

class AController;

/**
 *
 */
UCLASS()
class PRACTICEUNREAL_API AThirdPersonMPGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AThirdPersonMPGameModeBase();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
        void OnDeadPlayerCharacter(AController* deadPlayer);

    virtual void OnPostLogin(AController* NewPlayer) override;
};
