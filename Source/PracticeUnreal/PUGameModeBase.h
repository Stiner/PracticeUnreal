// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PUGameModeBase.generated.h"

class AController;

/**
 *
 */
UCLASS()
class PRACTICEUNREAL_API APUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    APUGameModeBase();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void OnDeadPlayerCharacter(AController* deadPlayer);

    virtual void OnPostLogin(AController* NewPlayer) override;
};
