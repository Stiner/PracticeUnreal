// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PUGameModeBase.generated.h"

UCLASS()
class PRACTICEUNREAL_API APUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    APUGameModeBase();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void OnDeadPlayerCharacter(class AController* deadPlayer);

    virtual void OnPostLogin(class AController* NewPlayer) override;
};
