// Practice Unreal by Stiner

#include "PUGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PUPlayerController.h"

APUGameModeBase::APUGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APUGameModeBase::OnDeadPlayerCharacter(AController* deadPlayer)
{
    APUPlayerController* playerController = Cast<APUPlayerController>(deadPlayer);
    if (IsValid(playerController))
    {
        UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Type::Quit, false);
    }
}

void APUGameModeBase::OnPostLogin(AController* NewPlayer)
{
    const FString msg = FString::Printf(TEXT("Logged in : %s"), *(NewPlayer->GetName()));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, msg);
}
