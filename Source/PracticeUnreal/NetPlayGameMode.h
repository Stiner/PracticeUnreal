// Practice Unreal by Stiner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetPlayGameMode.generated.h"

UCLASS()
class PRACTICEUNREAL_API ANetPlayGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANetPlayGameMode();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void OnDeadPlayerCharacter(class AController* deadPlayer);

    virtual void OnPostLogin(class AController* NewPlayer) override;
};
